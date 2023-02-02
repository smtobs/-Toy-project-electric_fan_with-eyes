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
#define MAX_KEYPAD_BUFFER      32

#define RISING_DEGE            1
#define FALLING_DEGE           0

#define PUSH_KEYPAD            RISING_DEGE
#define RELEASE_KEYPAD         FALLING_DEGE

#define INIT_COL_GPIO_STATUS   GPIOF_OUT_INIT_LOW
#define INIT_ROW_GPIO_STATUS   GPIOF_IN

#define COL_GPIO_SET           1
#define COL_GPIO_RESET         0

static int OpenKeyPadDriver(struct inode *inode, struct file *filp);
static int ReleaseKeyPadDriver(struct inode *inode, struct file *filp);
static ssize_t SendKeyPadDriverMsgToUser(char __user *buf, size_t count, const char *msg);
static void WaitKeyPadRelase(const uint8_t index);
static ssize_t ReadKeyPadDriver(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t WriteKeyPadDriver(struct file *filp, const char __user *buf, size_t len, loff_t *f_pos);
static int InitKeypadBuff(void);
static int InitKeyPadDriver(void);
static void ExitKeyPadDriver(void);

struct file_operations keypad_fops =
{
    .owner = THIS_MODULE,
    .open = OpenKeyPadDriver,
    .release = ReleaseKeyPadDriver,
    .read = ReadKeyPadDriver,
    .write = WriteKeyPadDriver,
};

typedef struct _keypad_driver
{
    const char lists[MAX_COL_KEYPAD][MAX_ROW_KEYPAD];
    const struct gpio row_pin[MAX_ROW_KEYPAD];
    const struct gpio col_pin[MAX_COL_KEYPAD];
    const unsigned int debounce_time;
    char *buffer;
    int major_num;
}keypad_driver;

keypad_driver g_keypad_driver =
{
    .col_pin =
    {
        {4, INIT_COL_GPIO_STATUS,  "Col 1" },
        {17, INIT_COL_GPIO_STATUS, "Col 2" },
        {22, INIT_COL_GPIO_STATUS, "Col 3" },
        {27, INIT_COL_GPIO_STATUS, "Col 4" }
    },

    .row_pin =
    {
        {23, INIT_ROW_GPIO_STATUS, "Row 1" },
        {24, INIT_ROW_GPIO_STATUS, "Row 2" },
        {25, INIT_ROW_GPIO_STATUS, "Row 3" },
        {18, INIT_ROW_GPIO_STATUS, "Row 4" }
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
    printk("Success to keypad Driver");
    return 0;
}

static int ReleaseKeyPadDriver(struct inode *inode, struct file *filp)
{
    uint8_t i;

    for (i = 0; i < MAX_COL_KEYPAD; i++)
    {
        gpio_set_value(g_keypad_driver.col_pin[i].gpio, COL_GPIO_RESET);
    }
    return 0;
}

static ssize_t SendKeyPadDriverMsgToUser(char __user *buf, size_t count, const char *msg)
{
    if (count > MAX_KEYPAD_BUFFER)
    {
        count = MAX_KEYPAD_BUFFER;
    }
    
    memset(g_keypad_driver.buffer, 0x0, MAX_KEYPAD_BUFFER);
    memcpy(g_keypad_driver.buffer, msg, strlen(msg));

    if (copy_to_user(buf, g_keypad_driver.buffer, count))
    {
	    printk("[%s] : copy_to_user failed", __FUNCTION__);
        return -EFAULT;
    }
    return count;
}

static void WaitKeyPadRelase(const uint8_t index)
{
    /* Timeout exception handling is handled by the application. */
    do
    {
        mdelay(1); 
    } while (((gpio_get_value(g_keypad_driver.row_pin[index].gpio)) != RELEASE_KEYPAD));
}

static ssize_t ReadKeyPadDriver(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    uint8_t i, k;
    char key[2] = {0,};
   
    for (i = 0; i < MAX_COL_KEYPAD; i++)
    {
        gpio_set_value(g_keypad_driver.col_pin[i].gpio, COL_GPIO_SET);
        for (k = 0; k < MAX_ROW_KEYPAD; k++)
        {
            if (gpio_get_value(g_keypad_driver.row_pin[k].gpio) == PUSH_KEYPAD)
            {
                WaitKeyPadRelase(k);
                key[0] = g_keypad_driver.lists[k][i];
            } 
        }
        gpio_set_value(g_keypad_driver.col_pin[i].gpio, COL_GPIO_RESET);
    }
    return key[0] != 0 ? SendKeyPadDriverMsgToUser(buf, count, key) : 0;
}

static ssize_t WriteKeyPadDriver(struct file *filp, const char __user *buf, size_t len, loff_t *f_pos)
{
    return 0;
}

static int InitKeypadBuff(void)
{
    g_keypad_driver.buffer = (char*)kmalloc(MAX_KEYPAD_BUFFER, GFP_KERNEL);
    if (g_keypad_driver.buffer == NULL)
    {
        return ENOMEM;
    }
    memset(g_keypad_driver.buffer, 0x0, MAX_KEYPAD_BUFFER);
    return 0;
}

static int InitKeyPadDriver(void)
{
    int index;

    if ((g_keypad_driver.major_num  = register_chrdev(DEVICE_MAJOR_NUM, DEV_NAME, &keypad_fops)) < 0)
    {
        printk(KERN_WARNING"%s: can't get or assign major number %d\n",
        DEV_NAME,
        DEVICE_MAJOR_NUM);
        return g_keypad_driver.major_num;
    }

    /* Row Keypad GPIO */
    if (gpio_request_array(g_keypad_driver.row_pin, ARRAY_SIZE(g_keypad_driver.row_pin)))
    {
        printk("[%s] : Row Keypad GPIO request Error...\n", __FUNCTION__);
        goto error_row_gpio_request;
    }

    /* Col Keypad GPIO */
    if (gpio_request_array(g_keypad_driver.col_pin, ARRAY_SIZE(g_keypad_driver.col_pin)))
    {
        printk("[%s] : Col Keypad GPIO request Error...\n", __FUNCTION__);
        goto error_col_gpio_request;
    }
    
    /* Row Keypad GPIO set debounce */
    for (index = 0; index < MAX_ROW_KEYPAD; index++)
    {
        gpio_set_debounce(g_keypad_driver.row_pin[index].gpio, g_keypad_driver.debounce_time);
    }

    /* Init Keypad Buff */
    if (InitKeypadBuff() > 0)
    {
        goto error_init_buffer;
    }
    printk("Success to load the device %s Major number is %d\n", DEV_NAME, g_keypad_driver.major_num);
    return 0;

error_init_buffer:
    gpio_free_array(g_keypad_driver.col_pin, ARRAY_SIZE(g_keypad_driver.col_pin));

error_col_gpio_request:
    gpio_free_array(g_keypad_driver.row_pin, ARRAY_SIZE(g_keypad_driver.row_pin));

error_row_gpio_request:
    unregister_chrdev(g_keypad_driver.major_num, DEV_NAME);

    return -1;
}

static void ExitKeyPadDriver(void)
{
    kfree(g_keypad_driver.buffer);
    gpio_free_array(g_keypad_driver.row_pin, ARRAY_SIZE(g_keypad_driver.row_pin));
    gpio_free_array(g_keypad_driver.col_pin, ARRAY_SIZE(g_keypad_driver.col_pin));
    unregister_chrdev(DEVICE_MAJOR_NUM, DEV_NAME);
    printk("Success to unload the device %s...\n", DEV_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BSOH");

module_init(InitKeyPadDriver);
module_exit(ExitKeyPadDriver);
