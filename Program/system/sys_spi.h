#ifndef __SYS_SPI_H__
#define __SYS_SPI_H__

#include <machine/_default_types.h>

#define SPI_FLASH_PORT  SPI0
#define TEST_NUMBER                 10 /* page numbers */

#define SPI_TX_DMA_CH           1
#define SPI_RX_DMA_CH           2

extern unsigned char SrcArray[256];
extern unsigned char DestArray[256];

void Init_PDMA_CH1_for_SPI0_TX(uint32_t u32SrcAddr);

#endif /* __SYS_SPI_H__ */