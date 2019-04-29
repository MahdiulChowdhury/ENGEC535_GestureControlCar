#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */

#include <asm/hardware.h>
#include <asm/arch/pxa-regs.h>
#include <asm/arch/gpio.h>

#include <linux/timer.h> /* struct timer_list */
#include <linux/string.h> /* strcmp() */   
#include <linux/param.h> /* HZ */
#include <linux/interrupt.h>

MODULE_LICENSE("Dual BSD/GPL");

static int mymodule_open(struct inode *inode, struct file *filp);
static int mymodule_release(struct inode *inode, struct file *filp);
static void mymodule_exit(void);
static int mymodule_init(void);
irqreturn_t gpio_irq(int irq, void *dev_id, struct pt_regs *regs);
static int proc_read( char *page, char **start, off_t off, int count, int *eof, void *data);

struct file_operations mymodule_fops = {
	open: mymodule_open,
	release: mymodule_release
};

module_init(mymodule_init);
module_exit(mymodule_exit);

int irq0 = 0;
int stop = 0;
int mymodule_len_read = 2;
static struct proc_dir_entry *proc_entry;

static int mymodule_init(void)
{
	int result;
	int gpio = 101;

	pxa_gpio_mode(gpio);
	irq0 = IRQ_GPIO(gpio);
	if (request_irq(irq0, &gpio_irq, SA_INTERRUPT | SA_TRIGGER_RISING | SA_TRIGGER_FALLING, "mymodule", NULL) != 0 ) {
	  printk ( "irq not acquired \n" );
	  result = -1;
	  goto fail; 
	}
	proc_entry = create_proc_entry( "mymodule", 0644, NULL );

	if (proc_entry == NULL) {
      		printk(KERN_INFO "mymodule: Couldn't create proc entry\n");
		result = -ENOMEM;
		goto fail; 
	}
	proc_entry->read_proc = proc_read;

	printk(KERN_ALERT "Inserting mymodule module\n"); 
	return 0;

fail: 
	mymodule_exit(); 
	return result;
}

static void mymodule_exit(void)
{
	free_irq(irq0, NULL);
	remove_proc_entry("mytimer", &proc_root);
	printk(KERN_ALERT "Removing mymodule module\n");
}

static int mymodule_open(struct inode *inode, struct file *filp)
{
	/*printk(KERN_INFO "open called: process id %d, command %s\n",
		current->pid, current->comm);*/
	/* Success */
	return 0;
}

static int mymodule_release(struct inode *inode, struct file *filp)
{
	/*printk(KERN_INFO "release called: process id %d, command %s\n",
		current->pid, current->comm);*/
	/* Success */
	return 0;
}

static int proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;
	if (off > 0) {
    		*eof = 1;
    		return 0;
  	}
	len = sprintf(page, "%d\n", stop);
	
	
	return len;
} 



irqreturn_t gpio_irq(int irq, void *dev_id, struct pt_regs *regs)
{
	if(irq == irq0)
	{
		if(pxa_gpio_get_value(101) > 0)
		{
			stop = 1;
		}
		else
		{
			stop = 0;
		}
	}

	printk(KERN_INFO "stop = %d\n", stop);
	return IRQ_HANDLED;

}

