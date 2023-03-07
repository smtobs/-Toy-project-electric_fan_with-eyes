#include <linux/init.h>
#include <linux/module.h>

#include "ssd1306.h"

static ssd1306_obj_t ssd1306;

static int __init InitModuleSSD1306(void)
{
    printk("init ssd1306 driver !\n");

    if (CreateSSD1306Obj(&ssd1306) == false)
    {
        /* Todo */
	printk("error SSD1306 obj");
	return -1;
    }

    printk("init.............");
    ssd1306.InitFunc(&ssd1306);

    return 0;
}

static void __exit ExitModuleSSD1306(void)
{ 
    if (ssd1306.info.instance)
    {
        ssd1306.ClearDisplayFunc();
        ssd1306.DeInitFunc(&ssd1306);
        pr_info("ssd1306 driver Unregistered\n");
    }
}
 
module_init(InitModuleSSD1306);
module_exit(ExitModuleSSD1306);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bsoh");
MODULE_DESCRIPTION("SSD1306 OLED Driver");
MODULE_VERSION("1.0");

