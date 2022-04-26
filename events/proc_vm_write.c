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


MODULE_DESCRIPTION("Process VM write hook");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");


#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

void error_vmwrite(int ret)
{
	switch (ret) {
		    case EINVAL:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: liovcnt OR riovcnt IS TOO LARGE.\n",current->comm, current->pid);
		    	break;
		    case EFAULT:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: MEMORY OUTSIDE PROCESS RANGE.\n", current->comm, current->pid);
		    	break;
		    case ENOMEM:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: UNABLE TO ALLOCATE MEMORY.\n",current->comm, current->pid);
		    	break;
		    case EPERM:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: CALLER DOESN'T HAVE SUFFICIENT PERMISSION.\n",current->comm, current->pid);
		    	break;
		    case ESRCH:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: PROCESS ID DOES NOT EXIST.\n",current->comm, current->pid);
		    	break;
		    default:
		      pr_info(",Memory allocation Error in process %s(%d): ERROR: UNKNOWN..\n",current->comm, current->pid);
		}

}

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_process_vm_writev)(struct pt_regs *regs);


static asmlinkage long fh_sys_process_vm_writev(struct pt_regs *regs)
{
	long ret;
	ret = real_sys_process_vm_writev(regs);
	if (ret < 0) {
		error_vmwrite(abs(ret));
		return ret;
    }
	pr_info(",Process %s(%d) performed memory allocation in foreign process PID:%ld of size %ld bytes at address 0x%lx\n\n", current->comm, current->pid, regs->di, ret, regs->cx);
	return ret;
}

#else
static asmlinkage long (*real_sys_process_vm_writev)(pid_t pid, const struct iovec __user *lvec,
	unsigned long liovcnt, const struct iovec __user *rvec, unsigned long riovcnt, unsigned long flags);


static asmlinkage long fh_sys_process_vm_writev(pid_t pid, const struct iovec __user *lvec,
	unsigned long liovcnt, const struct iovec __user *rvec, unsigned long riovcnt, unsigned long flags)
{
	long ret;

	ret = real_sys_process_vm_writev(pid, lvec, liovcnt, rvec, riovcnt, flags);
	if (ret < 0) {
		error_vmwrite(abs(ret));
		return ret;
    }
	pr_info(",Process %s(%d) did memory allocation in foreign process PID:%ld of size %ld bytes at address 0x%lx\n\n", current->comm, current->pid, pid, ret, rvec);

	return ret;
}
#endif
