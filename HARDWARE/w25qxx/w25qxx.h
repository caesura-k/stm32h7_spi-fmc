#ifndef __W25QXX_H
#define __W25QXX_H
#include "sys.h"

u16 w25qxx_readID(void);
void w25qxx_eraseSector( u16 sector);
void w25qxx_writePage(u8 *buff,u8 len,u32 address);
void w25qxx_readData(u8 *buff,u32 len,u32 address);
void w25qxx_writeEnable(void);
void w25qxx_waitBusy(void);
void spi_test(void);


#define USE_SPI_IO	0x00  //为0：不使用模拟io口功能；非0：使用模拟IO口功能；
extern SPI_HandleTypeDef *hspi1;



#define W25Q128	0XEF17
#define W25Q256 0XEF18

extern u16 W25QXX_TYPE;	 
 
//指令表
#define dummy					0xff
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9
#define W25X_SetReadParam		0xC0 
#define W25X_EnterQPIMode       0x38
#define W25X_ExitQPIMode        0xFF

#endif
