#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#define U_DELAY_MS(x)      mdelay(x)
#define U_DELAY_US(x)      udelay(x)
#define U_MALLOC(x)        kmalloc(x, GFP_KERNEL)

#define U_SAFETY_FREE(x) \
      if (x) \
      { \
           kfree(x); \
           x = NULL; \
      }
