KERNDIR=/lib/modules/$(shell uname -r)/build
KEYPAD_MODULE=keypad-module
BUZZER_MODULE=buzzer-module
OLED_MODULE=oled-module
PWM_DIR=PWM
BUZZER_DIR=BUZZER

PWD=$(shell pwd)

$(KEYPAD_MODULE)-y := \
	keyPad.o

$(BUZZER_MODULE)-y := \
	module_buzzer.o \
	buzzer.o \
	pwm.o

$(OLED_MODULE)-y := \
	module_ssd1306.o \
	ssd1306.o \
	linux_custom_spi.o \
	linux_custom_gpio.o

obj-m = $(KEYPAD_MODULE).o $(BUZZER_MODULE).o $(OLED_MODULE).o

all:
	make -C $(KERNDIR) M=$(PWD) modules

clean:
	make -C $(KERNDIR) M=$(PWD) clean
	rm -rf *.ko
	rm -rf *.o
