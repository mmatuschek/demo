/******************************************************************************
 *
 * Module:          wst_midi_sequenzer.c - Mainroutine from the Midi-Sequenzer Project
 *
 * Description:	    This file contains 
 *       
 *
 * Author:          Marco Matuschek
 *
 * Date:            2009-05-11
 *
 * History:
 *
 * Board:           Keil MCB1700
 * 
 * Date			Developer	Rev. 	Notes
 * ----------------------------------------------------------------------------
 * 2009-05-19	MM			0.03	Creation
 * 
 *****************************************************************************/

/*--------------------------------- Includes ---------------------------------*/  
   
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <LPC17xx.H>                    /* LPC17xx definitions                */
//#include "LCD.h"                        /* Graphic LCD function prototypes    */
#include "SinTable.h"                   /* Sinus Table function prototypes    */
#include "Serial.h"
#include "Timer.h"
#include "GLCD.h"
#include "LED.h"
#include "Midi-Sequenzer.h"
#include "DAC.h"

// Primary States
enum State
{
	undefined,
	NoteOn,
	Controller,
	Pitch	
}eCurrentState;

enum StatNoteOn
{
	
	/*0x60,
	0x61,
	0x62,
	0x63,
	0x64,
	0x65,
	0x66,
	0x67,
	0x68,
	0x69,
	0x6A,
	NoteOn_0x6B, */
	NoteOn_0x6C 
	/*0x6D,
	0x6E,
	0x6F,
	0x70,
	0x71,
	0x72,
	0x73,
	0x74,
	0x75,
	0x76,
	0x78*/
}eCurrentStateNoteOn; 
                  
/*-------------------------------- Variables --------------------------------*/

// Display Output String
char display_current[20];
int recieved[200];
int x=0,y=0;
unsigned dummy;
short AD_last;                          /* Last converted value  of analog digital converter (POT1)             */

unsigned short rx_buffer[100];
unsigned char data=0;
unsigned char status=0;
unsigned URSINUS;

int n=0, max=10, z,i;
int *zahlen=NULL;

unsigned int toggler=0;

const unsigned short tones[] = { 1046.50 / LOWEST_FREQ,	/* c */
							     1174.66 / LOWEST_FREQ,	/* d */
 							     1318.51 / LOWEST_FREQ,	/* e */
							     1396.91 / LOWEST_FREQ,	/* f */
							     1567.98 / LOWEST_FREQ,	/* g */
								 1760.00 / LOWEST_FREQ,	/* a */
								 1975.53 / LOWEST_FREQ	/* h */
							   };

unsigned char currentTone = 0;
unsigned short tickCounter = 0;
unsigned char iStateEntry = 0;

/*------------------------------------------------------------------------------
Notes:

The length of the receive and transmit buffers must be a power of 2.

Each buffer has a next_in and a next_out index.

If next_in = next_out, the buffer is empty.

(next_in - next_out) % buffer_size = the number of characters in the buffer.
------------------------------------------------------------------------------*/
#define TBUF_SIZE   512	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#define RBUF_SIZE   512      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/


unsigned int moncompteur = 0;




/******************************************************************************/
/*------------------------------ main function -------------------------------*/
/******************************************************************************/
//  31 - 28	  27 - 24   23 - 20   19 - 16   15 - 12   11 - 08   07 - 04   03 - 00
//  0 0 0 0   0 0 0 0   0 0 0 0   0 0 0 0	0 0 0 0	  0 0 0 0   0 0 0 0	  0 0 0 0 

