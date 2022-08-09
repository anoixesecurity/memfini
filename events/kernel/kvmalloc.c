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

MODULE_DESCRIPTION("kvmalloc hook module");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");

#define KEDR_NUM_FRAMES_INTERNAL 10

#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif

static asmlinkage void *(*real_kvmalloc)(size_t size, gfp_t flags, int node);

static asmlinkage void *fh_kvmalloc(size_t size, gfp_t flags, int node)
{
	long ret;
	unsigned int nr;
	unsigned long stack_entries[KEDR_NUM_FRAMES_INTERNAL];
	//struct task_struct *task;
	//task = current;
	nr = stack_trace_save(
		&stack_entries[0],
		KEDR_NUM_FRAMES_INTERNAL, /* as many entries as we can get */
		7 /* we need all stack entries here */);
	//dump_stack();
	pr_info(",Kernel Memory allocation performed for size %ld and flags 0x%lx. Caller details: %pS", size, flags, stack_entries[15]);
	ret = real_kvmalloc(size, flags, node);

	return ret;
}
