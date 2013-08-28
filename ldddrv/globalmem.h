#ifndef GLOBALMEM_H
#define GLOBALMEM_H

#include <linux/cdev.h>
#include <linux/semaphore.h>

#define GLOBALMEM_SIZE 0x1000 // 全局内存大小：4KB
#define GLOBALMEM_MAJOR 250 // 预设的globalmem的主设备号
#define GLOBALMEM_NAME "globalmem"
#define MEM_CLEAR 0x01 // 内存清零

struct GlobalmemDev{
    struct cdev cdev; // cdev结构体
    unsigned char mem[GLOBALMEM_SIZE]; // 全局内存
    struct semaphore sem; // 并发控制mem读写的信号量
}; // 自定义设备结构体

#endif // GLOBALMEM_H