int main(void) 
{
	LED_init();
	SystemInit() ;
	SER_init(0);            // Serial Initialization 
	SinTable_calculate();	  // Berechnung Sinus
//	init_dac0();
	init_timer0();	  		  // Timer0 Initialization
//	init_adc0();
	GLCD_init();
	GLCD_clear (White);
	   
 	while (1) {    /*    Embedded Loop 		 */
		sprintf(display_current, "%02X,%02X,%02X,%02X", rx_buffer[0], rx_buffer[1],	rx_buffer[2], rx_buffer[3]);
   		GLCD_displayStringLn( Line0, (unsigned char*)display_current);
		sprintf(display_current, "%02X,%02X,%02X,%02X", rx_buffer[4], rx_buffer[5], rx_buffer[6], rx_buffer[7]);
   		GLCD_displayStringLn( Line1, (unsigned char*)display_current);
							 
		//set_cursor (0, 1);
		sprintf(display_current, "Status: %-3d  ", status);
		GLCD_displayStringLn( Line2, (unsigned char*)display_current);
		//set_cursor (5, 1);
		sprintf(display_current, "Data  : %-2d  ", data);
		GLCD_displayStringLn( Line3, (unsigned char*)display_current);
		//set_cursor (10, 1);
		sprintf(display_current, "G     :%-2d  ", sizeof(rx_buffer)/sizeof(short));
		GLCD_displayStringLn( Line4, (unsigned char*)display_current);	 
	   
		
       if (0x90==rx_buffer[0])
		{
			eCurrentState=NoteOn;
	    }
	/*	state machine			*/
		switch (eCurrentState)
		{
			case NoteOn:
				if(iStateEntry == TRUE)
				{
					
					iStateEntry = FALSE;
				}
				//-----During
			  switch (eCurrentStateNoteOn)
			  {
			case NoteOn_0x6C:
				if(iStateEntry == TRUE)
				{
					
					iStateEntry = FALSE;
				}
				//-----During
				if (0x6C==rx_buffer[1])
				{
					eCurrentState=NoteOn;
					LED_on(1);
	    		}
				if (0x00==rx_buffer[2])
				{
					eCurrentState=NoteOn;
					LED_off(1);
				}
			   

				//----Exit
				//exit state?
				if(eCurrentState != NoteOn)
				{
					iStateEntry = TRUE;
				}
				break;
				}

				//----Exit
				//exit state?
				if(eCurrentState != NoteOn)
				{
					iStateEntry = TRUE;
				}
				break;

			case Controller:
				if(iStateEntry == TRUE)
				{
					
					iStateEntry = FALSE;
				}
				//-----During
			


				//----Exit
				//exit state?
				if(eCurrentState != Controller)
				{
					iStateEntry = TRUE;
				}
				break;

			case Pitch:
				if(iStateEntry == TRUE)
				{
					
					iStateEntry = FALSE;
				}
				//-----During
		


				//----Exit
				//exit state?
				if(eCurrentState != Pitch)
				{
					iStateEntry = TRUE;
				}
				break;
		}  
	}
		   
}

/******************************************************************************/
/*------------------------------- isr_serial ---------------------------------*/
/******************************************************************************/
//
//__irq void isr_serial(void) 
//{
//	volatile char	 dummy;
//	volatile char	 IIR;
//	struct buf_st	 *p;
//
//		x++;
//	/*
//	 * Repeat while there is at least one interrupt source.
//	 */
//	 
//	while ( ( (IIR = UxIIR) & 0x01 ) == 0 )	  // Interrupt Identification Register
//	{
//		switch( IIR & 0x0E )
//		{
//			case 0x06:		/* Receive Line Status */
//			FIO2SET = 0x00000001; // Turn on LEDs
//			dummy = UxLSR;  /* Just clear the interrupt source */
//			break;
//
//			case 0x04:		/* Receive Data Available */
// 		  
//			rx_current=UxRBR;
//			if (rx_current & 0x80)	   // check if last bit is set
//			{
//					// its a statusbyte	   
//					status ++;
//					// delete buffer
//					memset(rx_buffer, 0, sizeof(rx_buffer)/sizeof(short));
//					rx_buffer[0]=rx_current;	
//					data=1;
//			}
//			else
//			{
//					// its a databyte
//					rx_buffer[data]=rx_current;
//					data ++;
//			}		 
//
//		 	dummy = UxLSR;  /* Just clear the interrupt source */
//
//			break;
//			case 0x0C:		/* Character Time-Out */
//			FIO2SET = 0x00000004; // Turn on LEDs
//			p = &rbuf;
//
//			if ( ( (p->in - p->out) & ~(RBUF_SIZE - 1 ) ) == 0 )
//			{
//				p->buf[ p->in & (RBUF_SIZE - 1 ) ] = U1RBR;
//		   
//				/* Mecanism for inc cmd nb */
//				if( cpt == 0)
//				{
//					cpt = p->buf[ p->in & (RBUF_SIZE - 1 ) ];
//					cpt++;
//		   		}
//				else
//				{
//					cpt--;
//					if ( cpt == 0 )
//					{
//				//		monitorNbCmd++;
//					}
//				}
//				p->in++;
//			}
//			break;
//
//			case 0x02: /* THRE (Transmit Holding Register Empty) Interrupt */
//			FIO2SET = 0x00000010; // Turn on LEDs
//			p = &tbuf;
//
//			if ( p->in != p->out )
//			{
//				U1THR = p->buf[ p->out & (TBUF_SIZE - 1) ];
//				p->out++;
//				tx_restart = 0;
//			}
//			else
//			{
//				tx_restart = 1;
//			}
//			break;
//
//			case 0x00:			/* Modem Interrupt */
//			FIO2SET = 0x00000020; // Turn on LEDs
//				dummy = UxMSR;  /* Just clear the interrupt source */
//				break;
//
//			default:
//				break;
//		}
//	}
//
//	VICVectAddr = 0;	/* Acknowledge Interrupt */
//}

