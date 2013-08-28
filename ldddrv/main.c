#include <linux/init.h>
#include <linux/module.h>

#include "ldd.h"

static int __init main_init(void)
{
    //GlobalmemInit();
//    return globalfifo_init();
    return second_init();
//    return globalfifo_platform_init();
//    return 0;
}

static void __exit main_exit(void)
{
//    GlobalmemExit();
//    globalfifo_exit();
    second_exit();
//    globalfifo_platform_exit();
}

MODULE_LICENSE("GPL");

module_init(main_init);
module_exit(main_exit);

