/**
 * @file drv_w25qx_flash.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-10-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../includes.h"


void (*spiFlashAsync[MAX_W25QX_FLASH_OPS])(void);

void mapSpiFlashFn(void)
{
    spiFlashAsync[_SPI_ASYNC_READ] = asyncReadSpiFlash;
    spiFlashAsync[_SPI_ASYNC_WRITE] = asyncWriteSpiFlash;
    spiFlashAsync[_SPI_ASYNC_ERASE] = asyncEraseSpiFlash;
    spiFlashAsync[_SPI_ASYNC_COPY] = asyncCopySpiFlash;


}

void asyncReadSpiFlash(void)
{

}

void asyncWriteSpiFlash(void)
{

}

void asyncEraseSpiFlash(void)
{

}

void asyncCopySpiFlash(void)
{

}

/**
 * @brief
 * For W25Q16BV, Manufacturer ID: 0xEF; Device ID: 0x14
 * For W25Q32BV, Manufacturer ID: 0xEF; Device ID: 0x15
 * @return unsigned int
 */
unsigned int SpiFlash_ReadMidDid_Sync(void)
{
    unsigned int au32SourceData;
    unsigned int au32DestinationData;

    // /CS: active
    SPI_SET_SS_LOW(SPI_FLASH_PORT);
    // configure transaction length as 8 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 8);
    // send Command: 0x90, Read Manufacturer/Device ID
    au32SourceData = 0x90;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // configure transaction length as 24 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 24);
    // send 24-bit '0', dummy
    au32SourceData = 0x0;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // configure transaction length as 16 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 16);
    // receive
    SPI_ClearRxFIFO(SPI_FLASH_PORT);
    au32SourceData = 0x0;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // /CS: de-active
    SPI_SET_SS_HIGH(SPI_FLASH_PORT);
    // dump Rx register
    au32DestinationData = SPI_READ_RX(SPI_FLASH_PORT);

    return (au32DestinationData & 0xffff);

}

/**
 * @brief
 *
 */
void SpiFlash_ChipErase(void)
{
    unsigned int au32SourceData;

    // /CS: active
    SPI_SET_SS_LOW(SPI_FLASH_PORT);
    // configure transaction length as 8 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 8);
    // send Command: 0x06, Write enable
    au32SourceData = 0x06;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // /CS: de-active
    SPI_SET_SS_HIGH(SPI_FLASH_PORT);

    // /CS: active
    SPI_SET_SS_LOW(SPI_FLASH_PORT);
    // send Command: 0xC7, Chip Erase
    au32SourceData = 0xc7;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // /CS: de-active
    SPI_SET_SS_HIGH(SPI_FLASH_PORT);

}


/**
 * @brief
 *
 * @return unsigned int
 */
unsigned int SpiFlash_ReadStatusReg1(void)
{
    unsigned int au32SourceData;
    unsigned int au32DestinationData;

    // /CS: active
    SPI_SET_SS_LOW(SPI_FLASH_PORT);
    SPI_ClearRxFIFO(SPI_FLASH_PORT);
    // configure transaction length as 16 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 16);
    // send Command: 0x05, Read status register 1
    au32SourceData = 0x0500;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // /CS: de-active
    SPI_SET_SS_HIGH(SPI_FLASH_PORT);
    // dump Rx register
    au32DestinationData = SPI_READ_RX(SPI_FLASH_PORT);

    return (au32DestinationData & 0xFF);

}

/**
 * @brief
 *
 * @return unsigned int
 */
unsigned int SpiFlash_ReadStatusReg2(void)
{
    unsigned int au32SourceData;
    unsigned int au32DestinationData;

    // /CS: active
    SPI_SET_SS_LOW(SPI_FLASH_PORT);
    SPI_ClearRxFIFO(SPI_FLASH_PORT);
    // configure transaction length as 16 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 16);
    // send Command: 0x035, Read status register 2
    au32SourceData = 0x3500;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // /CS: de-active
    SPI_SET_SS_HIGH(SPI_FLASH_PORT);
    // dump Rx register
    au32DestinationData = SPI_READ_RX(SPI_FLASH_PORT);

    return (au32DestinationData & 0xFF);

}

/**
 * @brief
 *
 */
void SpiFlash_WaitReady(void)
{
    unsigned int ReturnValue;

    do
    {
        ReturnValue = SpiFlash_ReadStatusReg1();
        ReturnValue = ReturnValue & 1;
    } while (ReturnValue != 0); // check the BUSY bit

}

/**
 * @brief
 *
 * @param u32SrcAddr
 * @param StartAddress
 */
