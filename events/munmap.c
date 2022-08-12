
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
#include <linux/errno.h>
MODULE_DESCRIPTION("MUNMAP module");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");


#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

void error_unmap(int ret)
{
	switch (ret) {
		    case EINVAL:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: INVALID ARGUMENTS (Eg: Size to large or value not Allign on page boundry ).\n",current->comm, current->pid);
		    	break;
		    case EFAULT:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n", current->comm, current->pid);
		    	break;
		    case ENOMEM:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: NO MEMORY AVAILABLE OR MAXIMUM MAPPING LIMIT EXCEEDED.\n",current->comm, current->pid);
		    	break;
		    case EPERM:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n",current->comm, current->pid);
		    	break;
		    case ESRCH:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: PROCESS DOES NOT EXIST.\n",current->comm, current->pid);
		    	break;
			case EACCES:
				pr_info(",Memory allocation Error in process %s(%d): ERROR: FILE DISCRIPTOR INVALID OR REFER TO NON REGULAR FILE.\n",current->comm, current->pid);
		    	break;
			case EAGAIN:
  				pr_info(",Memory allocation Error in process %s(%d): ERROR: FILE HAS BEEN LOCKED.\n",current->comm, current->pid);
  		    	break;
			case EBADF:
  				pr_info(",Memory allocation Error in process %s(%d): ERROR: INVALID FILE DESCRIPTOR.\n",current->comm, current->pid);
  		    	break;
			case EEXIST:
  				pr_info(",Memory allocation Error in process %s(%d): ERROR: ADDRESS CLASH WITH EXISITING MAPPING.\n",current->comm, current->pid);
  		    	break;
			case ENODEV:
    			pr_info(",Memory allocation Error in process %s(%d): ERROR: UNDERLINE FILE SYSTEM DOESN'T SUPPORT MEMORY MAPPING.\n",current->comm, current->pid);
    			break;
			case EOVERFLOW:
	    		pr_info(",Memory allocation Error in process %s(%d): ERROR: LENGTH AND OFFSET OVERFLOW 32 BIT VALUE.\n",current->comm, current->pid);
	    		break;
			case ETXTBSY:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: MAP_DENYWRITE  SET BUT FD IS OPEN FOR WRITING.\n",current->comm, current->pid);
		    	break;
			//case SIGSEGV:
		    	//pr_info(",Memory allocation Error in process %s(%d): ERROR: ATTEMPT TO WRITE TO MEMORY THAT IS READ ONLY.\n",current->comm, current->pid);
		    	//break;
			case SIGBUS:
			    pr_info(",Memory allocation Error in process %s(%d): ERROR: ATTEMPT TO WRITE TO MEMORY THAT IS READ ONLY.\n",current->comm, current->pid);
			    break;
		    default:
		      pr_info(",Memory allocation Error in process %s(%d): ERROR: UNKNOWN..\n",current->comm, current->pid);
		}

}


#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_munmap)(struct pt_regs *regs);

static asmlinkage long fh_sys_munmap(struct pt_regs *regs)
{
	long ret;
	ret = real_sys_munmap(regs);
	if (ret < 0) {
		error_unmap(abs(ret));
		return ret;
    }
	//if (strncmp(current->comm, "demo1", 5) == 0) {
		//pr_info(" mem_alloc, Process: %s(%d), address: %lx, size: %ld, Memory permission: %ld, flag: %lx, fd: %ld\n",current->comm, current->pid, ret, regs->si, regs->dx, regs->cx, regs->r8);
	pr_info(",Process %s(%d) performed memory unmapping of address 0x%lx of size %ld\n", current->comm, current->pid, regs->di,regs->si);
	//}
	return ret;
}
#else
static asmlinkage unsigned long (*real_sys_munmap)(unsigned long addr, size_t len);

static asmlinkage unsigned long real_sys_munmap(unsigned long addr, size_t len)
{
	long ret;
	//struct task_struct *task;
	//task = current;

	ret = real_sys_munmap(addr, len);
	if (ret < 0) {
		error_unmap(abs(ret));
		return ret;
    }
	//pr_info(" mem_alloc, Process: %s(%d), address: %lx, size: %ld, Memory permission: %ld, flag: %lx, fd: %ld\n",current->comm, current->pid, ret, regs->si, regs->dx, regs->cx, regs->r8);
	pr_info(",Process %s(%d) performed memory unmapping of address 0x%lx and size %ld\n", current->comm, current->pid, addr, len);
	return ret;
}
#endif
