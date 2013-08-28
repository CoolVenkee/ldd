#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void eeprom_test(void)
{
    int fd, ret;
    char buffer[256] = {0};

    fd = open("/sys/class/i2c-adapter/i2c-0/0-0050/eeprom", O_RDWR);
    if (fd < 0) {
        perror("");
        return;
    }
    write(fd, "HelloWorld\n", 11);
    lseek(fd, 0, SEEK_SET);
    ret = read(fd, buffer, 11);
    buffer[ret] = 0;
    printf("%s", buffer);
    close(fd);
}
