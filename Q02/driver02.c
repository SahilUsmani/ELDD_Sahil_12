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
#include <linux/ioctl.h>

#define MEM_SIZE        50 
 
dev_t dev = 0;
static struct cdev mycdev;
uint8_t *kernel_buffer;

uint32_t buffer = 0;

static int      __init test_driver_init(void);
static void     __exit test_driver_exit(void);
static int      test_open(struct inode *inode, struct file *file);
static int      test_release(struct inode *inode, struct file *file);
static ssize_t  test_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  test_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long	test_ioctl(struct file *file, unsigned int cmd, unsigned long arg);



static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = test_read,
        .write          = test_write,
        .open           = test_open,
        .release        = test_release,
	.unlocked_ioctl	= test_ioctl,
};
 

static int test_open(struct inode *inode, struct file *file)
{
        printk("Device File Open Function Called\n");
        return 0;
}


static int test_release(struct inode *inode, struct file *file)
{
        printk("Device File Release Function Called\n");
        return 0;
}


static ssize_t test_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        
        printk("Driver Read Function Called\n");
        return 0;
}


static ssize_t test_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
	printk("Driver Write Function Called\n");
        return len;
}

static long test_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE:
                        if( copy_from_user(&buffer ,(int32_t*) arg, sizeof(buffer)) )
                        {
                                printk("Data Write : ERROR!\n");
                        }
                        printk("Data = %d\n", value);
                        break;
                case RD_VALUE:
                        if( copy_to_user((int32_t*) arg, &buffer, sizeof(buffer)) )
                        {
                                printk("Data Read : ERROR\n");
                        }
                        break;
                default:
                        printk("Default\n");
                        break;
        }
        return 0;
}


static int __init test_driver_init(void)
{
        /* Allocating Major number */
        if((alloc_chrdev_region(&dev, 0, 2, "my_Dev")) <0)
	{
                printk(KERN_ALERT "Cannot allocate major number\n");
                return -1;
        }
        printk("Major = %d  Minor = %d \n",MAJOR(dev), MINOR(dev));
 
       
        cdev_init(&mycdev, &fops);
 
   
        if((cdev_add(&mycdev, dev, 2)) < 0)
	{
            printk("Cannot add the device to the system\n");
            unregister_chrdev_region(dev, 2);
	    return -1;
        }
        
        return 0;
 
}


static void __exit test_driver_exit(void)
{
        cdev_del(&mycdev);
        unregister_chrdev_region(dev, 2);
        printk(KERN_ALERT "Device Driver Removed Successfully\n");
}
 
module_init(test_driver_init);
module_exit(test_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sahil Usmani");
