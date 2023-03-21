KERNDIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)


CFLAGS_KERNEL := -I /home/bsoh/toy_project_smart_home/lib/ioctl
CFLAGS += -D__KERNEL__ -DKERNEL_5_15 $(CFLAGS_KERNEL)
CFLAGS += -I $(PWD) -I /home/bsoh/toy_project_smart_home/lib/ioctl

KEYPAD_MODULE=keypad-module
BUZZER_MODULE=buzzer-module
OLED_MODULE=oled-module
PWM_DIR=PWM
BUZZER_DIR=BUZZER

all:
	make -C $(KERNDIR) M=$(PWD) modules

clean:
	make -C $(KERNDIR) M=$(PWD) clean
	rm -rf *.ko
	rm -rf *.o
install:
	sudo insmod $(OLED_MODULE).ko
	sudo insmod $(BUZZER_MODULE).ko
	sudo insmod $(KEYPAD_MODULE).ko

$(KEYPAD_MODULE)-y := \
	keyPad.o

$(BUZZER_MODULE)-y := \
	module_buzzer.o \
	buzzer.o \
	pwm.o

$(OLED_MODULE)-y := \
	module_oled.o \
	ssd1306.o \
	linux_custom_spi.o \
	linux_custom_gpio.o 

obj-m = $(KEYPAD_MODULE).o $(BUZZER_MODULE).o $(OLED_MODULE).o
