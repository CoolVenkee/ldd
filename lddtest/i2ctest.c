#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

void i2c_test(void)
{
    int fd, ret;
    char buffer[256] = {0};

    fd = open("/dev/i2c-0", O_RDWR);
    if (fd < 0) {
        perror("open i2c-dev error");
        exit(1);
    }

    if (ioctl(fd, I2C_SLAVE, 0x50) < 0) {
        perror("set addr error");
        exit(1);
    }
    ret = read(fd, buffer, 32);
    if (ret >= 0) {
        buffer[ret] = 0;
        printf("%s\n", buffer);
    }
    close(fd);
}
