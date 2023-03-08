#include "custom_gpio.h"
#include "custom_spi.h"
#include "ssd1306.h"

static gpio_obj_t gpio;
static spi_obj_t spi;

/*
** Variable to store Line Number and Cursor Position.
*/ 
static uint8_t SSD1306_LineNum   = 0;
static uint8_t SSD1306_CursorPos = 0;
static uint8_t SSD1306_FontSize  = SSD1306_DEF_FONT_SIZE;

/*
** Array Variable to store the letters.
*/ 
static const unsigned char SSD1306_font[][SSD1306_DEF_FONT_SIZE] = 
{
    {0x00, 0x00, 0x00, 0x00, 0x00},   // space
    {0x00, 0x00, 0x2f, 0x00, 0x00},   // !
    {0x00, 0x07, 0x00, 0x07, 0x00},   // "
    {0x14, 0x7f, 0x14, 0x7f, 0x14},   // #
    {0x24, 0x2a, 0x7f, 0x2a, 0x12},   // $
    {0x23, 0x13, 0x08, 0x64, 0x62},   // %
    {0x36, 0x49, 0x55, 0x22, 0x50},   // &
    {0x00, 0x05, 0x03, 0x00, 0x00},   // '
    {0x00, 0x1c, 0x22, 0x41, 0x00},   // (
    {0x00, 0x41, 0x22, 0x1c, 0x00},   // )
    {0x14, 0x08, 0x3E, 0x08, 0x14},   // *
    {0x08, 0x08, 0x3E, 0x08, 0x08},   // +
    {0x00, 0x00, 0xA0, 0x60, 0x00},   // ,
    {0x08, 0x08, 0x08, 0x08, 0x08},   // -
    {0x00, 0x60, 0x60, 0x00, 0x00},   // .
    {0x20, 0x10, 0x08, 0x04, 0x02},   // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00},   // 1
    {0x42, 0x61, 0x51, 0x49, 0x46},   // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31},   // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10},   // 4
    {0x27, 0x45, 0x45, 0x45, 0x39},   // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6
    {0x01, 0x71, 0x09, 0x05, 0x03},   // 7
    {0x36, 0x49, 0x49, 0x49, 0x36},   // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E},   // 9
    {0x00, 0x36, 0x36, 0x00, 0x00},   // :
    {0x00, 0x56, 0x36, 0x00, 0x00},   // ;
    {0x08, 0x14, 0x22, 0x41, 0x00},   // <
    {0x14, 0x14, 0x14, 0x14, 0x14},   // =
    {0x00, 0x41, 0x22, 0x14, 0x08},   // >
    {0x02, 0x01, 0x51, 0x09, 0x06},   // ?
    {0x32, 0x49, 0x59, 0x51, 0x3E},   // @
    {0x7C, 0x12, 0x11, 0x12, 0x7C},   // A
    {0x7F, 0x49, 0x49, 0x49, 0x36},   // B
    {0x3E, 0x41, 0x41, 0x41, 0x22},   // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C},   // D
    {0x7F, 0x49, 0x49, 0x49, 0x41},   // E
    {0x7F, 0x09, 0x09, 0x09, 0x01},   // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A},   // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F},   // H
    {0x00, 0x41, 0x7F, 0x41, 0x00},   // I
    {0x20, 0x40, 0x41, 0x3F, 0x01},   // J
    {0x7F, 0x08, 0x14, 0x22, 0x41},   // K
    {0x7F, 0x40, 0x40, 0x40, 0x40},   // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},   // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F},   // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E},   // O
    {0x7F, 0x09, 0x09, 0x09, 0x06},   // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E},   // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46},   // R
    {0x46, 0x49, 0x49, 0x49, 0x31},   // S
    {0x01, 0x01, 0x7F, 0x01, 0x01},   // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F},   // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F},   // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F},   // W
    {0x63, 0x14, 0x08, 0x14, 0x63},   // X
    {0x07, 0x08, 0x70, 0x08, 0x07},   // Y
    {0x61, 0x51, 0x49, 0x45, 0x43},   // Z
    {0x00, 0x7F, 0x41, 0x41, 0x00},   // [
    {0x55, 0xAA, 0x55, 0xAA, 0x55},   // Backslash (Checker pattern)
    {0x00, 0x41, 0x41, 0x7F, 0x00},   // ]
    {0x04, 0x02, 0x01, 0x02, 0x04},   // ^
    {0x40, 0x40, 0x40, 0x40, 0x40},   // _
    {0x00, 0x03, 0x05, 0x00, 0x00},   // `
    {0x20, 0x54, 0x54, 0x54, 0x78},   // a
    {0x7F, 0x48, 0x44, 0x44, 0x38},   // b
    {0x38, 0x44, 0x44, 0x44, 0x20},   // c
    {0x38, 0x44, 0x44, 0x48, 0x7F},   // d
    {0x38, 0x54, 0x54, 0x54, 0x18},   // e
    {0x08, 0x7E, 0x09, 0x01, 0x02},   // f
    {0x18, 0xA4, 0xA4, 0xA4, 0x7C},   // g
    {0x7F, 0x08, 0x04, 0x04, 0x78},   // h
    {0x00, 0x44, 0x7D, 0x40, 0x00},   // i
    {0x40, 0x80, 0x84, 0x7D, 0x00},   // j
    {0x7F, 0x10, 0x28, 0x44, 0x00},   // k
    {0x00, 0x41, 0x7F, 0x40, 0x00},   // l
    {0x7C, 0x04, 0x18, 0x04, 0x78},   // m
    {0x7C, 0x08, 0x04, 0x04, 0x78},   // n
    {0x38, 0x44, 0x44, 0x44, 0x38},   // o
    {0xFC, 0x24, 0x24, 0x24, 0x18},   // p
    {0x18, 0x24, 0x24, 0x18, 0xFC},   // q
    {0x7C, 0x08, 0x04, 0x04, 0x08},   // r
    {0x48, 0x54, 0x54, 0x54, 0x20},   // s
    {0x04, 0x3F, 0x44, 0x40, 0x20},   // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C},   // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C},   // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C},   // w
    {0x44, 0x28, 0x10, 0x28, 0x44},   // x
    {0x1C, 0xA0, 0xA0, 0xA0, 0x7C},   // y
    {0x44, 0x64, 0x54, 0x4C, 0x44},   // z
    {0x00, 0x10, 0x7C, 0x82, 0x00},   // {
    {0x00, 0x00, 0xFF, 0x00, 0x00},   // |
    {0x00, 0x82, 0x7C, 0x10, 0x00},   // }
    {0x00, 0x06, 0x09, 0x09, 0x06}    // ~ (Degrees)
};

