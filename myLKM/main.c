#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/pid.h>
#include <linux/pgtable.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/fdtable.h>
#include <asm/io.h>

#include "../my_dlink.h"

MODULE_LICENSE("GPL");

static int func = 0;
module_param(func, int, S_IRUGO);

//__exit()ed or not 
int is_dlinked=0;
FUNCPTR_t original;

static int file_write_log_dlink(struct file *file, char *buf, char *msg){
        loff_t pos = 0;
	int ret;

		memset(buf, '\0', 32);
                strcpy(buf, msg);
                ret = kernel_write(file, buf, 2, &pos);
                printk("write ret=%d", ret);
                return ret;

}

int dlink_alt(char* msg){
        struct file *file;
        char *buf;
	char *filename = "./hsl_log_dlink.txt";
	buf = kmalloc(32, GFP_KERNEL);
	memset(buf, '\0', 32);
	file = filp_open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	if(IS_ERR(file)){
		printk("pre_file open err=%ld", PTR_ERR(file));
	} else {
		file_write_log_dlink(file, buf, "n\n");
		file_write_log_dlink(file, buf, "m\n");
		filp_close(file, NULL);
	}
	kfree(buf);

	printk("hook_dlink_original %s", msg);
	return 0;
}

static int __init dlink_test_init(void){
	if(is_dlinked != 0){
		printk("previous dlink is not removed, remove dlink_test");
		return 0;
	}

	//change function pointer
	original = my_dlink_func;
	
	if(func == 1){
		printk("old my_dlink_func=%lx", (unsigned long) my_dlink_func);
		my_dlink_func = &dlink_alt;
		is_dlinked = 1;
		printk("new my_dlink_func=%lx", (unsigned long) my_dlink_func);
	}
	else{
		printk("not installed dlink func");
	}
	//if(func == 2)...
	return 0;
}

static void __exit dlink_test_exit(void){
	//undo
	is_dlinked = 0;
	my_dlink_func = original;
	printk("re  my_dlink_finc=%lx", (unsigned long) my_dlink_func);
	printk("dlink_test removed");
}

module_init(dlink_test_init);
module_exit(dlink_test_exit);
