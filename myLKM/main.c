#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/pid.h>
#include <linux/pgtable.h>
#include <linux/init.h>
#include "../my_dlink.h"

MODULE_LICENSE("GPL");

//receive arguments
static int func = 0;
module_param(func, int, S_IRUGO);

//original address of dlink func
FUNCPTR_t original;

int dlink_alt(char* msg){
	printk("hook_dlink_original %s", msg);
	return 0;
}

static int __init dlink_test_init(void){
	printk("dlink_test installed");
	//change function pointer
	original = my_dlink_func;
	
	if(func == 1){
		printk("old my_dlink_func=%lx", (unsigned long) my_dlink_func);
		my_dlink_func = &dlink_alt;
		printk("new my_dlink_func=%lx", (unsigned long) my_dlink_func);
	}
	//if(func == 2)...
	return 0;
}

static void __exit dlink_test_exit(void){
	//undo
	my_dlink_func = original;
	printk("re  my_dlink_finc=%lx", (unsigned long) my_dlink_func);
	printk("dlink_test removed");
}

module_init(dlink_test_init);
module_exit(dlink_test_exit);
