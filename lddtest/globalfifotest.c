#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <signal.h>

#ifdef BLOCK_TEST
int globalfifo_test(void)
{
    int fd, ret;
    char buffer[BUFSIZ];
    fd_set rfds, wfds;

    fd = open("/dev/globalfifo", O_RDWR | O_NONBLOCK);
    if (fd > 0) {
        // 测试阻塞与非阻塞
        while (1) {
            if ((ret = read(fd, buffer, BUFSIZ)) < 0) {
                perror("");
            } else {
                buffer[ret] = 0;
                printf("%s", buffer);
            }
            sleep(1);
        }
        // 测试poll
//        while (1) {
//            FD_ZERO(&rfds);
//            FD_ZERO(&wfds);
//            FD_SET(fd, &rfds);
//            FD_SET(fd, &wfds);
//            select(fd + 1, &rfds, NULL, NULL, NULL);
//            if (FD_ISSET(fd, &rfds)) {
//                printf("Poll monitor: can be read\n");
//                break;
//            }
//            if (FD_ISSET(fd, &wfds))
//                printf("Poll monitor: cat be written\n");
//        }
    }
    return 0;
}

#endif

#ifdef POLL_TEST
int globalfifo_test(void)
{
    int fd, ret;
    char buffer[BUFSIZ];
    fd_set rfds, wfds;

    fd = open("/dev/globalfifo", O_RDWR | O_NONBLOCK);
    if (fd > 0) {
        // 测试阻塞与非阻塞
        while (1) {
            if ((ret = read(fd, buffer, BUFSIZ)) < 0) {
                perror("");
            } else {
                buffer[ret] = 0;
                printf("%s", buffer);
            }
            sleep(1);
        }
        // 测试poll
//        while (1) {
//            FD_ZERO(&rfds);
//            FD_ZERO(&wfds);
//            FD_SET(fd, &rfds);
//            FD_SET(fd, &wfds);
//            select(fd + 1, &rfds, NULL, NULL, NULL);
//            if (FD_ISSET(fd, &rfds)) {
//                printf("Poll monitor: can be read\n");
//                break;
//            }
//            if (FD_ISSET(fd, &wfds))
//                printf("Poll monitor: cat be written\n");
//        }
    }
    return 0;
}
#endif

#ifdef FASYNC_TEST

static int fd = -1;

static void globalfifo_readable_handler(int signum)
{
    int ret;
    char buffer[BUFSIZ];

    if (signum == SIGIO) {
        ret = read(fd, buffer, BUFSIZ);
        buffer[ret] = 0;
        printf("recv: %s", buffer);
    }
}

int globalfifo_test(void)
{
    int oflags;

    fd = open("/dev/globalfifo", O_RDWR);
    if (fd > 0) {
        signal(SIGIO, globalfifo_readable_handler);
        fcntl(fd, F_SETOWN, getpid());
        oflags = fcntl(fd, F_GETFL);
        fcntl(fd, F_SETFL, oflags | FASYNC);
        while (1)
            sleep(100);
    }
    return 0;
}

#endif
