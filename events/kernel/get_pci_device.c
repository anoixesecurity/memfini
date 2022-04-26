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

MODULE_DESCRIPTION("get_pci_device hook module");
MODULE_AUTHOR("Shubham <shubham0d@protonmail.com>");
MODULE_LICENSE("GPL");

#define KEDR_NUM_FRAMES_INTERNAL 10

#ifndef CONFIG_X86_64
#error Currently only x86_64 architecture is supported
#endif
asmlinkage struct pci_dev *(*real_pci_get_device)(unsigned int vendor, unsigned int device, struct pci_dev *from);

asmlinkage struct pci_dev *fh_pci_get_device(unsigned int vendor,
		unsigned int device, struct pci_dev *from)
{
	struct pci_dev *dev;
	unsigned int nr;
	unsigned long stack_entries[KEDR_NUM_FRAMES_INTERNAL];
	//struct task_struct *task;
	//task = current;
	nr = stack_trace_save(
		&stack_entries[0],
		KEDR_NUM_FRAMES_INTERNAL, /* as many entries as we can get */
		7 /* we need all stack entries here */);
	//dump_stack();

	pr_info("Stack first value is %pS\n", stack_entries[12]);

	dev = real_pci_get_device(vendor, device, from);
    pr_info("real pci device executed\n");
	return dev;
}
