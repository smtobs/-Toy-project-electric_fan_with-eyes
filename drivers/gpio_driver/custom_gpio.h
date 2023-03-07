#ifndef _CUSTOM_GPIO_H_
#define _CUSTOM_GPIO_H_

#include "util.h"


#define GPIO_HIGH       ( 1 )
#define GPIO_LOW        ( 0 )

#define GPIO_OUTPUT     ( 1 )
#define GPIO_INPUT      ( 0 )

typedef struct gpio_obj_t
{
    bool (*Init)(uint8_t gpio_number, uint8_t io, uint8_t gpio_state, const char *label);
    void (*DeInit)(uint8_t gpio_number);
    //int (*Read)(void);
    void (*Write)(uint8_t gpio_number, uint8_t value);
}gpio_obj_t;

bool CreateGpioObj(struct gpio_obj_t *gpio);
#endif

