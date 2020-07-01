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
	//int usartLen;

	Cache_Enable();                 //��L1-Cache
	HAL_Init();				        //��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4);    //����ʱ��,400Mhz 
	delay_init(400);				//��ʱ��ʼ��
	uart_init(115200);				//����1��ʼ��				
	LED_Init();	
	//FMC_init();
	//SPI_Init();
	//spi_test();
	spi1_init(hspi1);
	spi_test();

	
	
	while(1)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);	//PB0��0	
		delay_ms(500);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);	//PB0��1	
		delay_ms(500);


	
	
		//if(USART_RX_STA&0x8000)
		//{					   
		//	usartLen=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���	
		//	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART_RX_BUF,usartLen,1000);	//���ͽ��յ�������
		//	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
		//	
		//	USART_RX_STA=0;
		//}

		#if 0
		FPGA_WRITE(1,1);
		HAL_Delay(500);
		FPGA_WRITE(1,0);
		printf("Read Data From FPGA:%x \r\n",FPGA_READ(3));

		FPGA_WRITE(1,2);
		HAL_Delay(500);
		FPGA_WRITE(1,0);
		HAL_Delay(500);
		#endif

		#if 0
		for(i=0;i<10;i++)
		{
			FPGA_WRITE(i,i);
			printf("fpga write fmcbuf[%d]: %d \r\n",i,fmcbuf[i]);
		}
		for(i=0;i<100000;i++);
		
		for(i=0;i<10;i++)
		{
			fmcbuf[i]=FPGA_READ(i);
			printf("fpga read fmcbuf[%d]: %d \r\n",i,fmcbuf[i]);
		}
		#endif
	}
}

