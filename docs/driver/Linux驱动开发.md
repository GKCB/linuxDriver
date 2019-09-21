
## Linux驱动开发

开发环境：基于韦东山的`JZ2440`  
内核版本：`Linux-2.6`  
参照资料：《[Linux驱动开发入门与实战](https://www.ituring.com.cn/book/1524)》  

### 目录  
[Linux驱动开发概述](#linux驱动开发概述)  
[构建第一个驱动程序](#构建第一个驱动程序)  
[简单的字符设备驱动程序](#简单的字符设备驱动程序)  
[字符设备驱动架构](#字符设备驱动架构)  
[设备驱动中的并发控制](#设备驱动中的并发控制)  
[设备驱动中的阻塞和同步机制](#设备驱动中的阻塞和同步机制)  

### Linux驱动开发概述  

设备驱动概述：是一个允许计算机软件与硬件交互的程序。  

设备驱动的分类  
1.字符设备    
2.块设备  
3.网络设备  

1.字符设备  
字符设备可以一个个字节读取数据的设备，LED 键盘 和鼠标。字符设备的驱动层要有open、close、read、write、ioctl函数。文件系统中有相关函数会调用这些函数。内核中对字符设备有对应的文件：`/dev/console`。字符设备不一定支持寻址。寻址：对硬件中的寄存器进行随机访问。不支持以为这使能对硬件寄存器进行顺序读取。读取了寄存器的数据之后，程序自己确定用那些数据。  

2.块设备  
类似磁盘，但是一次是一个或者多个块进行操作的。  

3.网络设备  
操作网络设备。  

用户控件只有应用程序和系统调用两部分。只有系统调用才能访问硬件设备。  

模块机制  
模块是运行时加入内核的代码。类似Windows中的DLL文件。  
按照装载的方式分为两种：如果是内核启动时装载就是`静态装载`，内核已经运行时称为`动态装载`。  

驱动模块基本架构：  
```c{.line-numbers}
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

int __init xxx_init(void)
{
    //模块加载时的初始化工作
    return 0;
}

void __exit xxx_exit(void)
{
    //模块卸载时销毁工作    
}
module_init(xxx_init);  //指定初始化函数的宏
module_exit(xxx_exit);  //指定卸载函数的宏
```

需要的技能点：  
1.C语言基础  
2.可以理解芯片手册上面的设备配置  
3.内核源码的理解  
4.多任务设计  

应用程序VS驱动程序：  
内核、驱动程序不能使用C库，C库是在用户控件实现。  
驱动开发要用的是GNU C  
内核支持[异步中断](https://blog.csdn.net/jasonLee_lijiaqi/article/details/80181814)  

返回: [目录](#目录)  

### 构建第一个驱动程序  

[驱动模块要素](#驱动模块要素)  
[编写HelloWorld模块](#编写helloworld模块)  
[编译HelloWorld模块](#编译helloworld模块)  
[添加到内核](#添加到内核)  

编写驱动之前，需要先处理内核代码：  
输入命令：`tar -xjvf linux2.6.xxx.tar.bz2`  
配置内核：`make menconfig`  
编译内核：`make`  

编写第一个驱动模块HelloWorld  
这个HelloWorld本身不会对硬件有任何的操作，只是在加载这个模块的时候console输出"Hello World"卸载的时候"Bye Hello World"。这个驱动只是需要体现驱动的最基本部分。  

#### 驱动模块要素  
一个最基本的驱动程序最起码需要下面五点要素  
1. 头文件
```c{.line-numbers}
#include <linux/module.h> 
#include <linux/init.h> 
```
`module.h`文件包含了加载模块时需要的符合和函数定义。  
`init.h`文件包含了模块加载函数和模块释放函数的宏定义。  

2. 模块参数  
打算传递什么参数给驱动模块。但是不一定每次都需要。  

3. 模块加载函数    
通过这个加载到系统去，模块的初始化工作。模块怎么注册，需要初始化那些寄存器。    

4. 模块卸载函数  
释放资源  

5. 模块许可声明  
没这个模块用不了，但是声明大致如下。  

```c{.line-numbers}
module_init(xxx_init);    
module_exit(xxx_exit);    

MODULE_LICENSE("GPL");
```

#### 编写HelloWorld模块  

```c{.line-numbers}
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

static int __init hello_init(void) 
{
    printk(KERN_ALERT "Hello World\n");
    return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_ALERT "Bye Hello World\n"); 
}                         
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
```
#### 编译HelloWorld模块  

1. 编译内核这里是linux-2.6版本的内核  
2. 编写驱动模块自己的Makefile  

```shell{.line-numbers}
KERN_DIR = /work/system/linux-2.6.22.6

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= hello.o
```
Line1 :`KERN_DIR`这里的是内核的位置。  
line10:`obj-m	+= hello.o`是表示hello.o会被编译成hello.ko模块。`ko`模块才是真的驱动模块。但是要注意既然是`hello.o`,那么对应的文件就该是`hello.c`.  

3. 编译模块  
输入make:  
![编译模块](./image/chapter05/编译模块.png)  

编译完了，需要移动到对应的根文件系统目录。并且做个新的文件系统。  

![运行效果](./image/chapter05/运行效果.png)   

#### 添加到内核  
使用insmod添加驱动模块的这叫做动态添加模块。如果需要随系统一起启动这个模块需要静态添加这个模块。  
1. 编写驱动文件  
2. 驱动文件放到Linux内核目录下，可能需要新建一个目录  
在linux-x.x/driver/目录下创建新的目录，所有的驱动代码都放到这个新目录下面。但是别忘了写一个新的`Kconfig`和`Makefile`文件。  

3. 在Kconfig文件中添加自定义驱动对应的项目编译选择  
4. Makefile文件中添加新驱动的编译语句  

***未完待续***

返回: [目录](#目录)  

### 简单的字符设备驱动程序  
  
[字符设备和块设备](#字符设备和块设备)  
[cdev结构](#cdev结构)  
[字符设备驱动组成](#字符设备驱动组成)   

#### 字符设备和块设备  
1. 字符设备  
只能一个字节的读取，不能随机读。例如，鼠标、键盘、串口控制台和LED。  
2. 块设备  
按照块进度读取，不一定要按照顺序来。常见：U盘、SD卡  
3. 怎么看，输入命令ls  
如果顶头有`c`字符表示字符设备，`b`表示是块设备。  
```shell
ls -l
```
#### 驱动设备号  

#### 主设备号和次设备号  
主设备号表示这是什么驱动，次设备号表示在主设备号下面是第几个。例如，两个LED。主设备号表示这是LED设备，次设备号表示这是第几个LED设备。  

1.设备号怎么表示  
Linux捏合中用dev_t表示，u_long在32位机是4字节，64位机是8字节。如果是32位机，高位12表示涉笔好，帝20表示次设备号。  

|  主设备号12位   | 次设备号20位  |  
|  ----  | ----  |

dev_t结构  
```c{.line-numbers}
typdef u_long dev_t;
```

2.主/次设备号怎么获得  
考虑到32位机和64位机的不同。首先尽量不要自己写。不同的芯片可能分配的设备号不一致。  
```c{.line-numbers}
#define MINORBITS	20
#define MINORMASK	((1U << MINORBITS) - 1)

#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
//右移20位取头12位
#define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
//低20位获得20位
#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))
//获得主设备和次设备
```

#### 静态分配设备号  
开发者自己选定的设备号，但是要注意可能会有冲突。  

#### 动态分配设备号  
让系统给用户分配设备号。需要有申请和释放设备号这两个操作。    

怎么查看设备号：查看`/proc/devices`  

#### 申请和释放设备号  

可以使用函数申请设备号：`register_chrdev_region`,`register_chrdev`
《[register_chrdev_region()、register_chrdev()、 alloc_chrdev_region()函数的区别](https://blog.csdn.net/welbell_uplooking/article/details/83654312)》  
用完了别忘了释放：`unregister_chrdev`  

返回：[简单的字符设备驱动程序](#简单的字符设备驱动程序)  
返回: [目录](#目录)  

#### cdev结构  
申请设备号之后，需要注册到系统中去。才能生效。  
cdev结构体包含了字符设备的特性：  
```c{.line-numbers}
struct cdev{
    ...
    const struct file_operations *ops;
    ...
    dev_t dev;
}
```
驱动中需要通过file_operations结构体来完成注册。这样可以创建一个设备文件。驱动所有的调用函数都需要通过`file_operations`结构体来完成注册。常见的有：`open`、`read`、 `write`、`close`、`ioctrl`等方法。  

```c{.line-numbers}
//参照fs.h
struct file_operations {
	struct module *owner;
	loff_t (*llseek) (struct file *, loff_t, int);
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
	ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
	int (*readdir) (struct file *, void *, filldir_t);
	unsigned int (*poll) (struct file *, struct poll_table_struct *);
	int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long);
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	int (*mmap) (struct file *, struct vm_area_struct *);
	int (*open) (struct inode *, struct file *);
	int (*flush) (struct file *, fl_owner_t id);
	int (*release) (struct inode *, struct file *);
	int (*fsync) (struct file *, struct dentry *, int datasync);
	int (*aio_fsync) (struct kiocb *, int datasync);
	int (*fasync) (int, struct file *, int);
	int (*lock) (struct file *, int, struct file_lock *);
	ssize_t (*sendfile) (struct file *, loff_t *, size_t, read_actor_t, void *);
	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
	int (*check_flags)(int);
	int (*dir_notify)(struct file *filp, unsigned long arg);
	int (*flock) (struct file *, int, struct file_lock *);
	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
};
```

```c{.line-numbers}
static struct file_operations xxx_drv_fops = {
    .owner   =  THIS_MODULE,    
    /* 这是一个宏，推向编译模块时自动创建的__this_module变量 */
    .open    =  xxx_drv_open,     
    .read    =	xxx_drv_read,	   
    .release =  xxx_drv_close,
    .poll    =  xxx_drv_poll,
    .fasync  =  xxx_drv_fasync,
};
```
`ioctrl` 执行设备命令的方法。  

返回：[简单的字符设备驱动程序](#简单的字符设备驱动程序)  
返回: [目录](#目录)  

#### 字符设备驱动组成  

字符设备的加载和卸载函数，这两部分工作主要在init函数和exit函数中完成。init函数中需要:1.申请设备号 2.完成cdev/file_operation结构体的初始化  

驱动和应用程序的数据交换：首先它们不能直接交换数据，因为他们的地址肯定不一致。需要使用内核提供的专用函数：  
```c{.line-numbers}
copy_to_user
copy_from_user
put_user
get_user
```
返回：[简单的字符设备驱动程序](#简单的字符设备驱动程序)  
返回: [目录](#目录)  


### 字符设备驱动架构  
详细可以参照[myleds.c](../../00_linux_driver/01chardev/leds/myleds.c)经典例子，需要多看。  
[驱动程序部分](#驱动程序部分)  
[应用程序部分](#应用程序部分)  

#### 驱动程序部分  
凡是应用层需要的各类函数xxx_open、xxx_read、xxx_write均在这里进行实现。  
首要是有对应的file_operations的初始化(用户代码段 `driver_function`)： 
```c{.line-numbers}
static struct file_operations xxx_drv_fops = {
	.owner  = THIS_MODULE,
	.open   = xxx_drv_open,
	.read	= xxx_drv_read,     
	.write	= xxx_drv_write,	   
};
```
同样还需要对应的模块加载和协议规范：  
```c{.line-numbers}
/* 这两行指定驱动程序的初始化函数和卸载函数 */
//s3c24xx_leds_init 这是命名习惯
module_init(xxx_xxx_init);
module_exit(xxx_xxx_exit);

MODULE_LICENSE("GPL");
```
1. [驱动函数的注册](#驱动函数的注册)  
2. [驱动函数的退出](#驱动函数的退出)  
3. [驱动模块函数实现](#驱动模块函数实现)  

#### 驱动函数的注册  
xxx_init  
1. 驱动函数入口  
```c{.line-numbers}
//register_chrdev(主设备号, 名字(可以随便写), 结构)
major = register_chrdev(0, "first_drv", &xxx_drv_fops);
//0表示自动分配
register_chrdev(LED_MAJOR, DEVICE_NAME, &s3c24xx_leds_fops);
//这里表示分配了主设备号
```
这个函数被xxx_xxx_init调用。调用注册函数register_chrdev的函数，也就是驱动函数的入口。 

2. 创建一个设备类,设备先要有这个类才能创建  
```c{.line-numbers}
static struct class *xxxdrv_class;
xxxdrv_class = class_create(THIS_MODULE, DEVICE_NAME);
```
在这个类下面创建一个设备    
[004_linux驱动之_class_create创建一个设备类](http://blog.sina.com.cn/s/blog_15992c4180102xrwj.html)  

3. 在设备类下面创建一个设备  
```c{.line-numbers}
static struct class_device *xxxdrv_class_dev;
xxxclass_dev = class_device_create(xxxdrv_class,NULL,MKDEV(major, 0),NULL,"DEVICE_NAME");
```
当我们使用class_create创建一个类之后我们就可以使用class_device_create函数在这个类下面创建一个设备了 
[005_LINUX驱动之_CLASS_DEVICE_CREATE函数](https://www.cnblogs.com/luxiaoguogege/p/9690216.html)  

4. 使用ioremap映射寄存器地址  
```c{.line-numbers}
volatile unsigned long *gpfcon = NULL;
volatile unsigned long *gpfdat = NULL;
gpfcon = (volatile unsigned long *)ioremap(0x56000050, 16);
gpfdat = gpfcon + 1;
```
ioremap在Linux中reg地址不能随便访问。  

#### 驱动函数的退出  

#### 驱动模块函数实现
1. [xxx_drv_open](#xxx_drv_open)  
2. [xxx_drv_read](#xxx_drv_read)  
3. [xxx_drv_write](#xxx_drv_write)  
#### xxx_drv_open:  
```c{.line-numbers}
static int xxx_drv_open(struct inode *inode, struct file *file)
{
	//regs conf
	return 0;
}
```
[linux驱动开发--copy_to_user 、copy_from_user函数实现内核空间数据与用户空间数据的相互访问](https://blog.csdn.net/xiaodingqq/article/details/80150347)  

驱动函数和应用程序之间的数据交换，不能够直接相互访问。因为，他们的地址空间往往是不一样的。  
这就需要内核提供的相互交换数据的函数：  
```c
/*内核空间->用户空间交换数据*/
copy_to_user //xxx_drv_read 中调用
/*用户空间->内核空间交换数据*/
copy_from_user //xxx_drv_write 中调用
```
#### xxx_drv_read:  

```c{.line-numbers}
ssize_t xxx_drv_read(struct file *file, char __user *buf, size_t size, loff_t *ppos)
{
	return 1;
}
```
#### xxx_drv_write:  
应用程序传进来的是`buf`,需要向下面的硬件传递
```c{.line-numbers}
static ssize_t xxx_drv_write(struct file *file, const char __user *buf, size_t count, loff_t * ppos)
{
	int val;
	copy_from_user(&val, buf, count);
	//app:write(fd, &val, 4);
	//set regs 
	return 0;
}
```

返回：[驱动程序部分](#驱动程序部分)  

返回：[字符驱动小结](#字符驱动小结)  

#### 应用程序部分
main函数内调用open、read、write 等函数。具体实现全部参照[驱动程序部分](#驱动程序部分)  

考虑到是在Linux开发板上面运行，需要交叉编译：
```shell{.line-numbers}
arm-linux-gcc -o firstdrvtest firstdrvtest.c
```
但是要注意的是，应用层打开的设备名得和驱动层的名字一致：  
```c{.line-numbers}
//APP 设备名 led
fd = open("/dev/led", O_RDWR);
//对应的驱动设备名字也得是 led
#define DEVICE_NAME     "led"
major = register_chrdev(0, DEVICE_NAME, &led_drv_fops);
led_drv_class = class_create(THIS_MODULE, DEVICE_NAME);
led_drv_class_dev = class_device_create(led_drv_class, NULL, MKDEV(major, 0), NULL, "led");
```

查看添加上去的设备情况：  
```shell{.line-numbers}
ls -l /dev/led
crw-rw----    1 0        0        252,   0 Jan  1 00:00 /dev/led
```
主设备号252，次设备号0  


返回：[字符驱动小结](#字符驱动小结) 
返回: [目录](#目录)  
