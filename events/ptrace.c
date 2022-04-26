#define pr_fmt(fmt) "memfini: " fmt

#include <linux/ftrace.h>
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/version.h>
#include <linux/kprobes.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <asm/signal.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_DESCRIPTION("Main module for memory monitor interface");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");


#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif


void error_ptrace(int ret)
{
	switch (ret) {
		    case EBUSY:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: ALLOCATING OR FREEING DEBUG REGISTER FAILED.\n",current->comm, current->pid);
		    	break;
		    case EFAULT:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: ATTEMPT TO READ OR WRITE INVALID MEMORY AREA.\n", current->comm, current->pid);
		    	break;
		    case EINVAL:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: ATTEMPT TO SET INVALID OPTION.\n",current->comm, current->pid);
		    	break;
			case EIO:
				pr_info(",Memory allocation Error in process %s(%d): ERROR: INVALID REQUEST.\n",current->comm, current->pid);
		    	break;
			case EPERM:
				pr_info(",Memory allocation Error in process %s(%d): ERROR: SPECIFIC PROCESS CANNOT BE TRACED.\n",current->comm, current->pid);
			    break;
			case ESRCH:
			    pr_info(",Memory allocation Error in process %s(%d): ERROR: PROCESS DOES NOT EXIST.\n",current->comm, current->pid);
			    break;
		    default:
		      pr_info(",Memory allocation Error in process %s(%d): ERROR: UNKNOWN..\n",current->comm, current->pid);
		}

}


#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_ptrace)(struct pt_regs *regs);

static asmlinkage long fh_sys_ptrace(struct pt_regs *regs)
{
	long ret;
	ret = real_sys_ptrace(regs);
	if (ret < 0) {
		error_ptrace(abs(ret));
		return ret;
    }
	if (regs->di == 4 || regs->di == 6)
	{
		pr_info(",Process %s(%d) performed memory allocation in foreign process PID:%ld of size 4 bytes at address 0x%lx\n", current->comm, current->pid, regs->si,regs->dx);
	}
	return ret;
}

#else
static asmlinkage long (*real_sys_ptrace)(int request, pid_t pid, void *addr, void *data);

static asmlinkage long fh_sys_ptrace(int request, pid_t pid, void *addr, void *data)
{
	long ret;
	ret = real_sys_ptrace(request, pid, addr, data);
	if (ret < 0) {
		error_ptrace(abs(ret));
		return ret;
    }
	if (request == 4 || request == 6)
	{
		pr_info(",Process %s(%d) did memory allocation in foreign process PID:%ld of size 4 bytes at address 0x%lx\n", current->comm, current->pid, pid, addr);
	}
	return ret;
}
#endif
