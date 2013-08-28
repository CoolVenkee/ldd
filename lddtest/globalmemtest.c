#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void GlobalmemTest(void)
{
    int fd;
    char buffer[BUFSIZ];

    fd = open("/dev/globalmem", O_RDWR);
    if (fd > 0) {
        write(fd, "HELLO", 5);
        lseek(fd, 0, SEEK_SET);
        buffer[read(fd, buffer, BUFSIZ)] = 0;
        printf("%s\n", buffer);

        lseek(fd, 0, SEEK_SET);
        write(fd, "HELLO", 5);
        lseek(fd, -4, SEEK_CUR);
        buffer[read(fd, buffer, BUFSIZ)] = 0;
        printf("%s\n", buffer);

        lseek(fd, 0, SEEK_SET);
        write(fd, "HELLO", 5);
        lseek(fd, -4092, SEEK_END);
        buffer[read(fd, buffer, BUFSIZ)] = 0;
        printf("%s\n", buffer);

        close(fd);
    }
}