void SpiFlash_PDMA_PageProgram(unsigned int u32SrcAddr, unsigned int StartAddress)
{
    unsigned int au32SourceData;
    unsigned int u32RegValue, u32Abort;

    // /CS: active
    SPI_SET_SS_LOW(SPI_FLASH_PORT);
    // configure transaction length as 8 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 8);
    // send Command: 0x06, Write enable
    au32SourceData = 0x06;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // /CS: de-active
    SPI_SET_SS_HIGH(SPI_FLASH_PORT);

    // /CS: active
    SPI_SET_SS_LOW(SPI_FLASH_PORT);
    // send Command: 0x02, Page program
    au32SourceData = 0x02;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // configure transaction length as 24 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 24);
    // send 24-bit start address
    au32SourceData = StartAddress;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // configure transaction length as 8 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 8);
    SPI_ClearTxFIFO(SPI_FLASH_PORT);

    // enable SPI PDMA
    /* Initial PDMA Channel */
    Init_PDMA_CH1_for_SPI0_TX((uint32_t)u32SrcAddr);
    SPI_TRIGGER_TX_PDMA(SPI_FLASH_PORT);

    while (1)
    {
        /* Get interrupt status */
        u32RegValue = PDMA_GET_INT_STATUS(PDMA);

        /* Check the DMA transfer done interrupt flag */
        if (u32RegValue & PDMA_INTSTS_TDIF_Msk)
        {
            /* Check the PDMA transfer done interrupt flags */
            if ((PDMA_GET_TD_STS(PDMA) & (1 << SPI_TX_DMA_CH)) == (1 << SPI_TX_DMA_CH))
            {
                /* Clear the DMA transfer done flags */
                PDMA_CLR_TD_FLAG(PDMA, 1 << SPI_TX_DMA_CH);
                /* Disable SPI PDMA TX function */
                SPI_DISABLE_TX_PDMA(SPI_FLASH_PORT);
                break;
            }

            /* Check the DMA transfer abort interrupt flag */
            if (u32RegValue & PDMA_INTSTS_ABTIF_Msk)
            {
                /* Get the target abort flag */
                u32Abort = PDMA_GET_ABORT_STS(PDMA);
                /* Clear the target abort flag */
                PDMA_CLR_ABORT_FLAG(PDMA, u32Abort);
                break;
            }
        }
    }

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // /CS: de-active
    SPI_SET_SS_HIGH(SPI_FLASH_PORT);

}

/**
 * @brief
 *
 * @param u32DestAddr
 * @param StartAddress
 */
void SpiFlash_PDMA_ReadData(unsigned int u32DestAddr, unsigned int StartAddress)
{
    unsigned int au32SourceData;
    unsigned int u32RegValue, u32Abort;

    // /CS: active
    SPI_SET_SS_LOW(SPI_FLASH_PORT);
    // configure transaction length as 8 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 8);
    // send Command: 0x03, Read data
    au32SourceData = 0x03;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // configure transaction length as 24 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 24);
    // send 24-bit start address
    au32SourceData = StartAddress;
    SPI_WRITE_TX(SPI_FLASH_PORT, au32SourceData);

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // configure transaction length as 8 bits
    SPI_SET_DATA_WIDTH(SPI_FLASH_PORT, 8);
    SPI_ClearRxFIFO(SPI_FLASH_PORT);

    // enable SPI PDMA
    Init_PDMA_CH2_for_SPI0_RX((uint32_t)u32DestAddr);
    SPI_TRIGGER_RX_PDMA(SPI_FLASH_PORT);

    while (1)
    {
        /* Get interrupt status */
        u32RegValue = PDMA_GET_INT_STATUS(PDMA);

        /* Check the DMA transfer done interrupt flag */
        if (u32RegValue & PDMA_INTSTS_TDIF_Msk)
        {
            /* Check the PDMA transfer done interrupt flags */
            if ((PDMA_GET_TD_STS(PDMA) & (1 << SPI_RX_DMA_CH)) == (1 << SPI_RX_DMA_CH))
            {
                /* Clear the DMA transfer done flags */
                PDMA_CLR_TD_FLAG(PDMA, 1 << SPI_RX_DMA_CH);
                /* Disable SPI PDMA RX function */
                SPI_DISABLE_RX_PDMA(SPI_FLASH_PORT);
                break;
            }

            /* Check the DMA transfer abort interrupt flag */
            if (u32RegValue & PDMA_INTSTS_ABTIF_Msk)
            {
                /* Get the target abort flag */
                u32Abort = PDMA_GET_ABORT_STS(PDMA);
                /* Clear the target abort flag */
                PDMA_CLR_ABORT_FLAG(PDMA, u32Abort);
                break;
            }
        }
    }

    // wait
    while (SPI_IS_BUSY(SPI_FLASH_PORT));

    // /CS: de-active
    SPI_SET_SS_HIGH(SPI_FLASH_PORT);
}

