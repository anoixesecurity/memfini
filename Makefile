KERNEL_PATH ?= /lib/modules/$(shell uname -r)/build

obj-m += memfini.o

all:
	make EXTRA_CFLAGS=-DKERNEL_MONITOR=0 -C $(KERNEL_PATH) M=$(PWD) modules

clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
