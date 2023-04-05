KERNDIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)

BASE_DIR=/home/bsoh/access_control_system

CFLAGS_KERNEL := -I $(BASE_DIR)
CFLAGS += -D__KERNEL__ -DKERNEL_5_15 $(CFLAGS_KERNEL)
CFLAGS += -I $(PWD) -I $(BASE_DIR)

BUZZER_DIR=buzzer_driver
OLED_DIR=oled_driver
KEYPAD_DIR=keypad_driver

KEYPAD_MODULE=keypad-module
BUZZER_MODULE=buzzer-module
OLED_MODULE=oled-module

all:
	make -C $(OLED_DIR)
	make -C $(BUZZER_DIR)
	make -C $(KEYPAD_DIR)
	make -C $(KERNDIR) M=$(PWD) modules

clean:
	make -C $(OLED_DIR) clean
	make -C $(BUZZER_DIR) clean
	make -C $(KEYPAD_DIR) clean
	make -C $(KERNDIR) M=$(PWD) clean

install:
	sudo insmod $(OLED_DIR)/$(OLED_MODULE).ko
	sudo insmod $(BUZZER_DIR)/$(BUZZER_MODULE).ko
	sudo insmod $(KEYPAD_DIR)/$(KEYPAD_MODULE).ko

