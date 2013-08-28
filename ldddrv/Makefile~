obj-m += ldddrv.o

ldddrv-objs := main.o globalmem.o globalfifo.o secondcdev.o globalfifoplatformdriver.o

ifdef MACH
ifeq ($(MACH),S3C6410)
KERNELDIR=~/dev/workspace/kernel/linux-3.4.56/
ARCH=arm 
CROSS_COMPILE=arm-linux-
endif
else
KERNELDIR=/lib/modules/$(shell uname -r)/build/
ARCH=x86
CROSS_COMPILE=
endif

ifndef BUILDDIR
BUILDDIR=$(PWD)
endif

EXTRA_CFLAGS += -DDEBUG

defualt:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(BUILDDIR) modules 
clean:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(BUILDDIR) clean
