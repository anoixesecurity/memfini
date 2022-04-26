#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/stacktrace.h>
#include <linux/printk.h>
#include <linux/sched.h>


#define KEDR_NUM_FRAMES_INTERNAL 3
/*
 * See the changes made to stack trace API in mainline commit
 * e9b98e162aa5 "stacktrace: Provide helpers for common stack trace operations".
 * Kernel 5.2+ has stack_trace_save() already.

#if LINUX_VERSION_CODE <= KERNEL_VERSION(5,2,0)
noinline unsigned int stack_trace_save(unsigned long *store,
				       unsigned int size,
				       unsigned int skipnr)
{
	struct stack_trace trace = {
		.entries = store,
		.max_entries = size,
		.skip = skipnr + 1,
	};

	save_stack_trace(&trace);
	return trace.nr_entries;
}
#endif
*/
static int myinit(void)
{
	unsigned int nr;
	unsigned long stack_entries[KEDR_NUM_FRAMES_INTERNAL];
	pr_info("dump_stack myinit\n");
	//unsigned long stack_entries[3];
	//dump_stack();
	/*
	struct stack_trace trace = {
	    .nr_entries = 0,
	    .entries = &stack_entries[0],

	    .max_entries = 3,

	    // How many "lower entries" to skip.
	    .skip = 0
	};
	save_stack_trace(&trace);
	print_stack_trace(&trace, 2);
	*/
	nr = stack_trace_save(
		&stack_entries[0],
		KEDR_NUM_FRAMES_INTERNAL, /* as many entries as we can get */
		0 /* we need all stack entries here */);
	pr_info("Stack first value is %pS\n", stack_entries[0]);
	pr_info("dump_stack after\n");
	return 0;
}

static void myexit(void)
{
	pr_info("panic myexit\n");
}

module_init(myinit)
module_exit(myexit)
MODULE_LICENSE("GPL");
