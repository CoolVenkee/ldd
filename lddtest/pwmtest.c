#include<stdint.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<linux/input.h>
#include<unistd.h>

void pwm_test(void)
{

    int fd, ret;
    struct input_event event;

    if ((fd = open("/dev/input/event0", O_RDWR)) < 0) {
        perror("beep test");
        return;
    }
    event.type = EV_SND;
    event.code = SND_TONE;
    event.value = 0;
    ret = write(fd, &event, sizeof(struct input_event));
    close(fd);
}
