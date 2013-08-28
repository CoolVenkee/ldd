#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>

#include <asm/uaccess.h>

#include "ldd.h"
#include "secondcdev.h"

static int second_major = SECOND_MAJOR;
static struct second_dev *second_devp; // 设备结构体指针

// 定时器处理函数
static void second_timer_handler(unsigned long data)
{
    mod_timer(&second_devp->s_timer, jiffies + HZ);
    atomic_inc(&second_devp->counter);
    DBGINFO("current jiffies is %lu", jiffies);
}

// 文件打开函数
static int second_open(struct inode *inodep, struct file *filp)
{
    struct second_dev *devp;

    filp->private_data = container_of(inodep->i_cdev, struct second_dev, cdev);
    devp = filp->private_data;
    // 初始化定时器
    init_timer(&devp->s_timer);
    devp->s_timer.function = second_timer_handler;
    devp->s_timer.expires = jiffies + HZ;
    add_timer(&devp->s_timer); // 添加（注册）定时器
    atomic_set(&devp->counter, 0);
    return 0;
}

// 文件释放函数
static int second_release(struct inode *inodep, struct file *filp)
{
    struct second_dev *devp = filp->private_data;

    del_timer(&devp->s_timer);
    return 0;
}

// 读函数
static ssize_t second_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    int counter;
    struct second_dev *devp = filp->private_data;

    counter = atomic_read(&devp->counter);
    if (put_user(counter, (int *)buf))
        return -EFAULT;
    else
        return sizeof(unsigned int);
}

static struct file_operations second_fops = {
    .owner = THIS_MODULE,
    .open = second_open,
    .release = second_release,
    .read = second_read
};

static int second_setup_cdev(struct second_dev *devp, int index)
{
    dev_t devno = MKDEV(second_major, index);

    devp->cdev.owner = THIS_MODULE;
    cdev_init(&devp->cdev, &second_fops);
    return cdev_add(&devp->cdev, devno, 1);
}

// 设备驱动模块加载函数
int second_init(void)
{
    int ret;
    dev_t devno = MKDEV(SECOND_MAJOR, 0);

    if (second_major)
        ret = register_chrdev_region(devno, 1, SECOND_NAME);
    else {
        ret = alloc_chrdev_region(&devno, 0, 1, SECOND_NAME);
        second_major = MAJOR(devno);
    }
    if (ret < 0)
        goto fail_alloc_chrdev;
    second_devp = kmalloc(sizeof(struct second_dev), GFP_KERNEL);
    if (second_devp == NULL) {
        ret = -ENOMEM;
        goto fail_malloc_dev;
    }
    memset(second_devp, 0, sizeof(struct second_dev));
    ret = second_setup_cdev(second_devp, 0);
    if (ret < 0)
        goto fail_setup_cdev;
    return 0;
fail_setup_cdev:
    kfree(second_devp);
fail_malloc_dev:
    unregister_chrdev_region(devno, 1);
fail_alloc_chrdev:
    return ret;
}

// 模块卸载函数
void second_exit(void)
{
    cdev_del(&second_devp->cdev); // 注销cdev
    kfree(second_devp); // 释放设备结构体内存
    unregister_chrdev_region(MKDEV(second_major, 0), 1); // 释放设备号
}
