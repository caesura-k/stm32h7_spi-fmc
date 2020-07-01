#include <stdio.h>
#include <string.h>

#include "w25qxx.h"

#include "delay.h"
#include "usart.h" 
#include "spi_io.h"



#if USE_SPI_IO
u16 w25qxx_readID(void)
{
	u16 id=0;
	SPI_CS(0);
	SPI_WRITE_READ_BYTE(W25X_ManufactDeviceID);
	SPI_WRITE_READ_BYTE(dummy);
	SPI_WRITE_READ_BYTE(dummy);
	SPI_WRITE_READ_BYTE(0x00);
	id=SPI_WRITE_READ_BYTE(dummy);//EFh,表示winbone生产
	id=id<<8;
	id|=SPI_WRITE_READ_BYTE(dummy);//17h,表示w25q128系列
	SPI_CS(1);
	printf("w25qxx id:%4x  \r\n",id);
	return id;
}
#else
u16 w25qxx_readID(void)
{
	u16 id=0;
	u8 sendbuff[8]={W25X_ManufactDeviceID,dummy,dummy,0x00,dummy,dummy};
	u8 revbuff[8];
	u16 len = 6;
	u32 timeout = 5000;
	memset(revbuff,dummy,sizeof(revbuff));
	
	SPI_CS(0);
	HAL_SPI_TransmitReceive(hspi1,sendbuff,revbuff,len,timeout);
	SPI_CS(1);

	id=revbuff[4]<<8;
	id=id|revbuff[5];
	printf("spi1 w25qxx id:%4x  \r\n",id);
	return id;
}
#endif


#if USE_SPI_IO
//每个扇区4096Bytes，一共有4096个sector；
void w25qxx_eraseSector( u16 sector)
{
	u32 address = sector*4096;
	 w25qxx_writeEnable( );
	 w25qxx_waitBusy();
	 SPI_CS(0);
	 SPI_WRITE_READ_BYTE(W25X_SectorErase);
	 SPI_WRITE_READ_BYTE(address>>16);	//A23-A16
	 SPI_WRITE_READ_BYTE(address>>8);	//A15-A8
	 SPI_WRITE_READ_BYTE(address);		//A7-A0
	 SPI_CS(1);
	 w25qxx_waitBusy();
	 printf("erase sector done...\r\n");
}
#else
void w25qxx_eraseSector( u16 sector)
{
	u32 address = sector*4096;
	u8 sendbuff[4];
	u8 revbuff[4]={dummy,dummy,dummy,dummy};
	u16 len =4;
	u32 timeout =5000;
	
	u8 *psend = sendbuff;
	 *psend = W25X_SectorErase;
	 *(psend+1) = address>>16;
	 *(psend+2) = address>>8;
	 *(psend+3) = address;

	w25qxx_writeEnable( );
	w25qxx_waitBusy();
	SPI_CS(0);
	HAL_SPI_TransmitReceive(hspi1,sendbuff,revbuff,len,timeout);
	SPI_CS(1);
	w25qxx_waitBusy();
	printf("spi1 erase sector done...\r\n");
}
#endif


#if USE_SPI_IO
//每个sector为4096bytes，寻址的最小单位是bytes；
void w25qxx_writePage(u8 *buff,u8 len,u32 address)
{
	u8 i =0;
	w25qxx_writeEnable( );
	w25qxx_waitBusy();
	SPI_CS(0);
	SPI_WRITE_READ_BYTE(W25X_PageProgram);
	SPI_WRITE_READ_BYTE(address>>16);
	SPI_WRITE_READ_BYTE(address>>8);
	SPI_WRITE_READ_BYTE(address);
	for(i=0;i<len;i++)
		SPI_WRITE_READ_BYTE(buff[i]);
	SPI_CS(1);
	w25qxx_waitBusy();
	printf("write page done...\r\n");
}
#else
void w25qxx_writePage(u8 *buff,u8 len,u32 address)
{
	u8 sendbuff[4];
	u8 revbuff[4]={dummy,dummy,dummy,dummy};
	u16 sendlen =4;
	u32 timeout =5000;
	u8 dummybuff[256];

	
	u8 *psend = sendbuff;
	 *psend = W25X_PageProgram;
	 *(psend+1) = address>>16;
	 *(psend+2) = address>>8;
	 *(psend+3) = address;
	memset(dummybuff,dummy,sizeof(dummybuff));
	 
	w25qxx_writeEnable( );
	w25qxx_waitBusy();
	SPI_CS(0);
	HAL_SPI_TransmitReceive(hspi1,sendbuff,revbuff,sendlen,timeout);
	HAL_SPI_TransmitReceive(hspi1,buff,dummybuff,len,timeout);
	SPI_CS(1);
	w25qxx_waitBusy();
	printf("spi1 write page done...\r\n");
}
#endif

