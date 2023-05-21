#include "my_dlink.h"
#include <asm/io.h>


int dlink_original(char *msg){
	printk("dlink_original %s", msg);
	return 0;
}

typedef int (*FUNCPTR_t)(char *msg);
FUNCPTR_t get_func_ptr(void){
	return my_dlink_func;
}

FUNCPTR_t my_dlink_func = &dlink_original;
EXPORT_SYMBOL(my_dlink_func);
EXPORT_SYMBOL(get_func_ptr);
