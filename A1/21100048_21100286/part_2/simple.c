#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday {
	int day;
	int month;
	int year;
	struct list_head list;
	};

static LIST_HEAD(birthday_list);

/* This function is called when the module is loaded. */
int simple_init(void)
{

	struct birthday *person1,*person2,*person3,*person4,*person5;
	struct birthday *ptr1;
	struct birthday *next1;
	

	person1 = kmalloc(sizeof(*person1),GFP_KERNEL);
	person1->day = 2;
	person1->month = 8;
	person1->year = 1995;
	INIT_LIST_HEAD(&person1->list);

	person2 = kmalloc(sizeof(*person2),GFP_KERNEL);
	person2->day = 21;
	person2->month = 10;
	person2->year = 1998;
	INIT_LIST_HEAD(&person2->list);
	
	person3 = kmalloc(sizeof(*person3),GFP_KERNEL);
	person3->day = 12;
	person3->month = 2;
	person3->year = 2002;
	INIT_LIST_HEAD(&person3->list);

	person4 = kmalloc(sizeof(*person4),GFP_KERNEL);
	person4->day = 11;
	person4->month = 4;
	person4->year = 1994;
	INIT_LIST_HEAD(&person4->list);

	person5 = kmalloc(sizeof(*person5),GFP_KERNEL);
	person5->day = 6;
	person5->month = 4;
	person5->year = 1998;
	INIT_LIST_HEAD(&person5->list);
	

	list_add_tail(&person1->list,&birthday_list);
	list_add_tail(&person2->list,&birthday_list);
	list_add_tail(&person3->list,&birthday_list);
	list_add_tail(&person4->list,&birthday_list);
	list_add_tail(&person5->list,&birthday_list);

	// Traversal 

	printk(KERN_INFO "Date of Birth\n");
	printk(KERN_INFO "Day	Month	Year"); 

	list_for_each_entry_safe(ptr1,next1,&birthday_list, list)
	{
		printk(KERN_INFO "%d	%d	%d\n", ptr1->day, ptr1->month, ptr1->year);
	}

	
	
       printk(KERN_INFO "Loading Module\n");
	
       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");

// Deletion

struct birthday *next2;
struct birthday *ptr2;		
		
list_for_each_entry_safe(ptr2, next2, &birthday_list, list)
	{
		list_del(&ptr2->list);
		kfree(ptr2);
	}

printk(KERN_INFO "Thank you for your time!\n");	

}



/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");












