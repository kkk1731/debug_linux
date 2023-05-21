//include guard
#ifndef MY_LKM_DLINK
#define MY_LKM_DLINK
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm_types.h>

//"my_dlink_func" is defined in include/linux/mm_types.h, line 30~33
extern FUNCPTR_t my_dlink_func;

extern FUNCPTR_t get_func_ptr(void);

extern int dlink_example(char *msg);
extern int dlink_original(char *msg);
#endif
