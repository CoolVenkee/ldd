#ifndef GLOBALFIFO_H
#define GLOBALFIFO_H

#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/wait.h>
#include <linux/fs.h>

#define GLOBALFIFO_SIZE 0x1000 // 全局内存大小：4KB
#define GLOBALFIFO_MAJOR 251 // 预设的globalfifo的主设备号
#define GLOBALFIFO_NAME "globalfifo"
#define MEM_CLEAR 0x01 // 内存清零

struct globalfifo_dev {
    struct cdev cdev; // cdev结构体
    unsigned int current_len; // fifo有效数据长度
    unsigned char mem[GLOBALFIFO_SIZE]; // 全局内存
    struct semaphore sem; // 并发控制fifo读写的信号量
    wait_queue_head_t r_wait; // 阻塞读用的等待队列头
    wait_queue_head_t w_wait; // 阻塞写用的等待队列头
    struct fasync_struct *async_queue; // 异步结构体指针，用于读
}; // 自定义设备结构体

#endif // GLOBALFIFO_H