static void FillSSD1306(uint8_t data);
static int DisplayInitSSD1306(void);
static void GoToNextLineSSD1306(void);
static void PrintCharSSD1306(unsigned char c);
static void StringSSD1306(char *str);
static void SetCursorSSD1306(uint8_t lineNo, uint8_t cursorPos);
static int WriteSSD1306(bool is_cmd, uint8_t data);

/* Register for SSD1306 Function */
static int InitSSD1306(struct ssd1306_obj_t *ssd1306);
static void DeInitSSD1306(struct ssd1306_obj_t *ssd1306);
static void WriteStringSSD1306(char *str, uint8_t lineNo, uint8_t cursorPos);
static void ClearDisplaySSD1306(void);
static void PrintLogoSSD1306(uint8_t etx_logo[]);
static void SetBrightnessSSD1306(uint8_t brightnessValue);
static void StartScrollHorizontalSSD1306( bool is_left_scroll, uint8_t start_line_no, uint8_t end_line_no);
static void InvertDisplaySSD1306(bool need_to_invert);
static void DeactivateScrollSSD1306(void);


bool CreateSSD1306Obj(struct ssd1306_obj_t *oled)
{
    oled->info.instance = true;

    if (CreateGpioObj(&gpio) == false)
    {
        printk("Failed to create Gpio object\n");
        return false;
    }


    if ((CreateSpiObj(&spi, "etx-spi-ssd1306-driver", MAX_SPEED_HZ, SPI_BUS_NUM, 0, SPI_MODE0)) == false)
    {
       	printk("Failed to create SPI object\n");
        return false;
    }

    /* Register for GPIO Function */
    oled->InitFunc                  = InitSSD1306;
    oled->DeInitFunc                = DeInitSSD1306;
    oled->WriteStringFunc           = WriteStringSSD1306;
    oled->ClearDisplayFunc          = ClearDisplaySSD1306;
    oled->PrintLogoFunc             = PrintLogoSSD1306;
    oled->SetBrightnessFunc         = SetBrightnessSSD1306;
    oled->StartScrollHorizontalFunc = StartScrollHorizontalSSD1306;
    oled->InvertDisplayFunc         = InvertDisplaySSD1306;
    oled->DeactivateScrollFunc      = DeactivateScrollSSD1306;

    return true;
}

static void FillSSD1306(uint8_t data)
{
    // 8 pages x 128 segments x 8 bits of data
    unsigned int total  = ( SSD1306_MAX_SEG * (SSD1306_MAX_LINE + 1) );
    unsigned int i      = 0;

    //Fill the Display
    for (i = 0; i < total; i++)
    {
        WriteSSD1306(false, data);
    }
}

