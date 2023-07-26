#include "my_debug.h"

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

SYSCALL_DEFINE0(show_vma){
	struct mm_struct *mm;
	struct vm_area_struct *first_vma, *vma;
	int loop_limit = 200;
	int loop;
	printk("show_vma, pid=%d start", current->pid);
	mm = current->mm;
	first_vma = mm->mmap;
	vma = first_vma->vm_next;

	for(loop=0; loop<loop_limit; loop++){
		if(!vma) break;
		if(vma == first_vma) break;
		printk("vm_start=%lx, vm_end=%lx", vma->vm_start, vma->vm_end);
		vma=vma->vm_next;
	}
	return 0;
}

SYSCALL_DEFINE1(is_pmd_huge, unsigned long, addr){
	struct mm_struct *mm;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd = NULL;
	pte_t *pte = NULL;
	int ret;
	//int total;
	struct vm_area_struct *vma;
	//struct page *page;
	printk("is_pmd_huge start");
	mm = current->mm;
	vma = find_vma(mm, addr);
	//pgtable walk start
	pgd = pgd_offset(mm, addr);
	if(!pgd_present(*pgd))
		return MY_DEBUG_NO_PGD;
	p4d = p4d_offset(pgd, addr);
	if(!p4d_present(*p4d))
		return MY_DEBUG_NO_PGD;
	pud = pud_offset(p4d, addr);
	if(!pud_present(*pud))
		return MY_DEBUG_NO_PMD;
	pmd = pmd_offset(pud, addr);
	//walk end
	ret = pmd_trans_huge(*pmd);
	if(ret > 0){
		//page = pmd_page(*pmd);
		printk("pmd is huge,vma=%p,mm=%p",vma,mm);
		//if(!page) return -1;
		//total=total_mapcount(page);
		//printk("page=%lx",(unsigned long)page);
		//printk("page_mapcount=%d, thp_mapcount=%d, total_mapcount=%d, compound=%d", page_mapcount(page),page_trans_huge_mapcount(page, &total), total, compound_mapcount(page));
	}
	else{
		pte = pte_offset_map(pmd, addr);
		//page = pte_page(*pte);
		printk("pmd is regu,vma=%p,mm=%p",vma,mm);
	        //printk("page_mapcount=%d, total_mapcount=%d, compound=%d", page_mapcount(page), total_mapcount(page), compound_mapcount(page));
		pte_unmap(pte);
		}
	return ret;
}

SYSCALL_DEFINE1(dlink_test, char *, msg){
	char buf[256];
	long copied = strncpy_from_user(buf, msg, sizeof(buf));
	int ret;
	if (copied < 0 || copied == sizeof(buf))
		return -EFAULT;
	printk("%s",buf);
        //"my_dlink_func" is defined in include/linux/mm_types.h, line 30~33
        ret = (*my_dlink_func)(buf);
	printk("dlink end");
	return 0;
}

SYSCALL_DEFINE1(out_ulong, unsigned long, addr){
	if(addr == -1){
		printk("task_struct = %lx",(unsigned long)current);
		return 0;
	}
	printk("out_ulong, %ld",addr);
	return 0;
}

//virtual address to physical address
SYSCALL_DEFINE1(v2p, unsigned long, addr)
{
	struct mm_struct *mm;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long ret;

	mm = current->mm;
	//pgtable walk
	pgd = pgd_offset(mm, addr);
	if (!pgd_present(*pgd))
		return MY_DEBUG_NO_PGD;
	p4d = p4d_offset(pgd, addr);
	if (!p4d_present(*p4d))
		return MY_DEBUG_NO_PGD;
	pud = pud_offset(p4d, addr);
	if (!pud_present(*pud))
		return MY_DEBUG_NO_PUD;
	pmd = pmd_offset(pud, addr);
	if (!pmd_present(*pmd))
		return MY_DEBUG_NO_PMD;

	if (pmd_trans_huge(*pmd))
		return pmd_val(*pmd);
	if (!pmd_bad(*pmd)){
		pte = pte_offset_map(pmd, addr);
		if (!pte_none(*pte))
			ret = pte_val(*pte);
		else
			ret = MY_DEBUG_NO_PTE;
		pte_unmap(pte);
	}
	return ret;
}
