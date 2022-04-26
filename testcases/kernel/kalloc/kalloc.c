#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
static int my_init(void)
{
    int i;
	char *ptr;
	ptr = kmalloc( 9000, GFP_HIGHUSER | GFP_ATOMIC);
    if (!ptr) {
		pr_err("Allocation failed\n");
		return;
	}
    // value 0x100ee2
	printk(KERN_INFO "allocation done with flag %lx\n", GFP_HIGHUSER | GFP_ATOMIC);
    kfree(ptr);
    return 0;

}

static void my_exit(void)
{
    printk(KERN_INFO "Goodbye world.\n");

    return;
}

module_init(my_init);
module_exit(my_exit);
