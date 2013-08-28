#ifndef LDD_H
#define LDD_H

#include <linux/kernel.h>

#ifdef DEBUG
#define DBGERR(fmt, args...) \
{ \
    do { \
    printk(KERN_ERR "%s[%d]:"fmt"\n", __FUNCTION__, __LINE__, ##args); \
    } while (0); \
}

#define DBGINFO(fmt, args...) \
{ \
    do { \
    printk(KERN_INFO "%s[%d]:"fmt"\n", __FUNCTION__, __LINE__, ##args); \
    } while (0); \
}
#else
#define DBGERR(fmt, args...)
#define DBGINFO(fmt, args...)
#endif

extern int GlobalmemInit(void);
extern void GlobalmemExit(void);

extern int globalfifo_init(void);
extern void globalfifo_exit(void);

extern int second_init(void);
extern void second_exit(void);

extern int globalfifo_platform_init(void);
extern void globalfifo_platform_exit(void);

#endif // LDD_H
