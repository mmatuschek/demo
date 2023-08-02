/*----------------------------------------------------------------------------
 * Name:    Timer.c
 * Purpose: MCB1700 Low level serial functions
 * Version: V1.00
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#include <LPC17xx.H>                              /* LPC17xx definitions    */
#include "Midi-Sequenzer.h"
#include "Timer.h"
#include "Led.h"

void init_timer0(void)
{
	TIM_TypeDef *timer0;
	timer0 = (TIM_TypeDef *)TIM0;

	timer0->MR0 = TIM_TICK - 1;       /* Timer0 Match Registers (S.468)     */
	timer0->MCR = 3;                  /* Interrupt and Reset on MR0 (S.469) */
	timer0->TCR = 1;                  /* Timer0 Enable (S.467)              */ 
  
	NVIC_EnableIRQ(TIMER0_IRQn );	  /* NVIC Timer0 enabled (S.64)	        */
}

void TIMER0_IRQHandler(void)
{
	TIM_TypeDef *timer0;
	timer0 = (TIM_TypeDef *)TIM0;

    GPIO2->FIOPIN ^= (1 << 3);		   /* Toggle Port Pin 2.3               */
																 
	timer0->IR  = 1;                   /* Clear MR0 interrupt flag (S.466)  */
//	timer0->TCR = 0;                   /* Counter Reset (S.467)             */
}
