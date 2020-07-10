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
	Cache_Enable();                 //��L1-Cache
	HAL_Init();				        //��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4);    //����ʱ��,400Mhz 
	delay_init(400);				//��ʱ��ʼ��
	uart_init(115200);				//����1��ʼ��				
	LED_Init();
	
	fmc_init();			//fmc��ʼ��
	//fmc_test();		//fmc����
	
	//SPI_Init();		//ioģ��spi��ʼ��
	//spi_test();		//spi����
	
	spi1_init(hspi1);	//spi1�����ʼ��
	//spi_test();		//spi1�������
		
	
	while(1)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);	//PB0��0	
		delay_ms(500);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);	//PB0��1	
		delay_ms(500);
		fmc_test();
		spi_test();
	}
}

