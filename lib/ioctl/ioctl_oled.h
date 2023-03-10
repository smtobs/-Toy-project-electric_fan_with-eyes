#ifndef _IOCTL_OLED_H_
#define _IOCTL_OLED_H_
 
#define IOCTL_OLED_MAGIC    253
 
typedef struct
{
    char str[64];
    uint8_t line_no;
    uint8_t cursor_pos;
} __attribute__ ((packed)) ioctl_oled_t;
 
 
#define IOCTL_OLED_TEST                 _IO(IOCTL_OLED_MAGIC, 0)
#define IOCTL_OLED_DISPLAY_WRITE        _IOW(IOCTL_OLED_MAGIC, 1, ioctl_oled_t)
//#define IOCTL_READ                      _IOR(IOCTL_OLED_MAGIC, 2, ioctl_oled_t)
 
#define IOCTL_MAXNR                     3
  
#endif
