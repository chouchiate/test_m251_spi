/**
 * @file sys_spi.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-10-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "../includes.h"

unsigned char   SrcArray[256];
unsigned char DestArray[256];

#define PDMA_TEST_LENGTH                 256


void Init_PDMA_CH1_for_SPI0_TX(uint32_t u32SrcAddr)
{
    /* Enable PDMA channels */
    PDMA_Open(PDMA, (1 << SPI_TX_DMA_CH));

    /* SPI PDMA TX channel configuration */
    /* Set transfer width (8 bits) and transfer count */
    PDMA_SetTransferCnt(PDMA, SPI_TX_DMA_CH, PDMA_WIDTH_8, PDMA_TEST_LENGTH);
    /* Set source/destination address and attributes */
    PDMA_SetTransferAddr(PDMA, SPI_TX_DMA_CH, (uint32_t)u32SrcAddr, PDMA_SAR_INC, (uint32_t)&SPI0->TX, PDMA_DAR_FIX);
    /* Set request source; set basic mode. */
    PDMA_SetTransferMode(PDMA, SPI_TX_DMA_CH, PDMA_SPI0_TX, FALSE, 0);
    /* Single request type */
    PDMA_SetBurstType(PDMA, SPI_TX_DMA_CH, PDMA_REQ_SINGLE, PDMA_BURST_128);
    /* Disable table interrupt */
    PDMA->DSCT[SPI_TX_DMA_CH].CTL |= PDMA_DSCT_CTL_TBINTDIS_Msk;
}

void Init_PDMA_CH2_for_SPI0_RX(uint32_t u32DstAddr)
{
    /* Enable PDMA channels */
    PDMA_Open(PDMA, (1 << SPI_RX_DMA_CH));

    /* SPI PDMA RX channel configuration */
    /* Set transfer width (8 bits) and transfer count */
    PDMA_SetTransferCnt(PDMA, SPI_RX_DMA_CH, PDMA_WIDTH_8, PDMA_TEST_LENGTH);
    /* Set source/destination address and attributes */
    PDMA_SetTransferAddr(PDMA, SPI_RX_DMA_CH, (uint32_t)&SPI0->RX, PDMA_SAR_FIX, (uint32_t)u32DstAddr, PDMA_DAR_INC);
    /* Set request source; set basic mode. */
    PDMA_SetTransferMode(PDMA, SPI_RX_DMA_CH, PDMA_SPI0_RX, FALSE, 0);
    /* Single request type */
    PDMA_SetBurstType(PDMA, SPI_RX_DMA_CH, PDMA_REQ_SINGLE, PDMA_BURST_128);
    /* Disable table interrupt */
    PDMA->DSCT[SPI_RX_DMA_CH].CTL |= PDMA_DSCT_CTL_TBINTDIS_Msk;
}

