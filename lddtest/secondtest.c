#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void second_test(void)
{
    int fd;
    unsigned int counter;

    fd = open("/dev/second", O_RDONLY);
    if (fd > 0) {
        while (1) {
            read(fd, &counter, sizeof(unsigned int));
            printf("counter: %d\r", counter);
            fflush(stdout);
        }
    } else {
        perror("");
    }
    close(fd);
}
