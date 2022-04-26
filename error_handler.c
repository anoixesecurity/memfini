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

MODULE_DESCRIPTION("Error handling code for memory monitor interface");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");


void error(int ret)
{
	switch (ret) {
		    case EINVAL:
		      pr_info("Process %d: ERROR: INVALID ARGUMENTS.\n", current->pid);
		      break;
		    case EFAULT:
		      pr_info("Process %d: ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n", current->pid);
		      break;
		    case ENOMEM:
		      pr_info("Process %d: ERROR: UNABLE TO ALLOCATE MEMORY.\n", current->pid);
		      break;
		    case EPERM:
		      pr_info("Process %d: ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n", current->pid);
		      break;
		    case ESRCH:
		      pr_info("Process %d: ERROR: PROCESS DOES NOT EXIST.\n", current->pid);
		      break;
		    default:
		      pr_info("Process %d: ERROR: AN UNKNOWN ERROR HAS OCCURRED.\n", current->pid);
		}

}
