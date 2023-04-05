KERNDIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

BUZZER_MODULE=buzzer-module
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
	sudo insmod $(BUZZER_MODULE).ko

$(BUZZER_MODULE)-y := \
	module_buzzer.o \
	buzzer.o \
	../pwm_driver/pwm.o

obj-m = $(BUZZER_MODULE).o
