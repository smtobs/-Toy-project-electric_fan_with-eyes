KERNDIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

OLED_MODULE=oled-module
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
	sudo insmod $(OLED_MODULE).ko

$(OLED_MODULE)-y := \
	module_oled.o \
	ssd1306.o \
	../spi_driver/linux_custom_spi.o \
	../gpio_driver/linux_custom_gpio.o 

obj-m = $(OLED_MODULE).o
