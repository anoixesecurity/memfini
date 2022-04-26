#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
MODULE_LICENSE("GPL");

static struct node{
char data;
struct node* link;
};
struct kmem_cache {
	unsigned int object_size;/* The original size of the object */
	unsigned int size;	/* The aligned/padded/added on size  */
	unsigned int align;	/* Alignment as calculated */
	slab_flags_t flags;	/* Active flags on the slab */
	unsigned int useroffset;/* Usercopy region offset */
	unsigned int usersize;	/* Usercopy region size */
	const char *name;	/* Slab name for sysfs */
	int refcount;		/* Use counter */
	void (*ctor)(void *);	/* Called on object slot creation */
	struct list_head list;	/* List of all slab caches on the system */
};

struct kmem_cache *drivermain_cache;
static struct node* start;
static struct node* cur;
static int my_init(void)
{
	pr_info("original size of node is %d \n", sizeof(struct node));
    drivermain_cache = kmem_cache_create("DriverMain_CACHE",sizeof(struct node),0, SLAB_HWCACHE_ALIGN ,NULL);
 if(IS_ERR(drivermain_cache)) {
     pr_alert("Error creating cache: %ld\n",PTR_ERR(drivermain_cache));
     return -ENOMEM;
 }

     start = kmem_cache_alloc(drivermain_cache,GFP_KERNEL);
	 pr_info("Size of mem: %ld and ", drivermain_cache->size);
     if(IS_ERR(start)) {
         pr_alert("Error allocating from cache: %ld\n", PTR_ERR(start));
         kmem_cache_destroy(drivermain_cache);
         return -ENOMEM;
     }


	printk(KERN_INFO "allocation done with flag tuntun\n");

    return 0;

}

static void my_exit(void)
{
    printk(KERN_INFO "Goodbye world.\n");
    kmem_cache_destroy (drivermain_cache);

    return;
}

module_init(my_init);
module_exit(my_exit);
