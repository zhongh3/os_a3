#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
//#include <asm/uaccess.h>


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
	.read =		onebyte_read,
	.write =	onebyte_write,
	.open =		onebyte_open,
	.release =	onebyte_release,
};

// struct file_operations onebye_fops = {
// 	read:		onebyte_read,
// 	write:		onebyte_write,
// 	open:		onebyte_open,
// 	release:	onebyte_release
// };

char *onebyte_data = NULL;
ssize_t size_of_file = sizeof(char);  // size of the file

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
	/* To copy from kernel to user space
	unsigned long copy_to_user(void _ _user *to, 
							   const void *from, 
                               unsigned long count);
                               */

	printk(KERN_NOTICE "onebyte_read: read at offset = 0x%x, count = 0x%lx, size_of_file = 0x%lx \n", 
		(int)*f_pos, count, size_of_file);

	// if current reading position is behind the end of the file
    if (*f_pos >= size_of_file){
    	return 0;  //end-of-file is reached
    }

    // if trying to read more than what we have, read only up to the end of the file
    if (*f_pos + count > size_of_file){
    	count = size_of_file - *f_pos;   
    }

	if (copy_to_user(buf, onebyte_data, count)){
		return -EFAULT;  // bad address
	}

	*f_pos += count;

	return count;
}

ssize_t onebyte_write(struct file *filep, const char *buf, size_t count, loff_t *f_pos)
{
	/* To copy from user space to kernel 
	unsigned long copy_from_user(void *to, 
                                 const void _ _user *from, 
                                 unsigned long count);
                               */
	void *p = onebyte_data;

	printk(KERN_NOTICE "onebyte_write: write at offset = 0x%x, count = 0x%lx, size_of_file = 0x%lx \n", 
	(int)*f_pos, count, size_of_file);

    if (count <= 0){
    	printk(KERN_ALERT "write error: Invalid count = %ld. \n", count);
    	return -EPERM; // Operation not permitted
    }

	if (copy_from_user(p, buf, size_of_file)){
		return -EFAULT;  // Bad address
	}

	if (count > size_of_file  - *f_pos){
    	printk(KERN_ALERT "write error: No space  - offset = 0x%x, count = 0x%lx, size_of_file = 0x%lx \n", 
    		(int)*f_pos, count, size_of_file);
    	return -ENOSPC; // No space left on device
    }

	return count; 
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









