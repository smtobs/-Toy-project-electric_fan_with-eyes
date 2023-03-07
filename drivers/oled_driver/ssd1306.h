#ifndef _SSD1306_H_
#define _SSD1306_H_

#include "util.h"

#define SPI_BUS_NUM             (  1 )    // SPI 1
#define SSD1306_RST_PIN         (  26 )   // Reset pin
#define SSD1306_DC_PIN          (  13 )   // Data/Command pin
#define SSD1306_MAX_SEG         ( 128 )   // Maximum segment
#define SSD1306_MAX_LINE        (   7 )   // Maximum line
#define SSD1306_DEF_FONT_SIZE   (   5 )   // Default font size

typedef struct ssd1306_info_t
{
    bool instance;
    uint8_t lineNum;
    uint8_t cursorPos;
    uint8_t fontSize;
}ssd1306_info_t;

typedef struct ssd1306_obj_t
{
    struct ssd1306_info_t info;
    int (*InitFunc)(struct ssd1306_obj_t *this);
    void (*DeInitFunc)(struct ssd1306_obj_t *this);
    void (*WriteStringFunc)(char *str, uint8_t lineNo, uint8_t cursorPos);
    void (*ClearDisplayFunc)(void);
    void (*PrintLogoFunc)(uint8_t etx_logo[]);
    void (*SetBrightnessFunc)(uint8_t brightnessValue);
    void (*StartScrollHorizontalFunc)(bool is_left_scroll, uint8_t start_line_no, uint8_t end_line_no);
    void (*InvertDisplayFunc)(bool need_to_invert);
    void (*DeactivateScrollFunc)(void);
}ssd1306_obj_t;

bool CreateSSD1306Obj(struct ssd1306_obj_t *oled);

#endif
