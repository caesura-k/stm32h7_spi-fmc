#include "spi1.h"
#include "usart.h"


void spi1_init(SPI_HandleTypeDef * hspi1)
{
	hspi1->Instance = SPI1;
	hspi1->Init.Mode = SPI_MODE_MASTER;
	hspi1->Init.Direction = SPI_DIRECTION_2LINES;
	hspi1->Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1->Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1->Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1->Init.NSS = SPI_NSS_SOFT;
	hspi1->Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1->Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1->Init.CRCPolynomial = 0x7;
	hspi1->Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
	hspi1->Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	hspi1->Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	hspi1->Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi1->Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi1->Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi1->Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi1->Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi1->Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
	hspi1->Init.IOSwap = SPI_IO_SWAP_DISABLE;
	if (HAL_SPI_Init(hspi1) != HAL_OK)
	{
		printf("spi1 init fail...\r\n");
	}
	else
		printf("spi1 init done...\r\n");
}

void HAL_SPI_MspInit(SPI_HandleTypeDef * hspi1)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_6;//CS
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP; 
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;//SCK,MISO
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7;//MOSI
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	printf("spi1 gpio init done...\r\n");
}

























