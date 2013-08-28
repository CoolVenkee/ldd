#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/platform_device.h>

#include <asm/current.h>
#include <asm/uaccess.h>

#include "ldd.h"
#include "globalfifo.h"

static int globalfifo_major = GLOBALFIFO_MAJOR;
static struct globalfifo_dev *globalfifo_devp;

static int globalfifo_open(struct inode *inodep, struct file *filp)
{
    filp->private_data = container_of(inodep->i_cdev, struct globalfifo_dev, cdev);
    return 0;
}

static ssize_t globalfifo_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    int ret;
    struct globalfifo_dev *devp = filp->private_data; // 获得设备结构体指针
    DECLARE_WAITQUEUE(wait, current); // 定义等待队列

    down(&devp->sem); // 获得信号量
    add_wait_queue(&devp->r_wait, &wait); // 进入读等待队列头
    // 等待FIFO非空
    while (devp->current_len == 0) {
        if (filp->f_flags & O_NONBLOCK) { // 如果为非阻塞立刻返回
            ret = -EAGAIN;
            goto out;
        }
        __set_current_state(TASK_INTERRUPTIBLE); // 改变进程状态为睡眠
        up(&devp->sem);
        schedule(); // 调度其他进程执行
        if (signal_pending(current)) { // 如果是因为信号唤醒
            ret = -ERESTARTSYS;
            goto out2;
        }
        down(&devp->sem);
    }
    // 拷贝到用户空间
    if (count > devp->current_len)
        count = devp->current_len;
    if (copy_to_user(buf, devp->mem, count)) {
        ret = -EFAULT;
        goto out;
    } else {
        // fifo数据前移
        memcpy(devp->mem, devp->mem + count, devp->current_len - count);
        devp->current_len -= count;
        DBGINFO("read %d bytes(s), current_len: %d", count, devp->current_len);
        wake_up_interruptible(&devp->w_wait); // 唤醒写等待队列
        ret = count;
    }
out:
    up(&devp->sem); // 释放信号量
out2:
    remove_wait_queue(&devp->r_wait, &wait);
    set_current_state(TASK_RUNNING);
    return ret;
}

static ssize_t globalfifo_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
    int ret;
    DECLARE_WAITQUEUE(wait, current); // 定义等待队列
    struct globalfifo_dev *devp = filp->private_data; // 获得设备结构体指针

    down(&devp->sem); // 获得信号量
    add_wait_queue(&devp->w_wait, &wait); // 进入写等待队列头
    // 等待FIFO非满
    while (devp->current_len >= GLOBALFIFO_SIZE) {
        // 如果是非阻塞访问
        if (filp->f_flags & O_NONBLOCK) {
            ret = -EAGAIN;
            goto out;
        }
        // 改变进程状态为睡眠
        __set_current_state(TASK_INTERRUPTIBLE);
        up(&devp->sem);
        schedule(); // 调度其他进程执行
        if (signal_pending(current)) {
            // 如果是因为信号唤醒
            ret = -ERESTARTSYS;
            goto out2;
        }
        down(&devp->sem); // 获得信号量
    }
    // 从用户空间拷贝到内核空间
    if (count > GLOBALFIFO_SIZE - devp->current_len)
        count = GLOBALFIFO_SIZE - devp->current_len;
    if (copy_from_user(devp->mem + devp->current_len, buf, count)) {
        ret = -EFAULT;
        goto out;
    } else {
        devp->current_len += count;
        DBGINFO("written %d bytes(s),current_len:%d", count, devp->current_len);
        wake_up_interruptible(&devp->r_wait); // 唤醒读等待队列
        ret = count;
    }
    // 产生异步读信号
    if (devp->async_queue)
        kill_fasync(&devp->async_queue, SIGIO, POLL_IN);
out:
    up(&devp->sem); // 释放信号量
out2:
    remove_wait_queue(&devp->w_wait, &wait);
    set_current_state(TASK_RUNNING);
    return ret;
}