static int DisplayInitSSD1306(void)
{
    if (gpio.Init(SSD1306_RST_PIN, GPIO_OUTPUT, GPIO_HIGH, "SSD1306_RST_PIN") == false)
    {
        printk("SSD1306 RST Gpio initialization failed\n");
        return -1;
    }

    if (gpio.Init(SSD1306_DC_PIN, GPIO_OUTPUT, GPIO_HIGH, "SSD1306_DC_PIN") == false)
    {
        gpio.DeInit(SSD1306_RST_PIN);
        printk("SSD1306 DC Gpio initialization failed\n");
        return -1;
    }

    gpio.Write(SSD1306_RST_PIN, 0u);
    U_DELAY_MS(100);

    gpio.Write(SSD1306_RST_PIN, 1u);
    U_DELAY_MS(100);
    
    /*
    ** Commands to initialize the SSD_1306 OLED Display
    */
    WriteSSD1306(true, 0xAE); // Entire Display OFF
    WriteSSD1306(true, 0xD5); // Set Display Clock Divide Ratio and Oscillator Frequency
    WriteSSD1306(true, 0x80); // Default Setting for Display Clock Divide Ratio and Oscillator Frequency that is recommended
    WriteSSD1306(true, 0xA8); // Set Multiplex Ratio
    WriteSSD1306(true, 0x3F); // 64 COM lines
    WriteSSD1306(true, 0xD3); // Set display offset
    WriteSSD1306(true, 0x00); // 0 offset
    WriteSSD1306(true, 0x40); // Set first line as the start line of the display
    WriteSSD1306(true, 0x8D); // Charge pump
    WriteSSD1306(true, 0x14); // Enable charge dump during display on
    WriteSSD1306(true, 0x20); // Set memory addressing mode
    WriteSSD1306(true, 0x00); // Horizontal addressing mode
    WriteSSD1306(true, 0xA1); // Set segment remap with column address 127 mapped to segment 0
    WriteSSD1306(true, 0xC8); // Set com output scan direction, scan from com63 to com 0
    WriteSSD1306(true, 0xDA); // Set com pins hardware configuration
    WriteSSD1306(true, 0x12); // Alternative com pin configuration, disable com left/right remap
    WriteSSD1306(true, 0x81); // Set contrast control
    WriteSSD1306(true, 0x80); // Set Contrast to 128
    WriteSSD1306(true, 0xD9); // Set pre-charge period
    WriteSSD1306(true, 0xF1); // Phase 1 period of 15 DCLK, Phase 2 period of 1 DCLK
    WriteSSD1306(true, 0xDB); // Set Vcomh deselect level
    WriteSSD1306(true, 0x20); // Vcomh deselect level ~ 0.77 Vcc
    WriteSSD1306(true, 0xA4); // Entire display ON, resume to RAM content display
    WriteSSD1306(true, 0xA6); // Set Display in Normal Mode, 1 = ON, 0 = OFF
    WriteSSD1306(true, 0x2E); // Deactivate scroll
    WriteSSD1306(true, 0xAF); // Display ON in normal mode
    
    // Clear the display
    ClearDisplaySSD1306();

    return 0;
}

static void GoToNextLineSSD1306(void)
{
    /*
    ** Increment the current line number.
    ** roll it back to first line, if it exceeds the limit. 
    */
    SSD1306_LineNum++;
    SSD1306_LineNum = (SSD1306_LineNum & SSD1306_MAX_LINE);
    SetCursorSSD1306(SSD1306_LineNum,0); /* Finally move it to next line */
}

static void PrintCharSSD1306(unsigned char c)
{
    uint8_t data_byte;
    uint8_t temp = 0;
  
  /*
  ** If we character is greater than segment len or we got new line charcter
  ** then move the cursor to the new line
  */ 
    if ((( SSD1306_CursorPos + SSD1306_FontSize ) >= SSD1306_MAX_SEG ) ||
        ( c == '\n' )
    )
    {
        GoToNextLineSSD1306();
    }
  
  // print charcters other than new line
    if (c != '\n')
    {

    /*
    ** In our font array (SSD1306_font), space starts in 0th index.
    ** But in ASCII table, Space starts from 32 (0x20).
    ** So we need to match the ASCII table with our font table.
    ** We can subtract 32 (0x20) in order to match with our font table.
    */
    c -= 0x20;  //or c -= ' ';
    do
    {
        data_byte = SSD1306_font[c][temp];    // Get the data to be displayed from LookUptable
        WriteSSD1306(false, data_byte);       // write data to the OLED
        SSD1306_CursorPos++;
        
        temp++;
        
    } while (temp < SSD1306_FontSize);

    WriteSSD1306(false, 0x00);         //Display the data
    SSD1306_CursorPos++;
    }
}

