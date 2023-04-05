#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#include "ssd1306.h"
#include "../common/util.h"
#include "logo.h"
#include "../../lib/ioctl/ioctl_oled.h"

#define DEVICE_MAJOR_NUM                    0
#define DEV_NAME                            "/dev/oled"

static ssd1306_obj_t oled;
static bool g_is_open_oled; 
static int g_major_num;

static int OpenOledModule(struct inode *inode, struct file *filp);
static int ReleaseOledModule(struct inode *inode, struct file *filp);
static long IoctlOledModule(struct file *filp, unsigned int cmd, unsigned long arg);

struct file_operations oled_fops =
{
    .owner   = THIS_MODULE,
    .open    = OpenOledModule,
    .release = ReleaseOledModule,
    .unlocked_ioctl = IoctlOledModule,
};

static int OpenOledModule(struct inode *inode, struct file *filp)
{
    if (g_is_open_oled)
    {
        printk("OLED Driver is already open.");
        return -1;
    }
    g_is_open_oled = true;

    return 0;
}

static int ReleaseOledModule(struct inode *inode, struct file *filp)
{
    g_is_open_oled = false;
    return 0;
}

static long IoctlOledModule(struct file *filp, unsigned int cmd, unsigned long arg)
{
    ioctl_oled_t *ioctl_oled;

    if (_IOC_TYPE(cmd) != IOCTL_OLED_MAGIC)
    {
        return -EINVAL;
    }

    switch (cmd)
    {
        case IOCTL_OLED_DISPLAY_WRITE :
            ioctl_oled = (ioctl_oled_t *)arg;
            oled.WriteStringFunc(ioctl_oled->str, ioctl_oled->line_no, ioctl_oled->cursor_pos);
            break;

        case IOCTL_OLED_CLEAR :
            oled.ClearDisplayFunc();
            break;

        default:
            break;
    }

    return 0;
}

static int __init InitModuleOled(void)
{
    /* Register for Module */
    g_major_num = register_chrdev(DEVICE_MAJOR_NUM, DEV_NAME, &oled_fops);
    if (g_major_num  < 0)
    {
        printk(KERN_WARNING"%s: can't get or assign major number %d\n",
        DEV_NAME,
        g_major_num);
        return g_major_num;
    }

    if (CreateSSD1306Obj(&oled) == false)
    {
        printk("error SSD1306 obj");
        return -1;
    }

    oled.InitFunc(&oled);
    oled.PrintLogoFunc(g_oled_boot_logo);
    U_DELAY_MS(5000);

    printk("OLED Driver major num : [%s] [%d]: \n", DEV_NAME, g_major_num);

    return 0;
}

static void __exit ExitModuleOled(void)
{ 
    if (oled.info.instance)
    {
        oled.ClearDisplayFunc();
        oled.DeInitFunc(&oled);
        pr_info("Oled driver Unregistered\n");
    }

    unregister_chrdev(g_major_num, DEV_NAME);
    printk("Success to unload the device %s...\n", DEV_NAME);
}
 
module_init(InitModuleOled);
module_exit(ExitModuleOled);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bsoh");
MODULE_DESCRIPTION("SSD1306 OLED Driver");
MODULE_VERSION("1.0");

