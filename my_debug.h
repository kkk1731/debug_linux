//include guard
#ifndef MY_SYSCALL_DEBUG
#define MY_SYSCALL_DEBUG

#include <linux/sched.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include <linux/pgtable.h>
#include <linux/mm.h>
#include <linux/pfn.h>
#include <linux/pfn_t.h>
#include <linux/pagemap.h>
#include <linux/hugetlb.h>
#include <linux/kernel.h>
#include "my_dlink.h"


#define MY_DEBUG_DEF_NO		0
#define MY_DEBUG_DEF_DO		1
#define MY_DEBUG_DEF_SPLIT	1

enum my_debug_types{
	MY_DEBUG_NO	= 0x000,
	MY_DEBUG_DO	= 0X001,
	MY_DEBUG_SPLIT	= 0X002,
};

enum my_debug_results{
	MY_DEBUG_NO_PGD = 0x001,
	MY_DEBUG_NO_PUD,
	MY_DEBUG_NO_PMD,
	MY_DEBUG_NO_PTE,
};

struct task_struct *search_task_struct_by_pid(int pid)
{
	struct task_struct *tsk	= NULL;
	struct pid *pid_s	= NULL;

	pid_s = find_get_pid(pid);
	if (!pid_s)
		return NULL;

	tsk = get_pid_task(pid_s, PIDTYPE_PID);
	if (!tsk)
		return NULL;

	return tsk;
}

#endif
#define MY_DEBUG_addr(debug_types)  
