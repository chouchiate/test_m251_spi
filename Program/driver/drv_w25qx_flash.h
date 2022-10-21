#ifndef __W25QX_FLASH_H__
#define __W25QX_FLASH_H__

#define MAX_W25QX_FLASH_OPS 5

typedef enum {
    _SPI_ASYNC_READ = 0,
    _SPI_ASYNC_WRITE = 1,
    _SPI_ASYNC_ERASE = 2,
    _SPI_ASYNC_COPY = 3,

} W25QX_FLASH_OPS;


void mapSpiFlashFn(void);

void asyncReadSpiFlash(void);
void asyncWriteSpiFlash(void);
void asyncEraseSpiFlash(void);
void asyncCopySpiFlash(void);

unsigned int SpiFlash_ReadMidDid_Sync(void);
void SpiFlash_ChipErase(void);
unsigned int SpiFlash_ReadStatusReg1(void);
unsigned int SpiFlash_ReadStatusReg2(void);
void SpiFlash_WaitReady(void);
void SpiFlash_PDMA_PageProgram(unsigned int u32SrcAddr, unsigned int StartAddress);
void SpiFlash_PDMA_ReadData(unsigned int u32DestAddr, unsigned int StartAddress);



#endif /* __W25QX_FLASH_H__ */