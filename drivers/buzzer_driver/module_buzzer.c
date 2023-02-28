#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <asm/io.h>
#include "buzzer.h"

#define DEVICE_MAJOR_NUM                    0
#define DEV_NAME                            "/dev/buzzer"

#define IOCTL_BUZZER_MAGIC                  'b'
#define SUCCESS_SOUND_BUZZER                0
#define BUTTON_PUSH_SOUND_BUZZER            1

#define IOCTL_CMD_SUCCESS_SOUND_BUZZER      _IO(IOCTL_BUZZER_MAGIC, SUCCESS_SOUND_BUZZER)
#define IOCTL_CMD_BUTTON_PUSH_SOUND_BUZZER  _IO(IOCTL_BUZZER_MAGIC, BUTTON_PUSH_SOUND_BUZZER)

static int OpenBuzzerModule(struct inode *inode, struct file *filp);
static int ReleaseBuzzerModule(struct inode *inode, struct file *filp);
static long IoctlBuzzerModule(struct file *filp, unsigned int cmd, unsigned long arg);
static int InitBuzzerModule(void);
static void ExitBuzzerModule(void);

static buzzer_obj_t buzzer_mgr;

struct file_operations buzzer_fops =
{
    .owner   = THIS_MODULE,
    .open    = OpenBuzzerModule,
    .release = ReleaseBuzzerModule,
    .unlocked_ioctl = IoctlBuzzerModule,
};

static int OpenBuzzerModule(struct inode *inode, struct file *filp)
{ 
    /* Create Buzzer Obj */
    if (CreateBuzzerObj(&buzzer_mgr) == false)
    {
        printk("Failed to create buzzer object.");
        return -1;
    }
    
    buzzer_mgr.instance = true;

    return 0;
}

static int ReleaseBuzzerModule(struct inode *inode, struct file *filp)
{
    if (buzzer_mgr.instance == true)
    {
        buzzer_mgr.RemoveObjFunc(&buzzer_mgr);
    }
    return 0;
}

static long IoctlBuzzerModule(struct file *filp, unsigned int cmd, unsigned long arg)
{
    if (_IOC_TYPE(cmd) != IOCTL_BUZZER_MAGIC)
    {
        return -EINVAL;
    }

    switch (cmd)
    {
        case IOCTL_CMD_SUCCESS_SOUND_BUZZER :
            buzzer_mgr.SuccessSoundFunc(&buzzer_mgr);
            break;

        case IOCTL_CMD_BUTTON_PUSH_SOUND_BUZZER :
            buzzer_mgr.ButtonPushSoundFunc(&buzzer_mgr);
            break;

        default:
            break;
    }

    return 0;
}

static int InitBuzzerModule(void)
{
    /* Register Module */
    buzzer_mgr.major_num = register_chrdev(DEVICE_MAJOR_NUM, DEV_NAME, &buzzer_fops);
    if (buzzer_mgr.major_num  < 0)
    {
        printk(KERN_WARNING"%s: can't get or assign major number %d\n",
        DEV_NAME,
        buzzer_mgr.major_num);
        return buzzer_mgr.major_num;
    }

    printk("Success to load the device %s. Major number is %d\n", DEV_NAME, buzzer_mgr.major_num);
    return 0;
}

static void ExitBuzzerModule(void)
{
    if (buzzer_mgr.instance == true)
    {
        buzzer_mgr.RemoveObjFunc(&buzzer_mgr);
    }

    unregister_chrdev(buzzer_mgr.major_num, DEV_NAME);
    printk("Success to unload the device %s...\n", DEV_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bsoh");

module_init(InitBuzzerModule);
module_exit(ExitBuzzerModule);