///* Timer Counter 0 Interrupt executes @ DATA_FREQ Rate                          */
//__irq void isr_timer0(void)
//{
////  31 - 28	  27 - 24   23 - 20   19 - 16   15 - 12   11 - 08   07 - 04   03 - 00
////  0 0 0 0   0 0 0 0   0 0 0 0   0 0 0 0	0 0 0 0	  0 0 0 0   0 0 0 0	  0 0 0 0 
////  0 0 0 0   0 0 0 0   0 0 0 0   0 0 0 0	1 1 1 1   1 1 1 1   1 1 0 0   0 0 0 0		  allgemeiner Wertebereich DAC Register
////  0 0 0 0   0 0 0 0   0 0 0 0   0 0 0 0	1 0 0 0   0 0 0 0   0 0 0 0   0 0 0 0    	  Mittelwert vom DACR 0x00008000   
////  0 0 0 0   0 0 0 0   0 0 0 0   0 0 0 0	0 0 0 0	  0 0 0 0   0 0 1 1	  1 0 1 1         0x3B           
////											0 0 1 1   1 0 1 1
//	unsigned int laut, leise;
//	unsigned int x, y;
//
//	tickCounter++;
//	if(tickCounter >= 11000)   															/* 11000 = 0,5 / 0,000045454545 */
//	{
//		tickCounter = 0;
//		currentTone++;
//		if(currentTone > 6)
//		{
//			currentTone = 0;
//		}
//	}
//
//	laut = sinTable[toggler];
//	leise = laut / (AD_last >> 4 + 1);																   // laut,leise = Lautstaerke regelung
//	//leise = laut / 10;																   // laut,leise = Lautstaerke regelung
//																					   
//	x = leise;																		   // x,y = Bit-verschiebung
//	y = x << 7;
//
//	DACR = y;
//	/* Wenn jeder Wert genutzt wird haben wir 22kHz/2048 Werte = 10.74 Hz
//	 * fuer ein c mit 1046.5 Hz also 1046.5/10.74  => toggler += 97
//	 */
//	toggler += tones[currentTone];			
//	if (toggler>=SINTABLE_ANZAHL)
//	{
//	   	toggler %= SINTABLE_ANZAHL;
//	}
//
///*	if (toggler==0)
//	{ 
//		toggler=1;
//		FIO2SET = 0x00000001; // Turn on LEDs
//  		DACR    = 0x00000F00;  
//	}
//	else
//	{
//		toggler=0;
//		FIO2CLR = 0x00000001; // Turn off LEDs
//  		DACR    = 0x00000000;  
//	}*/																		 
//	T0IR        = 1;                          /* Clear interrupt flag           */
//	VICVectAddr = 0;                          /* Acknowledge Interrupt          */
//}
//
///* A/D IRQ: Executed when A/D Conversion is done                              */
//__irq void ADC_IRQHandler(void) {
//
//  AD_last = (AD0DR0 >> 6) & 0x3FF;      /* Read Conversion Result             */
//
//  VICVectAddr = 0;                      /* Acknowledge Interrupt              */
//}
//
//void init_timer0(void)
//{
//	PINSEL1 |= 0x00204000;                    /* Select AOUT,AIN0               */
//	DACR    = 0x00008000;                     /* DAC Output set to Middle Point */
//	/* Setup Timer 0: Periodic Interrupt @ DATA_FREQ Rate                       */
//	T0MR0 = TIM_TICK - 1;                     /* Timer0 Match Value 0           */
//	T0MCR = 3;                                /* Interrupt and Reset on MR0     */
//	T0TCR = 1;                                /* Timer0 Enable                  */
//	VICVectAddr4 = (unsigned long)isr_timer0; /* Set Interrupt Vector in 0      */
//	VICVectCntl4 = 0x20 | 4;                  /* Use it for Timer0 Interrupt    */
//	VICIntEnable |= 0x00000010;               /* Enable Timer0 Interrupt        */
//}
//
// /* Power enable, Setup pin, enable and setup AD converter interrupt         */
// void init_adc0(void)
// {
//  PCONP        |= (1 << 12);                   /* Enable power to AD block    */
//  PINSEL1       |= 0x4000;                      /* AD0.0 pin function select   */
//  AD0INTEN      = (1 <<  0);                   /* CH0 enable interrupt        */
//  AD0CR         = 0x00210301;                  /* Power up, PCLK/4, sel AD0.0 */
//  VICVectAddr18 = (unsigned long)ADC_IRQHandler;/* Set Interrupt Vector       */
//  VICVectCntl18 = 14;                          /* use it for ADC Interrupt    */
//  VICIntEnable  |= (1  << 18);                  /* Enable ADC Interrupt        */
//  }
