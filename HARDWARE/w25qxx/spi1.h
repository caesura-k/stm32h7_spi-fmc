#ifndef _SPI1_H_
#define _SPI1_H_

#include "stm32h7xx_hal.h"


void spi1_init(SPI_HandleTypeDef    *hspi1);
void HAL_SPI_MspInit(SPI_HandleTypeDef * hspi1);


#endif






















