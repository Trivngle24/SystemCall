#include "process_ancestors.h"
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/uaccess.h>

asmlinkage long sys_process_ancestors(
	struct process_info info_array[], long size, long *num_filled)
{
	struct task_struct *task = current; 
	struct list_head *head = NULL;
	long children_count = 0;
	long sibling_count = 0;
	long num_write = 0;

	long pid;
	char name[ANCESTOR_NAME_LEN];
	long state;
	long uid;
	long nvcsw;
	long nivcsw;
	long num_children;
	long num_siblings;

	if(size <= 0)
		return -EINVAL;

	while(task->parent != task){

		children_count = sibling_count = 0;
		list_for_each(head, &(task->children))
			children_count++;
		list_for_each(head, &(task->sibling))
			sibling_count++;


		if(num_write < size){			
			
			pid = (long) task->pid;			
			strcpy(name, task->comm);			
			state = task->state;				
			uid = (long) (task->cred)->uid.val;			
			nvcsw = task->nvcsw;									
			nivcsw = task->nivcsw;					
			num_children = children_count;	
			num_siblings = sibling_count;

			if(copy_to_user(&(info_array[num_write].pid), &pid, sizeof(long)) 
				|| copy_to_user(&(info_array[num_write].name), &name, sizeof(name))
				|| copy_to_user(&(info_array[num_write].state), &state, sizeof(long))
				|| copy_to_user(&(info_array[num_write].uid), &uid, sizeof(long))
				|| copy_to_user(&(info_array[num_write].nvcsw), &nvcsw, sizeof(long))
				|| copy_to_user(&(info_array[num_write].nivcsw), &nivcsw, sizeof(long))
				|| copy_to_user(&(info_array[num_write].num_children), &num_children, sizeof(long))
				|| copy_to_user(&(info_array[num_write].num_siblings), &num_siblings, sizeof(long))){
					return -EFAULT;
			}	
			
			num_write++;
		}


		task = task->parent;
	}

	if(copy_to_user(num_filled, &num_write, sizeof(long)))
		return -EFAULT;

	return 0;
}