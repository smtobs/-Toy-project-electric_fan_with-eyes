#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include "buzzer.h"

#define DEVICE_MAJOR_NUM       251
#define DEV_NAME               "/dev/buzzer"

static int OpenBuzzerModule(struct inode *inode, struct file *filp);
static ssize_t WriteBuzzerModule(struct file *filp, const char __user *buf, size_t len, loff_t *f_pos);
static int InitBuzzerModule(void);
static void ExitBuzzerModule(void);

static buzzer_obj_t buzzer_mgr;

struct file_operations buzzer_fops =
{
    .owner = THIS_MODULE,
    .open  = OpenBuzzerModule,
    .write = WriteBuzzerModule,
};

static int OpenBuzzerModule(struct inode *inode, struct file *filp)
{
    printk("Simple device driver programming: oepn virtual device !!!\n");

    /* Create Buzzer Obj */
    if (CreateBuzzerObj(&buzzer_mgr) == false)
    {
         /* Todo */
         return -1;
    }
    return 0;
}

static ssize_t WriteBuzzerModule(struct file *filp, const char __user *buf, size_t len, loff_t *f_pos)
{
#if (0)
    if (buzzer_mgr == NULL)
    {
         
    }

    switch ()
    {
        case :
             break;
        default :
             break;
    }
#endif
    buzzer_mgr.OnSoundFunc(&buzzer_mgr);

    printk("Simple device driver programming: oepn virtual device !!!\n");
    return len;
}

static int InitBuzzerModule(void)
{
    /* Register Module */
    buzzer_mgr.major_num = register_chrdev(DEVICE_MAJOR_NUM, DEV_NAME, &buzzer_fops);
    if (buzzer_mgr.major_num  < 0)
    {
        printk(KERN_WARNING"%s: can't get or assign major number %d\n",
        DEV_NAME,
        DEVICE_MAJOR_NUM);
        return buzzer_mgr.major_num;
    }

    printk("Success to load the device %s. Major number is %d\n", DEV_NAME, buzzer_mgr.major_num);
    return 0;
}

static void ExitBuzzerModule(void)
{
    int num = buzzer_mgr.major_num;
    buzzer_mgr.RemoveObjFunc(&buzzer_mgr);

    unregister_chrdev(num, DEV_NAME);
    printk("Success to unload the device %s...\n", DEV_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bsoh");

module_init(InitBuzzerModule);
module_exit(ExitBuzzerModule);
