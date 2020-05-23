#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/sched.h>

//int count = 0;

//DFS function that iterates over the linux modules and 
//prints informtation of all the currently running tasks.

void DFS(struct task_struct *task1)
{
	 
	struct task_struct *proc;
	struct list_head *list;

	list_for_each(list,&task1->children)
	{	
		//count++;
		proc = list_entry(list, struct task_struct,sibling);
		printk(KERN_INFO "Process Name: %-20s Process State: %1d\tProcess ID %d\n",proc->comm,proc->state,proc->pid);
		DFS(proc);		
	}
	
}


/* This function is called when the module is loaded. */

int simple_init(void)
{	
	// Explanantion: The below-commented lines have been commented out 
	// because they refer to the iteration of tasks linearly (Part 1). 
	// Please comment out the DFS-related lines to run this. Otherwise
	// it will iterate over the on-going tasks with a DFS Tree. 	
	
	/*struct task_struct *task2;
 	printk(KERN_INFO "Loading Tasks.\n"); 
	printk(KERN_INFO "Start Listing Tasks.\n");      

	for_each_process(task2)
	{
		printk(KERN_INFO "Process Name: %-20s Process State: %1d\tProcess ID %d\n",task2->comm,task2->state,task2->pid);
	}*/
	
	
	printk(KERN_INFO "DFS Tree Of Tasks Is As Follows\n:");
	DFS(&init_task);
	
	
       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	
	printk(KERN_INFO "Removing Task\n");
}



/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Momin's Module");
MODULE_AUTHOR("SGG");












