#include <lpc17xx.h>
#include "DAC.h"

void init_dac0()
{
	DAC_TypeDef *dac;
	PINCON_TypeDef *pin;
	
	pin->PINSEL1 |= 0x00204000;                    /* Select AOUT,AIN0               */
	dac->DACR    = 0x00008000;                     /* DAC Output set to Middle Point */	
}

