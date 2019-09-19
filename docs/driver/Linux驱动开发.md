
## Linux驱动开发

开发环境：基于韦东山的`JZ2440`  
内核版本：`Linux-2.6`  
参照资料：《[Linux驱动开发入门与实战](https://www.ituring.com.cn/book/1524)》  

### 目录  

[Linux驱动开发概述](#linux驱动开发概述)  
[构建第一个驱动程序](#构建第一个驱动程序)  

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
