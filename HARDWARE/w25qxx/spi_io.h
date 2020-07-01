#ifndef __IO_SPI_H
#define __IO_SPI_H
#include "stm32h7xx_hal.h"


#define _SPI_DELAY_			1		//数值太小可能造成通讯异常，当读不到数据时，可以加大这个数值


#define _SPI_CPOL   		0		//选择CLK在空闲时的电平状态，0：空闲时为低电平，1：空闲时为高电平
#define _SPI_CPHA   		0		//选择SPI数据在第几个边沿采集数据，0：在第一个边沿采集数据，1：在第二个边沿采集数据

#define SPI_CS(x)          (x ?  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET) )       
#define SPI_CLK(x)         (x ?  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET) )       
#define SPI_MOSI(x)        (x ?  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET) )       
#define SPI_MISO           HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)      


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//函数声明
extern void SPI_Init(void);
extern void SPI_DELAY(uint16_t del);
extern uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT);


#endif


