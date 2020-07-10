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
	
	/*SRAMʱ������趨*/
	Timing.AddressSetupTime=7; //��ַ����ʱ��n*FMC�������ʱ������
	Timing.AddressHoldTime=1;  //��ַ����ʱ��n*FMC�������ʱ������
	Timing.DataSetupTime=6;    //���ݽ���ʱ��n*FMC�������ʱ������
	Timing.BusTurnAroundDuration=0;      //��дת������ʱ������Ϊ0
	Timing.CLKDivision=0;          	     //FMC���ʱ�ӷ�Ƶϵ�� ���첽����ʱ��Ч
	Timing.DataLatency=0;                //�����ӳ� ���첽����ʱ��Ч
	Timing.AccessMode=FMC_ACCESS_MODE_A; //ʱ�����ģʽA ��FMC_BCRx�Ĵ�����ExtModeλΪ0ʱ��������Ч
	
	/*FMC SRAM�����趨*/
	hnor.Instance=FMC_NORSRAM_DEVICE;                       //SRAM�ӿڼĴ�����ַ
	hnor.Extended=FMC_NORSRAM_EXTENDED_DEVICE;              //��չ�Ĵ�����ַ ��д�ɲ�ͬʱ��
	hnor.Init.NSBank=FMC_NORSRAM_BANK1;					    //ѡ��FMC SRAM Bank1��
	hnor.Init.DataAddressMux=FMC_DATA_ADDRESS_MUX_DISABLE;   //ʹ�����ݵ�ַ������
	hnor.Init.MemoryType=FMC_MEMORY_TYPE_NOR;           	 //ѡ��洢������
	hnor.Init.MemoryDataWidth=FMC_NORSRAM_MEM_BUS_WIDTH_16; //����λ��Ϊ16λ
	hnor.Init.BurstAccessMode=FMC_BURST_ACCESS_MODE_DISABLE;//��ֹ��ͻ������ �������������첽ģʽ
	hnor.Init.WaitSignal=FMC_WAIT_SIGNAL_DISABLE;
	hnor.Init.WaitSignalPolarity=FMC_WAIT_SIGNAL_POLARITY_LOW;
	hnor.Init.WaitSignalActive=FMC_WAIT_TIMING_BEFORE_WS;
	hnor.Init.AsynchronousWait=FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hnor.Init.WriteOperation=FMC_WRITE_OPERATION_ENABLE;
	hnor.Init.ExtendedMode=FMC_EXTENDED_MODE_DISABLE;       //��ֹ��չģʽ ��ʱ��д����Mode1ģʽ����
	hnor.Init.WriteBurst=FMC_WRITE_BURST_DISABLE;           //��ֹдͻ������ д�����������첽ģʽ
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







