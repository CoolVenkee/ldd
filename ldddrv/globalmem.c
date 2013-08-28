#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>

#include <asm/uaccess.h>

#include "ldd.h"
#include "globalmem.h"

static int globalmemMajor = GLOBALMEM_MAJOR; // 初始化主设备号
static struct GlobalmemDev *pGlobalmemDev; // 设备结构体实例指针
//static struct class *globalmemClass;

static int GlobalmemOpen(struct inode *pInode, struct file *filp)
{
    filp->private_data = container_of(pInode->i_cdev, struct GlobalmemDev, cdev);
    return 0;
}

static ssize_t GlobalmemRead(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
    unsigned long p = *ppos;
    struct GlobalmemDev *pDev = filp->private_data;

    //  分析和获取有效的读长度
    if (p > GLOBALMEM_SIZE) // 要读的偏移位置越界
        return 0;
    if (count > GLOBALMEM_SIZE - p)
        count = GLOBALMEM_SIZE - p;
    if (down_interruptible(&pDev->sem))
        return -ERESTART;
    if (copy_to_user(buf, (void *)(pDev->mem + p), count))
        return -EFAULT;
    else
        *ppos += count;
    up(&pDev->sem);
    DBGINFO("Read %d byte(s)", count);
    return count;
}

static ssize_t GlobalmemWrite(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
    unsigned long p = *ppos;
    struct GlobalmemDev *pDev = filp->private_data;

    if (p > GLOBALMEM_SIZE)
        return 0;
    if (count > GLOBALMEM_SIZE - p)
        count = GLOBALMEM_SIZE - p;
    if (down_interruptible(&pDev->sem))
        return -ERESTART;
    if (copy_from_user((void *)(pDev->mem + p), buf, count))
        return -EFAULT;
    else
        *ppos += count;
    up(&pDev->sem);
    DBGINFO("Written %d byte(s)", count);
    return count;
}

static loff_t GlobalmemLlseek(struct file *filp, loff_t offset, int orig)
{
    loff_t ret;

    switch (orig) {
    case SEEK_SET:
        if ((offset < 0) || (offset > GLOBALMEM_SIZE)) {
            ret = -EINVAL;
            break;
        }
        filp->f_pos = offset;
        ret = filp->f_pos;
        break;
    case SEEK_CUR:
        if (((offset + filp->f_pos) < 0) || ((offset + filp->f_pos) > GLOBALMEM_SIZE)) {
            ret = -EINVAL;
            break;
        }
        filp->f_pos += offset;
        ret = filp->f_pos;
        break;
    case SEEK_END:
        if ((offset > 0) || ((offset + GLOBALMEM_SIZE) < 0)) {
            ret = -EINVAL;
            break;
        }
        filp->f_pos = offset + GLOBALMEM_SIZE;
        ret = filp->f_pos;
        break;
    default:
        ret = -EINVAL;
        break;
    }
    return ret;
}

static long GlobalmemUnlockedIoctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct GlobalmemDev *pDev = filp->private_data;

    switch (cmd) {
    case MEM_CLEAR:
        if (down_interruptible(&pDev->sem))
            return -ERESTART;
        memset(pDev->mem, 0, GLOBALMEM_SIZE);
        up(&pDev->sem);
        DBGINFO("globalmem is set to 0");
        break;
    default:
        return -EINVAL;
    }
    return 0;
}

static int GlobalmemRelease(struct inode *pInode, struct file *filp)
{
//    filp->private_data = NULL;
    return 0;
}

static struct file_operations globalmemFileOps = {
    .owner = THIS_MODULE,
    .open = GlobalmemOpen,
    .read = GlobalmemRead,
    .write = GlobalmemWrite,
    .llseek = GlobalmemLlseek,
    .unlocked_ioctl = GlobalmemUnlockedIoctl,
    .release = GlobalmemRelease
};

static void GlobalmemDevSetup(struct GlobalmemDev *pDev, int index)
{
    int err;
    dev_t devno = MKDEV(globalmemMajor, index);

    pDev->cdev.owner = THIS_MODULE;
    cdev_init(&pDev->cdev, &globalmemFileOps);
    err = cdev_add(&pDev->cdev, devno, 1);
    if (err)
        DBGERR("cdev_add error: %d", err);
}

int GlobalmemInit(void)
{
    int result;
    dev_t devno = MKDEV(globalmemMajor, 0); // 1、生成主设备号

    // 2、申请字符设备区域
    if (globalmemMajor) {
        result = register_chrdev_region(devno, 2, GLOBALMEM_NAME);
    } else {
        result = alloc_chrdev_region(&devno, 0, 2, GLOBALMEM_NAME);
        globalmemMajor = MAJOR(devno);
    }
    if (result < 0) {
        DBGERR("register_chrdev_region error: %d", result);
        return result;
    }

    // 动态申请设备结构体的内存
    pGlobalmemDev = kmalloc(2 * sizeof(struct GlobalmemDev), GFP_KERNEL);
    if (!pGlobalmemDev) {
        result = -ENOMEM;
        goto fail_kmalloc;
    }
    memset(pGlobalmemDev, 0, 2 * sizeof(struct GlobalmemDev));
    GlobalmemDevSetup(&pGlobalmemDev[0], 0);
    GlobalmemDevSetup(&pGlobalmemDev[1], 1);
    sema_init(&pGlobalmemDev->sem, 1); // 初始化信号量
//    globalmemClass = class_create(THIS_MODULE, GLOBALMEM_NAME);
//    if (IS_ERR(globalmemClass)) {
//        DBGERR("class_create error");
//        return -EFAULT;
//    }

    return 0;
fail_kmalloc:
    unregister_chrdev_region(devno, 2);
    return result;
}

void GlobalmemExit(void)
{
//    class_destroy(globalmemClass);
    cdev_del(&pGlobalmemDev[0].cdev);
    cdev_del(&pGlobalmemDev[1].cdev);
    kfree(pGlobalmemDev);
    unregister_chrdev_region(MKDEV(globalmemMajor, 0), 2);
}
