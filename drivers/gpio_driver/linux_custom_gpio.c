#include <linux/gpio.h>
#include "custom_gpio.h"

static bool InitGpio(uint8_t gpio_number, uint8_t io, uint8_t gpio_state, const char *label);
static void DeInitGpio(uint8_t gpio_number);
//static int ReadGpio();
static void WriteGpio(uint8_t gpio_number, uint8_t value);

bool CreateGpioObj(struct gpio_obj_t *gpio)
{
    /* Register for GPIO Function */
    gpio->Init    = InitGpio;
    gpio->DeInit  = DeInitGpio;
    //gpio->Read    = ReadGpio;
    gpio->Write   = WriteGpio;

    return true;
}

static _Bool InitGpio(uint8_t gpio_number, uint8_t io, uint8_t gpio_state, const char *label)
{
    if (gpio_is_valid(gpio_number) == false)
    {
        goto error;
    }
    
    if ((gpio_request(gpio_number, label)) < 0)
    {
        goto error;
    }

    if (io == GPIO_INPUT)
    {
        // Todo
    }
    else
    {
        gpio_direction_output(gpio_number, gpio_state);
    }
    
    return true;

    error:
    return false;
}

static void DeInitGpio(uint8_t gpio_number)
{
    gpio_free(gpio_number);
}

//static int ReadGpio()
//{
    //Todo
//}

static void WriteGpio(uint8_t gpio_number, uint8_t value)
{
    gpio_set_value(gpio_number, value);
}

