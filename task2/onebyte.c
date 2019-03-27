#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define MAJOR_NUMBER 61

/* forward declaration */
int onebyte_open(struct inode *inode, struct file *filep);
int onebyte_release(struct inode *inode, struct file *filep);

ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos);
ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos);

static int onebyte_init(void);
static void onebyte_exit(void);

/* file_operation structure */
struct file_operations onebye_fops = {
	read:		onebyte_read,
	write:		onebyte_write,
	open:		onebyte_open,
	release:	onebyte_release
};

char *onebyte_data = NULL;

int onebyte_open(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}

int onebyte_release(struct inode *inode, struct file *filep)
{
	return 0; // always successful
}

ssize_t onebyte_read(struct file *filep, char *buf, size_t count, loff_t *f_pos)
{
	// to be completed
	return 0;
}

ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
	// to be completed
	return 0;
}

static int onebyte_init(void)
{
	int result;

	// register the device
	result = register_chrdev(MAJOR_NUMBER, "onebyte", &onebye_fops);

	if (result < 0){
		return result;
	}

	// allocate one byte of memory for storage
	// kmalloc() -- malloc(); 2nd parameter is memory type
	// kfree() -- free()
	onebyte_data = kmalloc(sizeof(char), GFP_KERNEL);
	if (!onebyte_data){
		onebyte_exit();
		// can't allocate memory, return no memory error (negative signify a failure)
		return -ENOMEM;
	}

	// initialize the value to X
	*onebyte_data = 'X';
	printk(KERN_ALERT "This is a onebyte device module.\n");

	return 0;
}

static void onebyte_exit(void)
{
	// if the pointer is pointing to something
	if (onebyte_data){
		//free memory and assign the pointer to NULL
		kfree(onebyte_data);
		onebyte_data = NULL;
	}

	// unregister the device
	unregister_chrdev(MAJOR_NUMBER, "onebyte");
	printk(KERN_ALERT "Onebyte device module is unloaded.\n");
}

MODULE_LICENSE("GPL");
module_init(onebyte_init);
module_exit(onebyte_exit);









