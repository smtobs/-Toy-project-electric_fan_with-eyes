#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define DEVICE_MAJOR_NUM       0
#define DEV_NAME               "keyPad"

#define MAX_ROW_KEYPAD         4
#define MAX_COL_KEYPAD         4

typedef struct _keypad_driver
{
    const char lists[MAX_COL_KEYPAD][MAX_ROW_KEYPAD];
    const struct gpio row_pin[MAX_ROW_KEYPAD];
    const struct gpio col_pin[MAX_COL_KEYPAD];
    const unsigned int debounce_time;
}keypad_driver;

keypad_driver g_keypad_driver =
{
    .col_pin =
    {
        {4, GPIOF_OUT_INIT_HIGH,  "Col 1" },
        {17, GPIOF_OUT_INIT_HIGH, "Col 2" },
        {22, GPIOF_OUT_INIT_HIGH, "Col 3" },
        {27, GPIOF_OUT_INIT_HIGH, "Col 4" }
    },

    .row_pin =
    {
        {23, GPIOF_IN, "Row 1" },
        {24, GPIOF_IN, "Row 2" },
        {25, GPIOF_IN, "Row 3" },
        {18, GPIOF_IN, "Row 4" }
    },

    .lists = 
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    },
    .debounce_time = 100,
};

static int OpenKeyPadDriver(struct inode *inode, struct file *filp)
{
    printk("Simple device driver programming: oepn virtual device !!!\n");
    return 0;
}

static int ReleaseKeyPadDriver(struct inode *inode, struct file *filp)
{
    return 0;
}

static ssize_t ReadKeyPadDriver(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    const uint8_t gpio_reset = 0, gpio_set = 1, detect_voltage = 1; 
    char key = 0;
    int i, k;
   
    printk("Access read() function through \"struct file_operations\"\n");
    for (i = 0; i < MAX_COL_KEYPAD; i++)
    {
        gpio_set_value(g_keypad_driver.col_pin[i].gpio, gpio_set);
        for (k = 0; k < MAX_ROW_KEYPAD; k++)
        {
            if (gpio_get_value(g_keypad_driver.row_pin[k].gpio) == detect_voltage)
            {
                key = g_keypad_driver.lists[k][i];
               // goto key_press_detection;
    	       printk("key_press_detection [%c]\n\n", key);
            } 
        }
        gpio_set_value(g_keypad_driver.col_pin[i].gpio, gpio_reset);
    }
   // key_press_detection:
    //printk("key_press_detection [%c]\n\n", key);
    return 0;
}

static ssize_t WriteKeyPadDriver(struct file *filp, const char __user *buf, size_t len, loff_t *f_pos)
{
    return 0;
}

struct file_operations keypad_fops =
{
    .owner = THIS_MODULE,
    .open = OpenKeyPadDriver,
    .release = ReleaseKeyPadDriver,
    .read = ReadKeyPadDriver,
    .write = WriteKeyPadDriver,
};

static int InitKeyPadDriver(void)
{
    int index, major_num = register_chrdev(DEVICE_MAJOR_NUM, DEV_NAME, &keypad_fops);
    if (major_num < 0)
    {
        printk(KERN_WARNING"%s: can't get or assign major number %d\n",
        DEV_NAME,
        DEVICE_MAJOR_NUM);
        return major_num;
    }

    /* Row Keypad GPIO */
    if (gpio_request_array(g_keypad_driver.row_pin, ARRAY_SIZE(g_keypad_driver.row_pin)))
    {
        printk("Row Keypad GPIO request Error...\n");
        return -1;
    }

    /* Col Keypad GPIO */
    if (gpio_request_array(g_keypad_driver.col_pin, ARRAY_SIZE(g_keypad_driver.col_pin)))
    {
        printk("Col Keypad GPIO request Error...\n");
        return -1;
    }
    
    for (index = 0; index < MAX_ROW_KEYPAD; index++)
    {
        if (gpio_set_debounce(g_keypad_driver.row_pin[index].gpio, g_keypad_driver.debounce_time) < 0)
        {
            printk("Row Keypad GPIO debounce Setting Error...[%d]", index);
           // return -1;
        }
    }
    printk("Success to load the device %s Major number is %d\n", DEV_NAME, major_num);
    return 0;
}

static void ExitKeyPadDriver(void)
{
    unregister_chrdev(DEVICE_MAJOR_NUM, DEV_NAME);
    printk("Success to unload the device %s...\n", DEV_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BSOH");

module_init(InitKeyPadDriver);
module_exit(ExitKeyPadDriver);

