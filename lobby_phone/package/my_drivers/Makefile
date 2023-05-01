PWD := $(shell pwd)

KEYPAD_MODULE=keypad-module
BUZZER_MODULE=buzzer-module
OLED_MODULE=oled-module

all:
	$(MAKE) -C '$(LINUX_DIR)' M='$(PWD)'

clean:
	$(MAKE) -C '$(LINUX_DIR)' M='$(PWD)' clean

install:
	sudo insmod $(BUZZER_MODULE).ko
	sudo insmod $(OLED_MODULE).ko
	sudo insmod $(KEYPAD_MODULE).ko


obj-m += $(BUZZER_MODULE).o
obj-m += $(KEYPAD_MODULE).o
obj-m += $(OLED_MODULE).o

$(BUZZER_MODULE)-objs := \
	buzzer_driver/module_buzzer.o \
	buzzer_driver/buzzer.o \
	pwm_driver/pwm.o

$(KEYPAD_MODULE)-objs := \
	keypad_driver/keyPad.o

$(OLED_MODULE)-objs := \
	oled_driver/module_oled.o \
	oled_driver/ssd1306.o \
	spi_driver/linux_custom_spi.o \
	gpio_driver/linux_custom_gpio.o

