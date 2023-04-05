KERNDIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

KEYPAD_MODULE=keypad-module
BASE_DIR=/home/bsoh/access_control_system

CFLAGS_KERNEL := -I $(BASE_DIR)
CFLAGS += -D__KERNEL__ -DKERNEL_5_15 $(CFLAGS_KERNEL)
CFLAGS += -I $(PWD) -I $(BASE_DIR)

all:
	make -C $(KERNDIR) M=$(PWD) modules

clean:
	make -C $(KERNDIR) M=$(PWD) clean
	rm -rf *.ko
	rm -rf *.o
install:
	sudo insmod $(KEYPAD_MODULE).ko

$(KEYPAD_MODULE)-y := \
	keyPad.o

obj-m = $(KEYPAD_MODULE).o
