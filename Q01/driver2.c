#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>          
#include <linux/uaccess.h>       
#include <linux/err.h>
#include <linux/semaphore.h>
#include <linux/wait.h>

#define mem_size        50
 
dev_t dev = 0;
static struct cdev mycdev;
uint8_t *kernel_buffer;
static struct semaphore sem;
DECLARE_WAIT_QUEUE_HEAD(wq);

static int      __init test_driver_init(void);
static void     __exit test_driver_exit(void);
static int      test_open(struct inode *inode, struct file *file);
static int      test_release(struct inode *inode, struct file *file);
static ssize_t  test_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  test_write(struct file *filp, const char *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = test_read,
        .write          = test_write,
        .open           = test_open,
        .release        = test_release,
};
 

static int test_open(struct inode *inode, struct file *file)
{
	sema_init(&sem, 1);
	wait_q_flag = 0;
        printk("Device File Open Function Called\n");
        return 0;
}


static int test_release(struct inode *inode, struct file *file)
{
	up(&sem);
	wait_q_flag = 7;
        printk("Device File Release Function Called\n");
        return 0;
}

static ssize_t test_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        
        if( copy_to_user(buf, kernel_buffer, MEM_SIZE) )
        {
                printk("Data Read : ERROR\n");
        }
        printk("Data Read Successful\n");
        return MEM_SIZE;
}


static ssize_t test_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        down(&sem);
        if( copy_from_user(kernel_buffer, buf, len) )
        {
                printk("Data Write : ERROR\n");
        }
        printk("Data Write Successfull\n");

	wait_event_interruptible(wq, wait_q_flag == 7);
        return len;
}

static int __init test_driver_init(void)
{
       
        if((alloc_chrdev_region(&dev, 0, 2, "test_Dev")) <0){
                pr_info("Cannot allocate major number\n");
                return -1;
        }
        printk(KERN_ALERT "Major = %d 	Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        
        cdev_init(&test_cdev,&fops);
 

        if((cdev_add(&test_cdev, dev, 1)) < 0)
	{
            printk("Cannot add the device to the system\n");
        }
        
       
        if((kernel_buffer = kmalloc(MEM_SIZE , GFP_KERNEL)) == 0)
	{
            printk("Cannot allocate memory in kernel\n");
	    return -1;
            
        }
        
        printk(KERN_ALERT "Device Driver Inserted Successfully\n");
        return 0;

}

static void __exit test_driver_exit(void)
{
	kfree(kernel_buffer);
        cdev_del(&mycdev);
        unregister_chrdev_region(dev, 2);
        printk(KERN_ALERT "Device Driver Removed Successfully\n");
}
 
module_init(test_driver_init);
module_exit(test_driver_exit);      
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");

/*
 * This is a static method of creating a device file. You will need mknod utility to create device file
 * after insertion of this module.
 *
 * step 1 - Insert the Module using " sudo insmod driver.ko  "
 * step 2 - Check the Major and Minor number using " sudo dmesg  "
 * step 3 - After you get Major and Minor number use " sudo mknod -m 666 /dev/driver_device1 c <MAJOR> <MINOR> "
 * step 4 - Make device two using " sudo mknod -m 666 /dev/driver_device2 c <MAJOR> <MINOR>  "
 * */
