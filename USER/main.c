#include <stdio.h>
#include <string.h>

#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h"

#include "fmc.h"

#include "spi_io.h"
#include "w25qxx.h"
#include "spi1.h"

SPI_HandleTypeDef hspi;
SPI_HandleTypeDef *hspi1 = &hspi;

int main(void)
{
	Cache_Enable();                 //打开L1-Cache
	HAL_Init();				        //初始化HAL库
	Stm32_Clock_Init(160,5,2,4);    //设置时钟,400Mhz 
	delay_init(400);				//延时初始化
	uart_init(115200);				//串口1初始化				
	LED_Init();
	
	fmc_init();			//fmc初始化
	//fmc_test();		//fmc测试
	
	//SPI_Init();		//io模拟spi初始化
	//spi_test();		//spi测试
	
	spi1_init(hspi1);	//spi1外设初始化
	//spi_test();		//spi1外设测试
		
	
	while(1)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);	//PB0置0	
		delay_ms(500);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);	//PB0置1	
		delay_ms(500);
		fmc_test();
		spi_test();
	}
}

