/*----------------------------------------------------------------------------
 * Name:    Timer.h
 * Purpose: MCB1700 Low level serial definitions
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
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#ifndef __TIMER_H
#define __TIMER_H

extern void init_timer0(void);

#define CPU_CLOCK   100000000                // CPU Clock 100MHz                   
#define DATA_FREQ       22000                // Sound Data Frequency          
#define VPB_CLOCK   (CPU_CLOCK/12)           // VPB Clock                 
#define TIM_TICK    (1.0/DATA_FREQ)*18000000    // Timer Tick, laut Berechnung von Quategra  14400000 
												// leider habe ich noch keine rechnung: 18000000 = 22kHz
#endif
