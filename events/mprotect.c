
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

MODULE_DESCRIPTION("Mprotect module");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");


#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

void error_mprotect(int ret)
{
	switch (ret) {
		    case EINVAL:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: ADDRESS NOT VALID OR NOT PAGE ALIGNED.\n",current->comm, current->pid);
		    	break;
		    case EFAULT:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n", current->comm, current->pid);
		    	break;
		    case ENOMEM:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: ADDRESS IN INVALID RANGE OR KERNEL STRUCTURE ALLOCATION ERROR.\n",current->comm, current->pid);
		    	break;
			case EACCES:
				pr_info(",Memory allocation Error in process %s(%d): ERROR: MEMORY CANNOT GIVEN SPECIFIC ACCESS.\n",current->comm, current->pid);
		    	break;
		    default:
		      pr_info(",Memory allocation Error in process %s(%d): ERROR: UNKNOWN..\n",current->comm, current->pid);
		}

}

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_mprotect)(struct pt_regs *regs);

static asmlinkage long fh_sys_mprotect(struct pt_regs *regs)
{
	long ret;
	ret = real_sys_mprotect(regs);
	pr_info(",aabb %ld", ret);
	if (ret < 0) {
		error_mprotect(abs(ret));
		return ret;
    }

	//if (strncmp(current->comm, "demo1", 5) == 0) {
		//pr_info(" mem_alloc, Process: %s(%d), address: %lx, size: %ld, Memory permission: %ld, flag: %lx, fd: %ld\n",current->comm, current->pid, ret, regs->si, regs->dx, regs->cx, regs->r8);
	pr_info(",mprotect,%s(%d),0x%lx,0x%lx,permission%ld\n",current->comm, current->pid, regs->di, regs->si, regs->dx);

	//}
	return ret;
}
#else
static asmlinkage long (*real_sys_mprotect)(unsigned long start, size_t len,
				unsigned long prot);

static asmlinkage long fh_sys_mprotect(unsigned long start, size_t len,
				unsigned long prot)
{
	long ret;
	//struct task_struct *task;
	//task = current;

	ret = real_sys_mprotect(start, len, prot);
	if (ret < 0) {
		error_mprotect(abs(ret));
		return ret;
    }
	//pr_info(" mem_alloc, Process: %s(%d), address: %lx, size: %ld, Memory permission: %ld, flag: %lx, fd: %ld\n",current->comm, current->pid, ret, regs->si, regs->dx, regs->cx, regs->r8);
	pr_info(",mprotect,%s(%d),0x%lx,0x%lx,permission%ld\n",current->comm, current->pid, start, len, prot);
	return ret;
}
#endif
