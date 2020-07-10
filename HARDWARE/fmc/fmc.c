#include "fmc.h"
#include "stm32h7xx_hal_nor.h"
#include "usart.h"

void HAL_NOR_MspInit(NOR_HandleTypeDef *hnor)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
	//FMC NL/NADV
	GPIO_InitStruct.Pin=GPIO_PIN_7;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull=GPIO_NOPULL;
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate=GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//FMC NE1 NWAIT
	GPIO_InitStruct.Pin=GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Alternate=GPIO_AF9_FMC;
	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	//FMC A DA CLK ...GPIO_PIN_6|GPIO_PIN_7
	GPIO_InitStruct.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|
						GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|
						GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Alternate=GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOD,&GPIO_InitStruct);
		
	//FMC A DA
	GPIO_InitStruct.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10|
                        GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11| 
                        GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE,&GPIO_InitStruct);
	printf("fmc gpio init done...\r\n");
}


int fmc_init(void)
{
	FMC_NORSRAM_TimingTypeDef Timing;
	NOR_HandleTypeDef hnor;
	printf("fmc init...\r\n");
	__HAL_RCC_FMC_CLK_ENABLE();	
	
	//HAL_NOR_DeInit(&hnor);
	HAL_NOR_MspInit(&hnor);
	
	/*SRAM时序操作设定*/
	Timing.AddressSetupTime=7; //地址建立时间n*FMC外设核心时钟周期
	Timing.AddressHoldTime=1;  //地址保持时间n*FMC外设核心时钟周期
	Timing.DataSetupTime=6;    //数据建立时间n*FMC外设核心时钟周期
	Timing.BusTurnAroundDuration=0;      //读写转换插入时间周期为0
	Timing.CLKDivision=0;          	     //FMC输出时钟分频系数 在异步操作时无效
	Timing.DataLatency=0;                //数据延迟 在异步操作时无效
	Timing.AccessMode=FMC_ACCESS_MODE_A; //时序操作模式A 在FMC_BCRx寄存器的ExtMode位为0时此设置无效
	
	/*FMC SRAM操作设定*/
	hnor.Instance=FMC_NORSRAM_DEVICE;                       //SRAM接口寄存器地址
	hnor.Extended=FMC_NORSRAM_EXTENDED_DEVICE;              //扩展寄存器地址 读写可不同时序
	hnor.Init.NSBank=FMC_NORSRAM_BANK1;					    //选择FMC SRAM Bank1区
	hnor.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE;   //使能数据地址不复用
	hnor.Init.MemoryType=FMC_MEMORY_TYPE_NOR;           	 //选择存储器类型
	hnor.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; //数据位宽为16位
	hnor.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE;//禁止读突发访问 读操作工作在异步模式
	hnor.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;
	hnor.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;
	hnor.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;
	hnor.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hnor.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;
	hnor.Init.ExtendedMode=FMC_EXTENDED_MODE_DISABLE;       //禁止扩展模式 此时读写按照Mode1模式进行
	hnor.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;           //禁止写突发访问 写操作工作在异步模式
	hnor.Init.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
	hnor.Init.WriteFifo=FMC_WRITE_FIFO_ENABLE;
	hnor.Init.PageSize=FMC_PAGE_SIZE_NONE;
	
	if(HAL_NOR_Init(&hnor,&Timing,NULL)!=HAL_OK)
	{
		printf("hal nor-ram init fail......\r\n");
	}
	else
		printf("hal nor-ram init success......\r\n");
	return 0;
}


void fmc_test(void)
{
	FPGA_WRITE(1,1);
	HAL_Delay(500);
	FPGA_WRITE(1,0);
	printf("Read Data From FPGA:%04x \r\n",FPGA_READ(3));
	
	FPGA_WRITE(1,2);
	HAL_Delay(500);
	FPGA_WRITE(1,0);
	HAL_Delay(500);
}







