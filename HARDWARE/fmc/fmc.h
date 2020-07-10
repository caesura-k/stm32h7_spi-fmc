#ifndef _FMC_H_
#define _FMC_H_

#include "sys.h"	 
#include "stdlib.h" 
#include "usart.h"


#define FPGA_WRITE(offset,data)	*((volatile unsigned short int *)(0x60000000 + (offset << 17))) = data
#define FPGA_READ(offset)	    *((volatile unsigned short int *)(0x60000000 + (offset << 17)))

int fmc_init(void);
void fmc_test(void);


#endif
