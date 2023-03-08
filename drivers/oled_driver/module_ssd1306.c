#include <linux/init.h>
#include <linux/module.h>

#include "ssd1306.h"
#include "util.h"
#include "logo.h"

static ssd1306_obj_t ssd1306;

static int __init InitModuleSSD1306(void)
{
    printk("init ssd1306 driver !\n");

    if (CreateSSD1306Obj(&ssd1306) == false)
    {
        printk("error SSD1306 obj");
        return -1;
    }

    printk("init.............");
    ssd1306.InitFunc(&ssd1306);

    ssd1306.PrintLogoFunc(g_oled_logo[BOOT_LOGO]);
    U_DELAY_MS(5000);

    ssd1306->ClearDisplayFunc();
    ssd1306->DeactivateScrollFunc();

    /* Enable the Horizontal scroll for first 3 lines */
    ssd1306->StartScrollHorizontalFunc(true, 0, 2);

    ssd1306->WriteStringFunc("Welcome\nTo\nET-E9\n", 0, 0);
    ssd1306->WriteStringFunc("SPI Linux\n", 4, 35);
    ssd1306->WriteStringFunc("Device Driver\n", 5, 23);
    ssd1306->WriteStringFunc("Project\n", 6, 37);

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

