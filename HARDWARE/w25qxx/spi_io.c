#include "spi_io.h"


/*******************************************************************************
函数名称        ：SPI_Init
函数参数        ：void
函数返回值      ：void
函数说明        ：SPI初始化函数。
                
*******************************************************************************/
void SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
    
    //PB3:SCK、PB6:/CS
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    //PB4:MISO 
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);   

	//PD7:MOSI
	GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    SPI_CS(1);
    SPI_MOSI(0);
  
	#if (_SPI_CPOL == 0)    
    	SPI_CLK(0);//空闲时为低电平     
	#else
    	SPI_CLK(1);   
	#endif

	HAL_Delay(100);//上电延时一段时间
}

/*******************************************************************************
函数名称        ：SPI_DELAY
函数参数        ：延时参数
函数返回值      ：void
函数说明        ：NOP延时                
*******************************************************************************/
void SPI_DELAY(uint16_t del)
{
	uint8_t i;
	for(i=0;i<del;i++)
	{
        __NOP();    
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
}

/*******************************************************************************
函数名称        ：SPI_WRITE_READ_BYTE
函数参数        ：SPI发送的数据
函数返回值      ：SPI读取的数据
函数说明        ：通过_SPI_CPOL和_SPI_CPHA宏来区分SPI 4种不同的通信模式         
*******************************************************************************/
#if (_SPI_CPOL == 0)&&(_SPI_CPHA == 0)

uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT)
{
    uint8_t i;
    uint8_t RX_DAT = 0;        
    for(i = 0;i < 8;i ++)
    {
        SPI_CLK(0);
        
        if(TX_DAT & 0x80)
        {
            SPI_MOSI(1);    
        }
        else
        {
            SPI_MOSI(0);       
        }     
        TX_DAT <<= 1;    
        
        SPI_DELAY(_SPI_DELAY_);
                
        SPI_CLK(1);
        
        RX_DAT <<= 1;
    
        if(SPI_MISO != 0)
        {
            RX_DAT |= 0x01;    
        }   
		
        SPI_DELAY(_SPI_DELAY_);			
    }
    
    SPI_CLK(0);//空闲时SCK为低电平
    
    return RX_DAT;
}

#elif (_SPI_CPOL == 0)&&(_SPI_CPHA == 1)
    
uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT)
{
    uint8_t i;
    uint8_t RX_DAT = 0;    
       
    for(i = 0;i < 8;i ++)
    {
        SPI_CLK(1);
        
        if(TX_DAT & 0x80)
        {
            SPI_MOSI(1);    
        }
        else
        {
            SPI_MOSI(0);       
        }    

        TX_DAT <<= 1;    
        
        SPI_DELAY(_SPI_DELAY_);
        
        SPI_CLK(0);

        RX_DAT <<= 1;
    
        if(SPI_MISO != 0)
        {
            RX_DAT |= 0x01;    
        }
		
        SPI_DELAY(_SPI_DELAY_);		
		
    }
    
    return RX_DAT;
}

#elif (_SPI_CPOL == 1)&&(_SPI_CPHA == 0)

uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT)
{
    uint8_t i;
    uint8_t RX_DAT = 0;    
       
    for(i = 0;i < 8;i ++)
    {
        SPI_CLK(1);
        
        if(TX_DAT & 0x80)
        {
            SPI_MOSI(1);    
        }
        else
        {
            SPI_MOSI(0);       
        }    

        TX_DAT <<= 1;    
    
        SPI_DELAY(_SPI_DELAY_);
    
        SPI_CLK(0);
       
        RX_DAT <<= 1;
    
        if(SPI_MISO != 0)
        {
            RX_DAT |= 0x01;  			
        }   
		
        SPI_DELAY(_SPI_DELAY_); 		
    }

    SPI_SCK(1);//空闲时SCK为高电平
    
    return RX_DAT;
}

#elif (_SPI_CPOL == 1)&&(_SPI_CPHA == 1)

uint8_t SPI_WRITE_READ_BYTE(uint8_t TX_DAT)
{
    uint8_t i;
    uint8_t RX_DAT = 0;    
       
    for(i = 0;i < 8;i ++)
    {
        SPI_CLK(0);
        
        if(TX_DAT & 0x80)
        {
            SPI_MOSI(1);    
        }
        else
        {
            SPI_MOSI(0);       
        }    

        TX_DAT <<= 1;    
   
        SPI_DELAY(_SPI_DELAY_);
    
        SPI_CLK(1);

        RX_DAT <<= 1;
    
        if(SPI_MISO != 0)
        {
            RX_DAT |= 0x01;    
        }
		
		SPI_DELAY(_SPI_DELAY_);			
			
    }
   
    return RX_DAT;
}

#endif











