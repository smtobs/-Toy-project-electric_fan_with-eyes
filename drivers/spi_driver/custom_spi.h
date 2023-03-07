#ifndef _CUSTOM_SPI_H_
#define _CUSTOM_SPI_H_

#include "util.h"

#define SPI_MODE0     (0)
#define SPI_MODE1     (1)
#define SPI_MODE2     (2)

#define MAX_SPI       (3)
#define MAX_SPEED_HZ  (4000000)

typedef struct spi_info_t
{
    uint8_t instance;
    int bus;
    int chip_select;
    int mode;
    unsigned long max_speed;
    char *modalias;
}spi_info_t;

typedef struct spi_obj_t
{
    spi_info_t info;
    bool (*Init)(struct spi_obj_t *this);
    void (*DeInit)(struct spi_obj_t *this);
    int (*Write)(uint8_t data);
}spi_obj_t;

bool CreateSpiObj(struct spi_obj_t *spi, char *modalias, unsigned long max_speed, 
                    int bus, int chip_select, int mode);
#endif