static unsigned int globalfifo_poll(struct file *filp, struct poll_table_struct *poll_tablep)
{
    unsigned int mask = 0;
    struct globalfifo_dev *devp = filp->private_data;

    down(&devp->sem);

    if (devp->current_len > 0)
        mask |= POLLIN | POLLRDNORM;
    else
        poll_wait(filp, &devp->r_wait, poll_tablep);
    if (devp->current_len < GLOBALFIFO_SIZE)
        mask |= POLLOUT | POLLWRNORM;
    else
        poll_wait(filp, &devp->w_wait, poll_tablep);
    up(&devp->sem);
    return mask;
}

static int globalfifo_fasync(int fd, struct file *filp, int mode)
{
    struct globalfifo_dev *devp = filp->private_data;

    return fasync_helper(fd, filp, mode, &devp->async_queue);
}

static int globalfifo_release(struct inode *inodep, struct file *filp)
{
    // 将文件从异步通知列表中删除
    globalfifo_fasync(-1, filp, 0);
    return 0;
}

static struct file_operations globalfifo_fops = {
    .owner = THIS_MODULE,
    .open = globalfifo_open,
    .read = globalfifo_read,
    .write = globalfifo_write,
    .poll = globalfifo_poll,
    .fasync = globalfifo_fasync,
    .release = globalfifo_release
};

static int globalfifo_setup_cdev(struct globalfifo_dev *devp, int index)
{
    int ret;
    dev_t devno = MKDEV(globalfifo_major, 0);

    cdev_init(&devp->cdev, &globalfifo_fops);
    devp->cdev.owner = THIS_MODULE;
    ret = cdev_add(&devp->cdev, devno, 1);
    if (ret < 0)
        DBGERR("cdev_add error: %d", ret);
    return ret;
}

static int globalfifo_probe(struct platform_device *pdev)
{
    int ret = 0;
    dev_t devno = MKDEV(globalfifo_major, 0);

    if (globalfifo_major) {
        ret = register_chrdev_region(devno, 1, GLOBALFIFO_NAME);
    } else {
        ret = alloc_chrdev_region(&devno, 0, 1, GLOBALFIFO_NAME);
        globalfifo_major = MAJOR(devno);
    }
    if (ret < 0) {
        DBGERR("register_chrdev_region error: %d", ret);
        goto fail_alloc_chrdev;
    }
    globalfifo_devp = kmalloc(sizeof(struct globalfifo_dev), GFP_KERNEL);
    if (!globalfifo_devp) {
        ret = -ENOMEM;
        goto fail_kmalloc;
    }
    memset(globalfifo_devp, 0, sizeof(struct globalfifo_dev));
    ret = globalfifo_setup_cdev(globalfifo_devp, 0);
    if (ret < 0)
        goto fail_setup_cdev;
    sema_init(&globalfifo_devp->sem, 1); // 初始化信号量
    init_waitqueue_head(&globalfifo_devp->r_wait); // 初始化读等待队列头
    init_waitqueue_head(&globalfifo_devp->w_wait); // 初始化写等待队列头
    return 0;
fail_setup_cdev:
    kfree(globalfifo_devp);
fail_kmalloc:
    unregister_chrdev_region(devno, 1);
fail_alloc_chrdev:
    return ret;
}

static int globalfifo_remove(struct platform_device *pdev)
{
    cdev_del(&globalfifo_devp->cdev);
    kfree(globalfifo_devp);
    unregister_chrdev_region(MKDEV(globalfifo_major, 0), 1);
    return 0;
}

static struct platform_driver globalfifo_platform_driver = {
    .probe = globalfifo_probe,
    .remove = globalfifo_remove,
    .driver = {
        .name = GLOBALFIFO_NAME,
        .owner = THIS_MODULE
    }
};


int globalfifo_platform_init(void)
{
    return platform_driver_register(&globalfifo_platform_driver);
}

void globalfifo_platform_exit(void)
{
    platform_driver_unregister(&globalfifo_platform_driver);
}