#if USE_SPI_IO
void w25qxx_readData(u8 *buff,u32 len,u32 address)
{
	u32 i =0;
	SPI_CS(0);
	SPI_WRITE_READ_BYTE(W25X_ReadData);
	SPI_WRITE_READ_BYTE(address>>16);
	SPI_WRITE_READ_BYTE(address>>8);
	SPI_WRITE_READ_BYTE(address);
	for(i=0;i<len;i++)
		buff[i]=SPI_WRITE_READ_BYTE(dummy);
	SPI_CS(1);
	printf("read data done...\r\n");
}
#else
void w25qxx_readData(u8 *buff,u32 len,u32 address)
{
	u8 sendbuff[4];
	u8 revbuff[4]={dummy,dummy,dummy,dummy};
	u16 sendlen =4;
	u32 timeout =5000;
	u8 dummybuff[256];

	u8 *psend = sendbuff;
	 *psend = W25X_ReadData;
	 *(psend+1) = address>>16;
	 *(psend+2) = address>>8;
	 *(psend+3) = address;
	memset(dummybuff,dummy,sizeof(dummybuff));
	
	SPI_CS(0);
	HAL_SPI_TransmitReceive(hspi1,sendbuff,revbuff,sendlen,timeout);
	HAL_SPI_TransmitReceive(hspi1,dummybuff,buff,len,timeout);
	SPI_CS(1);
	printf("spi1 read data done...\r\n");
}
#endif

#if USE_SPI_IO
void w25qxx_writeEnable(void)
{
	SPI_CS(0);
	SPI_WRITE_READ_BYTE(W25X_WriteEnable);//置位WEL,write enable latch;
	SPI_CS(1);
	printf("write enable done...\r\n");
}
#else
void w25qxx_writeEnable(void)
{
	u8 sendbuff[1]={W25X_WriteEnable};
	u8 revbuff[1]={0xff};
	u16 len =1;
	u32 timeout = 5000;
	SPI_CS(0);
	HAL_SPI_TransmitReceive(hspi1,sendbuff,revbuff,len,timeout);
	SPI_CS(1);
	printf("spi1 write enable done...\r\n");
}
#endif

#if USE_SPI_IO
void w25qxx_waitBusy(void)
{	
	u8 sreg1 =0;
	SPI_CS(0);
	SPI_WRITE_READ_BYTE(W25X_ReadStatusReg1);
	do{
		sreg1=SPI_WRITE_READ_BYTE(dummy);	
	}while(sreg1&0x01);
	SPI_CS(1);
	printf("wait busy done...\r\n");
}
#else
void w25qxx_waitBusy(void)
{
	u8 sendbuff[1]={W25X_ReadStatusReg1};
	u8 revbuff[1]={0xff};
	u8 dummybuff[1]={0xff};
	u16 len =1;
	u32 timeout =5000;
	SPI_CS(0);
	HAL_SPI_TransmitReceive(hspi1,sendbuff,revbuff,len,timeout);
	do{
		HAL_SPI_TransmitReceive(hspi1,dummybuff,revbuff,len,timeout);
	}while(revbuff[0]&0x01);
	SPI_CS(1);
	printf("spi1 wait busy done...\r\n");
}
#endif

void spi_test(void)
{
	u8 sendbuff[16];
	u8 revbuff[16];
	int i;
	w25qxx_readID();
	
	memset(revbuff,0,sizeof(revbuff));
	for(i=0;i<16;i++)
		sendbuff[i]=i;
		
	w25qxx_eraseSector(0);
	delay_ms(500);
	w25qxx_writePage(sendbuff,16,0);
	delay_ms(500);
	w25qxx_readData(revbuff,16,0);
	for(i=0;i<16;i++)
		printf("revbuff[%2d]:%2x \r\n",i,revbuff[i]);
}

