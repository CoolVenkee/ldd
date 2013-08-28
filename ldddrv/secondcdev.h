#ifndef SECONDCDEV_H
#define SECONDCDEV_H

#include <linux/cdev.h>
#include <asm/atomic.h>
#include <linux/timer.h>

#define SECOND_MAJOR 248 // 预设的second的主设备号
#define SECOND_NAME "second"

struct second_dev {
    struct cdev cdev; // cdev结构体
    atomic_t counter; // 一共经历了多少秒
    struct timer_list s_timer; // 设备要使用的定时器
};

#endif // SECONDCDEV_H
