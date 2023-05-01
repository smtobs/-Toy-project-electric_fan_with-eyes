#include <linux/spi/spi.h>

#include "custom_spi.h" 

static struct spi_device *spi_device;

static bool InitSpi(struct spi_obj_t *spi);
static void DeInitSpi(struct spi_obj_t *spi);
static int WriteSpi(uint8_t data);

bool CreateSpiObj(struct spi_obj_t *spi, char *modalias, unsigned long max_speed, 
                    int bus, int chip_select, int mode)
{
    /* SPI Set */
    spi->info.modalias = modalias;
    spi->info.max_speed = max_speed;
    spi->info.bus = bus;
    spi->info.chip_select = chip_select;
    spi->info.mode = mode;

    /* Register for SPI Function */
    spi->Init    = InitSpi;
    spi->DeInit  = DeInitSpi;
    spi->Write   = WriteSpi;

    return true;
}

static bool InitSpi(struct spi_obj_t *spi)
{
    int     ret;
    struct  spi_master *master;
    struct spi_board_info spi_info = 
    {
        .modalias     = {&spi->info.modalias},
        .max_speed_hz = spi->info.max_speed,              // speed your device (slave) can handle
        .bus_num      = spi->info.bus,          // SPI 1
        .chip_select  = spi->info.chip_select,                    // Use 0 Chip select (GPIO 16)
        .mode         = spi->info.mode            // SPI mode 0
    };

    printk("modalias [%s]\n", spi_info.modalias);
    printk("max speed [%d]\n", spi_info.max_speed_hz);
    printk("bus num [%d]\n", spi_info.bus_num);
    printk("chip [%d]\n", spi_info.chip_select);
    printk("mode [%d]\n", spi_info.mode);

    master = spi_busnum_to_master(spi_info.bus_num);
    if (master == NULL)
    {
        printk("SPI Master not found.\n");
        return false;
    }
   
    // create a new slave device, given the master and device info
    spi_device = spi_new_device(master, &spi_info);
    if (spi_device == NULL) 
    {
        printk("FAILED to create slave.\n");
        return false;
    }
  
    // 8-bits in a word
    spi_device->bits_per_word = 8;

    // setup the SPI slave device
    ret = spi_setup(spi_device);
    if (ret)
    {
        printk("FAILED to setup slave.\n");
        spi_unregister_device(spi_device);
        return false;
    }
    return true;
}

static void DeInitSpi(struct spi_obj_t *spi)
{ 
#if (0)
    uint8_t index = spi->info.instance;

    if (index > MAX_SPI)
    {
        pr_info("DeInit SPI Instance Error.. \n");
        return;
    }

    if (spi_device[index])
    {
        spi_unregister_device(spi_device[index]);
        pr_info("SPI driver Unregistered\n");
    }
#else
    if (spi_device)
    {
        spi_unregister_device(spi_device);
    }
#endif
}

/****************************************************************************
 * Name: etx_spi_write
 *
 * Details : This function writes the 1-byte data to the slave device using SPI.
 ****************************************************************************/
static int WriteSpi(uint8_t data)
{
    int     ret = -1;
    uint8_t rx  = 0x00;
  
    if (spi_device)
    {    
        struct spi_transfer tr = 
        {
        .tx_buf = &data,
        .rx_buf = &rx,
        .len = 1,
        };

        spi_sync_transfer(spi_device, &tr, 1);
    }
 
    return ret;
}
