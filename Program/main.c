/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-10-21
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "./includes.h"

int main(void)
{
    unsigned int u32ByteCount;
    unsigned int u32PageNumber;
    unsigned int u32ProgramFlashAddress = 0;
    unsigned int u32VerifyFlashAddress = 0;
    unsigned int MidDid;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O. */
    SYS_Init();

    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);

    /* Configure SPI_FLASH_PORT as a master, MSB first, 8-bit transaction, SPI Mode-0 timing, clock is 12MHz */
    SPI_Open(SPI_FLASH_PORT, SPI_MASTER, SPI_MODE_0, 8, 12000000);

    /* Disable auto SS function, control SS signal manually. */
    SPI_DisableAutoSS(SPI_FLASH_PORT);
    SPI_SET_SS_HIGH(SPI_FLASH_PORT);

    printf("\n+------------------------------------------------------------------------+\n");
    printf("|                      SPI PDMA Read Write Sample with SPI Flash          |\n");
    printf("+------------------------------------------------------------------------+\n");
    /* Read MID & DID */
    MidDid = SpiFlash_ReadMidDid();
    printf("\nMID and DID = %x", MidDid);

    /* Erase SPI Flash */
    SpiFlash_ChipErase();
    printf("\nFlash Erasing... ");

    /* Wait ready */
    SpiFlash_WaitReady();
    printf("Done!");

    /* Fill the Source Data and clear Destination Data Buffer */
    for (u32ByteCount = 0; u32ByteCount < 256; u32ByteCount++)
    {
        SrcArray[u32ByteCount] = u32ByteCount;
        DestArray[u32ByteCount] = 0;
    }

    u32ProgramFlashAddress = 0;
    u32VerifyFlashAddress = 0;

    for (u32PageNumber = 0; u32PageNumber < TEST_NUMBER; u32PageNumber++)
    {
        printf("\n\nTest Page Number = %d", u32PageNumber);

        CLK_SysTickDelay(200000);

        /*=== Program SPI Flash ===*/
        printf("\nFlash Programming... ");

        /* Page Program */
        SpiFlash_PDMA_PageProgram((uint32_t)SrcArray, u32ProgramFlashAddress);
        SpiFlash_WaitReady();
        u32ProgramFlashAddress += 0x100;
        printf("Done!");

        /*=== Read Back and Compare Data ===*/
        printf("\nFlash Verifying... ");

        /* Page Read */
        SpiFlash_PDMA_ReadData((uint32_t)DestArray, u32VerifyFlashAddress);
        u32VerifyFlashAddress += 0x100;

        for (u32ByteCount = 0; u32ByteCount < 256; u32ByteCount++)
        {
            if (DestArray[u32ByteCount] != u32ByteCount)
            {
                /* Error */
                printf("SPI Flash R/W Fail!");

                while (1);
            }
        }

        /* Clear Destination Data Buffer */
        for (u32ByteCount = 0; u32ByteCount < 256; u32ByteCount++)
            DestArray[u32ByteCount] = 0;

        printf("Done!");
    }

    printf("\n\nSPI Flash Test Ok!");
    printf("\n\n");
		while(1);
}