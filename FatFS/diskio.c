/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32f4xx.h"
#include "sdio_sdcard.h"


#include "usbh_usr.h"




#define BLOCK_SIZE            512 /* Block Size in Bytes */



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
    BYTE drv				/* Physical drive nmuber (0..) */
)
{
    SD_Error  Status;

    switch (drv)
    {

        /*-------------------------- SD Init ----------------------------- */
    case MMC :
        //result = MMC_disk_initialize();

        Status = SD_Init();
        if (Status!=SD_OK )
        {
            return STA_NOINIT;
        }
        else
        {
#ifdef SD_DMA_MODE
            SD_NVIC_Configuration();
#endif
            return RES_OK;
        }
    case USB :


        if(USB_disk_status())return RES_OK;
        else return STA_NOINIT;
    }
    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
    BYTE drv		/* Physical drive nmuber (0..) */
)
{

    return RES_OK;

}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
u32 diskreaderror = 0;
DRESULT disk_read (
    BYTE drv,		/* Physical drive nmuber (0..) */
    BYTE *buff,		/* Data buffer to store read data */
    DWORD sector,	/* Sector address (LBA) */
    BYTE count		/* Number of sectors to read (1..255) */
)
{
    int result;

    switch (drv)
    {

    case MMC :
        if (count > 1)
        {
            SD_ReadMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);

            /* Check if the Transfer is finished */
            SD_WaitReadOperation();  //循环查询dma传输是否结束

            /* Wait until end of DMA transfer */
            while(SD_GetStatus() != SD_TRANSFER_OK)
            {
                diskreaderror++;
                if(diskreaderror>=0xffffff)
                {
                    diskreaderror = 0;
                    break;
                }
            }

        }
        else
        {

            SD_ReadBlock(buff, sector*BLOCK_SIZE, BLOCK_SIZE);

            /* Check if the Transfer is finished */
            SD_WaitReadOperation();  //循环查询dma传输是否结束

            /* Wait until end of DMA transfer */
            while(SD_GetStatus() != SD_TRANSFER_OK)
            {
                diskreaderror++;
                if(diskreaderror>=0xffffff)
                {
                    diskreaderror = 0;
                    break;
                }
            }

        }
        return RES_OK;
    case USB :


        result = USB_disk_read(buff,sector,count);
        if(result == 0x00)
            return RES_OK;
        else
            return RES_ERROR;
    }
    return RES_PARERR;

}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
    BYTE drv,			/* Physical drive nmuber (0..) */
    const BYTE *buff,	/* Data to be written */
    DWORD sector,		/* Sector address (LBA) */
    BYTE count			/* Number of sectors to write (1..255) */
)
{
    int result;

    switch (drv)
    {


    case MMC :

        if (count > 1)
        {
            SD_WriteMultiBlocks((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);

            /* Check if the Transfer is finished */
            SD_WaitWriteOperation();	   //等待dma传输结束
            while(SD_GetStatus() != SD_TRANSFER_OK) //等待sdio到sd卡传输结束
            {
                diskreaderror++;
                if(diskreaderror>=0xffffff)
                {
                    diskreaderror = 0;
                    break;
                }
            }
        }
        else
        {
            SD_WriteBlock((uint8_t *)buff,sector*BLOCK_SIZE, BLOCK_SIZE);

            /* Check if the Transfer is finished */
            SD_WaitWriteOperation();	   //等待dma传输结束
            while(SD_GetStatus() != SD_TRANSFER_OK) //等待sdio到sd卡传输结束
            {
                diskreaderror++;
                if(diskreaderror>=0xffffff)
                {
                    diskreaderror = 0;
                    break;
                }
            }
        }
        return RES_OK;
    case USB :

        result = USB_disk_write((u8*)buff,sector,count);
        if(result == 0x00)
            return RES_OK;
        else
            return RES_ERROR;
    }
    return RES_PARERR;
}
#endif /* _READONLY */




/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
    BYTE drv,		/* Physical drive nmuber (0..) */
    BYTE ctrl,		/* Control code */
    void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT res;
    switch (drv)
    {


    case MMC :

        return RES_OK;
    case USB :
        switch(ctrl)
        {
        case CTRL_SYNC:
            res = RES_OK;
            break;
        case GET_SECTOR_SIZE:
            *(WORD*)buff=512;
            res = RES_OK;
            break;
        case GET_BLOCK_SIZE:
            *(WORD*)buff=512;
            res = RES_OK;
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)buff=USBH_MSC_Param.MSCapacity;
            res = RES_OK;
            break;
        default:
            res = RES_PARERR;
            break;
        }
        return res;
    }
    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/
DWORD get_fattime(void)
{

    return 0;

}
