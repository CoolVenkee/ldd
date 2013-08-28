TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/setup-sdhci-gpio.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/setup-sdhci.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/setup-i2c1.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/setup-i2c0.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/setup-fb-24bpp.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/s3c6410.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/s3c6400.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/pm.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/mach-smdk6410.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/mach-smdk6400.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/mach-ncp.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/mach-hmt.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/mach-anw6410.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/irq-pm.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/irq-eint.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/irq.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/gpiolib.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/dma.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/dev-uart.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/dev-spi.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/dev-rtc.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/dev-audio.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/dev-adc.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/cpufreq.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/cpu.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/clock.c \
    ../../kernel/linux-3.4.56/drivers/i2c/i2c-smbus.c \
    ../../kernel/linux-3.4.56/drivers/i2c/i2c-dev.c \
    ../../kernel/linux-3.4.56/drivers/i2c/i2c-core.c \
    ../../kernel/linux-3.4.56/drivers/i2c/i2c-boardinfo.c \
    ../../kernel/linux-3.4.56/drivers/i2c/busses/i2c-s3c2410.c \
    ../../kernel/linux-3.4.56/drivers/i2c/algos/i2c-algo-pcf.c \
    ../../kernel/linux-3.4.56/drivers/i2c/algos/i2c-algo-pca.c \
    ../../kernel/linux-3.4.56/drivers/i2c/algos/i2c-algo-bit.c \
    ../../kernel/linux-3.4.56/drivers/rtc/rtc-ds1307.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/time.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/pwm-clock.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/pwm.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/pm-gpio.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/pm-check.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/pm.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/irq-vic-timer.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/irq-uart.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/init.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/gpiolib.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/gpio-config.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/gpio.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dma.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-usb-hsotg.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-usb.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-uart.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-nand.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-i2c1.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-i2c0.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-hsmmc2.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-hsmmc1.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-hsmmc.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/dev-fb.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/clock-clksrc.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/clock.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/adc.c \
    ../../kernel/linux-3.4.56/drivers/gpio/gpiolib.c \
    main.c \
    ../../kernel/linux-3.4.56/drivers/w1/w1_netlink.c \
    ../../kernel/linux-3.4.56/drivers/w1/w1_io.c \
    ../../kernel/linux-3.4.56/drivers/w1/w1_int.c \
    ../../kernel/linux-3.4.56/drivers/w1/w1_family.c \
    ../../kernel/linux-3.4.56/drivers/w1/w1.c \
    ../../kernel/linux-3.4.56/drivers/w1/slaves/w1_ds2431.c \
    ../../kernel/linux-3.4.56/drivers/w1/slaves/w1_therm.c \
    ../../kernel/linux-3.4.56/drivers/w1/masters/w1-gpio.c \
    globalmem.c \
    globalfifo.c \
    secondcdev.c \
    globalfifoplatformdriver.c \
    ../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/mach-mini6410.c \
    ../../kernel/linux-3.4.56/drivers/input/touchscreen/mini6410_1wire_host.c \
    ../../kernel/linux-3.4.56/drivers/misc/eeprom/at24.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/devs.c \
    ../../kernel/linux-3.4.56/arch/arm/plat-samsung/platformdata.c \
    ../../kernel/linux-3.4.56/drivers/input/touchscreen/ads7846.c

HEADERS += \
    ../../kernel/linux-3.4.56/drivers/i2c/i2c-core.h \
    ../../kernel/linux-3.4.56/drivers/i2c/algos/i2c-algo-pcf.h \
    ../../kernel/linux-3.4.56/drivers/w1/w1_netlink.h \
    ../../kernel/linux-3.4.56/drivers/w1/w1_log.h \
    ../../kernel/linux-3.4.56/drivers/w1/w1_int.h \
    ../../kernel/linux-3.4.56/drivers/w1/w1_family.h \
    ../../kernel/linux-3.4.56/drivers/w1/w1.h \
    ../../kernel/linux-3.4.56/drivers/w1/slaves/w1_ds2760.h \
    globalmem.h \
    ldd.h \
    globalfifo.h \
    secondcdev.h

INCLUDEPATH += \
../../kernel/linux-3.4.56/include \
../../kernel/linux-3.4.56/arch/arm/include \
../../kernel/linux-3.4.56/arch/arm/mach-s3c64xx/include \
../../kernel/linux-3.4.56/arch/arm/plat-samsung/include \

DEFINES += \
__KERNEL__ \
CONFIG_S3C_DEV_FB \
DEBUG \

OTHER_FILES += \
    Makefile

