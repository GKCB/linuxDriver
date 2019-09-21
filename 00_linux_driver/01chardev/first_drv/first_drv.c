#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/arch/regs-gpio.h>
#include <asm/hardware.h>

//File    :   first_drv.c
//Time    :   2019/09/21
//Author  :   CHENG Bo 
//Version :   1.0
//Github  :   https://github.com/GKCB
//Desc    :   None
//Chip    :   s3c2440
//Module  :   led

int major;
#define DEVICE_NAME     "led"
static struct class *led_drv_class;
static struct class_device	*led_drv_class_dev;

volatile unsigned long *gpfcon = NULL;
volatile unsigned long *gpfdat = NULL;


static int led_drv_open(struct inode *inode, struct file *file)
{
	//set regs
	*gpfcon &= ~((0x3<<(4*2)) | (0x3<<(5*2)) | (0x3<<(6*2)));
	*gpfcon |= ((0x1<<(4*2)) | (0x1<<(5*2)) | (0x1<<(6*2)));
	return 0;
}

ssize_t led_drv_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	return 1;
}

ssize_t led_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	int val;
	copy_from_user(&val, buf, count);
	//set regs
	if (val == 1)
	{
		*gpfdat &= ~((1<<4) | (1<<5) | (1<<6));
	}
	else
	{
		*gpfdat |= (1<<4) | (1<<5) | (1<<6);
	}
	return 0;
}

int led_drv_close(struct inode *inode, struct file *file)
{
	return 0;
}

static struct file_operations led_drv_fops = {
	.owner   =  THIS_MODULE,
	.open    =  led_drv_open,
	.read	 =	led_drv_read,
	.write	 =	led_drv_write,
	.release =  led_drv_close,
};


static int __init s3c2440_led_init(void) 
{                         
	//1. set file_operation
	major = register_chrdev(0, DEVICE_NAME, &led_drv_fops);
	led_drv_class = class_create(THIS_MODULE, DEVICE_NAME);
	led_drv_class_dev = class_device_create(led_drv_class, NULL, MKDEV(major, 0), NULL, "led");
	gpfcon = (volatile unsigned long *)ioremap(0x56000050, 16);
	gpfdat = gpfcon + 1;

	return 0;
}

static void __exit s3c2440_led_exit(void)
{                         
	unregister_chrdev(major, DEVICE_NAME);
	class_device_unregister(led_drv_class_dev);
	class_destroy(led_drv_class);
	iounmap(gpfcon);
		
}                         
module_init(s3c2440_led_init);
module_exit(s3c2440_led_exit);

MODULE_LICENSE("GPL");