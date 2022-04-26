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


MODULE_DESCRIPTION("MemFD create hook");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");


#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

void error_memfd(int ret)
{
	switch (ret) {
		    case EFAULT:
		    	pr_info(",Memory FD allocation Error in process %s(%d): ERROR: ADDRESS IN NAMES POINT TO INVALID MEMORY.\n", current->comm, current->pid);
		    	break;
		    case EINVAL:
		    	pr_info(",Memory FD allocation Error in process %s(%d): ERROR: FLAGS CONTAIN INVALID BIT OR NAME TO LONG.\n",current->comm, current->pid);
		    	break;
		    case EMFILE:
		    	pr_info(",Memory FD allocation Error in process %s(%d): ERROR: PER PROCESS OR SYSTEM WIDE LIMIT OF OPEN FILE EXCEEDED.\n",current->comm, current->pid);
		    	break;
		    case ENOMEM:
		    	pr_info(",Memory FD allocation Error in process %s(%d): ERROR: INSUFFICIENT MEMORY.\n",current->comm, current->pid);
		    	break;
		    default:
		      pr_info(",Memory allocation Error in process %s(%d): ERROR: UNKNOWN\n",current->comm, current->pid);
		}

}

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_memfd_create)(struct pt_regs *regs);


static asmlinkage long fh_sys_memfd_create(struct pt_regs *regs)
{
	long ret = 0;
	ret = real_sys_memfd_create(regs);
	if (ret < 0) {
		error_memfd(abs(ret));
		return ret;
    }
	pr_info(",Process %s(%d) created File in memory with name %s and FD value %ld\n", current->comm, current->pid, regs->di, ret);
	return ret;
}

#else
static asmlinkage long (*real_sys_memfd_create)(const char __user *uname_ptr, unsigned int flags);

static asmlinkage long fh_sys_memfd_create(const char __user *uname_ptr, unsigned int flags)
{
	long ret;
	ret = real_sys_memfd_create(uname_ptr, flags);
	if (ret < 0) {
		error_memfd(abs(ret));
		return ret;
    }
	pr_info(",Process %s(%d) created FD in memory with name %s and FD value %ld\n", current->comm, current->pid, uname_ptr, ret);

	return ret;
}
#endif