static void StringSSD1306(char *str)
{
    while (*str)
    {
        PrintCharSSD1306(*str++);
    }
}

static void SetCursorSSD1306(uint8_t lineNo, uint8_t cursorPos)
{
    /* Move the Cursor to specified position only if it is in range */
    if ((lineNo <= SSD1306_MAX_LINE) && (cursorPos < SSD1306_MAX_SEG))
    {
        SSD1306_LineNum   = lineNo;                 // Save the specified line number
        SSD1306_CursorPos = cursorPos;              // Save the specified cursor position

        WriteSSD1306(true, 0x21);              // cmd for the column start and end address
        WriteSSD1306(true, cursorPos);         // column start addr
        WriteSSD1306(true, SSD1306_MAX_SEG-1); // column end addr
        WriteSSD1306(true, 0x22);              // cmd for the page start and end address
        WriteSSD1306(true, lineNo);            // page start addr
        WriteSSD1306(true, SSD1306_MAX_LINE);  // page end addr
    }
}

static int WriteSSD1306(bool is_cmd, uint8_t data)
{
    int     ret = 0;
    uint8_t pin_value;

    if (is_cmd)
    {
        //DC pin has to be low, if this is command.
        pin_value = 0u;
    }
    else
    {
        //DC pin has to be high, if this is data.
        pin_value = 1u;
    }

    gpio.Write(SSD1306_DC_PIN, pin_value);

    ret = spi.Write(data);

    return  ret;
}

static int InitSSD1306(struct ssd1306_obj_t *ssd1306)
{
    if (spi.Init(&spi) == false)
    {
        printk("Spi initialization failed");
        return -1;
    }

    /* Init Display */
    DisplayInitSSD1306();

    /* Print the String */
    ssd1306->SetBrightnessFunc(255);
    ssd1306->InvertDisplayFunc(false);

    return 0;
}

static void DeInitSSD1306(struct ssd1306_obj_t *ssd1306)
{
    gpio.DeInit(SSD1306_RST_PIN);
    gpio.DeInit(SSD1306_DC_PIN);

    spi.DeInit(&spi);
}

static void WriteStringSSD1306(char *str, uint8_t lineNo, uint8_t cursorPos)
{
    SetCursorSSD1306(lineNo, cursorPos);

    StringSSD1306(str);
}

static void ClearDisplaySSD1306(void)
{
    /* Set cursor */
    SetCursorSSD1306(0, 0);

    FillSSD1306(0x00);
}

static void PrintLogoSSD1306(uint8_t etx_logo[])
{
    int i;

    /* Set cursor */
    SetCursorSSD1306(0, 0);

    for (i = 0; i < ( SSD1306_MAX_SEG * (SSD1306_MAX_LINE + 1) ); i++)
    {
        WriteSSD1306(false, etx_logo[i]);
    }
}

static void SetBrightnessSSD1306(uint8_t brightnessValue)
{
    WriteSSD1306(true, 0x81);            // Contrast command
    WriteSSD1306(true, brightnessValue); // Contrast value (default value = 0x7F)
}

static void StartScrollHorizontalSSD1306( bool is_left_scroll,
                                        uint8_t start_line_no,
                                        uint8_t end_line_no
                                      )
{
    if (is_left_scroll)
    {
        /* left horizontal scroll */
        WriteSSD1306(true, 0x27);
    }
    else
    {
        /* right horizontal scroll */ 
        WriteSSD1306(true, 0x26);
    }

    WriteSSD1306(true, 0x00);            // Dummy byte (dont change)
    WriteSSD1306(true, start_line_no);   // Start page address
    WriteSSD1306(true, 0x00);            // 5 frames interval
    WriteSSD1306(true, end_line_no);     // End page address
    WriteSSD1306(true, 0x00);            // Dummy byte (dont change)
    WriteSSD1306(true, 0xFF);            // Dummy byte (dont change)
    WriteSSD1306(true, 0x2F);            // activate scroll
}

static void InvertDisplaySSD1306(bool need_to_invert)
{
    if (need_to_invert)
    {
        WriteSSD1306(true, 0xA7); // Invert the display
    }  
    else
    {
        WriteSSD1306(true, 0xA6); // Normal display
    }
}

static void DeactivateScrollSSD1306(void)
{
    WriteSSD1306(true, 0x2E); // Deactivate scroll
}
