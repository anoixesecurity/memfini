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

MODULE_DESCRIPTION("brk hook module");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");


#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

void error_brk(int ret)
{
	pr_info(",Memory break Error in process %s(%d): ERROR: NO MEMORY AVAILABLE OR MAXIMUM MAPPING LIMIT EXCEEDED.\n",current->comm, current->pid);
}

#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_brk)(struct pt_regs *regs);

static asmlinkage long fh_sys_brk(struct pt_regs *regs)
{
	long ret;
	//long init_brk;
	//init_brk = current->mm->brk;
	ret = real_sys_brk(regs);
	if (strncmp(current->comm, "brk_test", 8) == 0) {

		if (ret < 0) {
			error_brk(abs(ret));
			return ret;
	    }
		if (regs->di !=0) {
			pr_info(",Process %s(%d) changed process break at address 0x%lx\n",current->comm, current->pid,  ret);
		}
		//long ret2;
		//ret2 = real_sys_brk(0);

	}

	return ret;
}
#else
static asmlinkage long (*real_sys_brk)(unsigned long brk);

static asmlinkage long fh_sys_brk(unsigned long brk)
{
	long ret;
	//struct task_struct *task;
	//task = current;

	ret = real_sys_brk(brk);
	if (ret < 0) {
		error_brk(abs(ret));
		return ret;
    }
	if (strncmp(current->comm, "brk_test", 8) == 0) {
		pr_info(",Process %s(%d) changed process break at address 0x%lx\n",current->comm, current->pid, ret);
	}


	return ret;
}
#endif
