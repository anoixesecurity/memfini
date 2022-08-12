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

MODULE_DESCRIPTION("Main module for memfini interface");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");


#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif

#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif



void error_shm(int ret)
{
	switch (ret) {
		    case EINVAL:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: SIZE IS GREATER OR LESS THAN LIMIT.\n",current->comm, current->pid);
		    	break;
		    case ENFILE:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: TOTAL OPEN FILE LIMIT EXCEEDED.\n", current->comm, current->pid);
		    	break;
		    case ENOMEM:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: NO MEMORY CAN BE ALLOCATED.\n",current->comm, current->pid);
		    	break;
		    case EPERM:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: INSUFFICIENT PRIVILEGES TO ACEESS MEMORY.\n",current->comm, current->pid);
		    	break;
		    case ENOENT:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: NO SEGMENT EXIST FOR GIVEN KEY.\n",current->comm, current->pid);
		    	break;
			case EACCES:
				pr_info(",Memory allocation Error in process %s(%d): ERROR: USER DOES NOT HAVE PERMISSION TO ACCESS THE MEMORY.\n",current->comm, current->pid);
		    	break;
			case EAGAIN:
  				pr_info(",Memory allocation Error in process %s(%d): ERROR: FILE HAS BEEN LOCKED.\n",current->comm, current->pid);
  		    	break;
			case ENOSPC:
  				pr_info(",Memory allocation Error in process %s(%d): ERROR: ALL MEMORY IDS ARE TAKEN.\n",current->comm, current->pid);
  		    	break;
			case EEXIST:
  				pr_info(",Memory allocation Error in process %s(%d): ERROR: IPC_CREAT AND IPC_EXCL WERE SPECIFIED BUT MEMORY ALREADY EXIST.\n",current->comm, current->pid);
  		    	break;
		    default:
		      pr_info(",Memory allocation Error in process %s(%d): ERROR: UNKNOWN..\n",current->comm, current->pid);
		}

}


void error_shmat(int ret)
{
	switch (ret) {
		    case EINVAL:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: INVALID SHMID OR CAN'T ATTACH.\n",current->comm, current->pid);
		    	break;
		    case ENOMEM:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: NO MEMORY CAN BE ALLOCATED FOR DISCRIPTOR.\n",current->comm, current->pid);
		    	break;
		    case EIDRM:
		    	pr_info(",Memory allocation Error in process %s(%d): ERROR: SHMID POINTS TO REMOVED ID.\n",current->comm, current->pid);
		    	break;
			case EACCES:
				pr_info(",Memory allocation Error in process %s(%d): ERROR: PROCESS DOES NOT HAVE PERMISSION TO ACCESS THE MEMORY.\n",current->comm, current->pid);
		    	break;
		    default:
		      pr_info(",Memory allocation Error in process %s(%d): ERROR: UNKNOWN..\n",current->comm, current->pid);
		}

}


#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_shmget)(struct pt_regs *regs);

static asmlinkage long fh_sys_shmget(struct pt_regs *regs)
{
	long ret;
	ret = real_sys_shmget(regs);
	if (ret < 0) {
		error_shm(abs(ret));
		return ret;
    }
    if (regs->dx !=0666)
    {
		pr_info(",Shared memory allocation performed by process %s(%d) of size 0x%lx and memory id(shmid) is %ld\n",current->comm, current->pid, regs->si, ret);
	}
	return ret;
}
#else
static asmlinkage long (*real_sys_shmget)(key_t key, size_t size, int flag);

static asmlinkage long fh_sys_shmget(key_t key, size_t size, int flag)
{
	long ret;
	//struct task_struct *task;
	//task = current;

	ret = real_sys_shmget(key, size, flag);
	if (ret < 0) {
		error_shm(abs(ret));
		return ret;
    }

    if (shmflg !=0666)
		pr_info(",Shared memory allocation performed by process %s(%d) of size 0x%lx and memory id is %ld\n",current->comm, current->pid, size, ret);


	return ret;
}
#endif


#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*real_sys_shmat)(struct pt_regs *regs);

static asmlinkage long fh_sys_shmat(struct pt_regs *regs)
{
	long ret;
	ret = real_sys_shmat(regs);
	if (ret < 0) {
		error_shmat(abs(ret));
		return ret;
    }

	pr_info(",Process %s(%d) attached to Shared memory of shmid %ld at address %lx\n",current->comm, current->pid, regs->di, ret);

	return ret;
}
#else
static asmlinkage long (*real_sys_shmat)(int shmid, char __user *shmaddr, int shmflg);

static asmlinkage long fh_sys_shmat(int shmid, char __user *shmaddr, int shmflg)
{
	long ret;
	//struct task_struct *task;
	//task = current;

	ret = real_sys_shmat(shmid, shmaddr, shmflg);
	if (ret < 0) {
		error_shmat(abs(ret));
		return ret;
    }

	pr_info(",Process %s(%d) attached to Shared memory of shmid %d at address %lx\n",current->comm, current->pid, shmid, ret);


	return ret;
}
#endif
