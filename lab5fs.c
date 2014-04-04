#include "lab5fs.h"

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sourav Chakraborty");

static int __init init_lab5fs(void)
{
	printk("Initializing module lab5fs\n");
	return 0;
}

static void __exit exit_lab5fs(void)
{
	printk("Cleaning up module lab5fs\n");
}

module_init(init_lab5fs);
module_exit(exit_lab5fs);
