/*----------------------------------------------------------------------------
 * Name:    Serial.c
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Serial.h"
#include "Midi-Sequenzer.h"

unsigned short rx_current=0;

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
#if TBUF_SIZE < 2
#error TBUF_SIZE is too small.  It must be larger than 1.
#elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
#error TBUF_SIZE must be a power of 2.
#endif

#if RBUF_SIZE < 2
#error RBUF_SIZE is too small.  It must be larger than 1.
#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
#error RBUF_SIZE must be a power of 2.
#endif

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/

static struct buf_st rbuf = { 0, 0, };
#define SIO_RBUFLEN ((unsigned short)(rbuf.in - rbuf.out))
static struct buf_st tbuf = { 0, 0, };
#define SIO_TBUFLEN ((unsigned short)(tbuf.in - tbuf.out))
static unsigned int	tx_restart = 1;   /* NZ if TX restart is required */
static int 			cpt		   = 0;

/*


	VICIntEnable = 1 << 7;					  // enable UART1 interrupt	S.92

	UxLCR |= 0x80;           // enable access to divisor latches S.422
    UxDLM  = 0;              // no need for divisor latch S.416 
    UxDLL  = 24;             // 31250 Baud Rate @ 12.0 MHZ PCLK (UARTnBaudrate=PCLK/(16x(256xUnDLM+UnDLL))) S.406  
	UxLCR &= ~0x80;          // disable access to divisor latches S.422
	UxFCR  = 2;				 // enable UART1 Rx FIFO S.421

	dummy = UxIIR;   // read IrqID - required to get interrupts started

*/

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_init (int uart) {
UART_TypeDef *pUart;
   

  if (uart == 0) {                               /* UART0 */
    PINCON->PINSEL0 |= (1 << 4);                 /* Pin P0.2 used as TXD0 (Com0) */
    PINCON->PINSEL0 |= (1 << 6);                 /* Pin P0.3 used as RXD0 (Com0) */

    pUart = (UART_TypeDef *)UART0;
	NVIC_EnableIRQ(UART0_IRQn ); 
  } else {                                       /* UART1 */
    PINCON->PINSEL4 |= (2 << 0);                 /* Pin P2.0 used as TXD1 (Com1) */
    PINCON->PINSEL4 |= (2 << 2);                 /* Pin P2.1 used as RXD1 (Com1) */

    pUart = (UART_TypeDef *)UART1;
	NVIC_EnableIRQ(UART1_IRQn ); 
  }

  pUart->IER	= 0;             // disable UART RDA (Receive Date Available) interrupts

  pUart->LCR    = 0x83;                          /* 8 bits, no Parity, 1 Stop bit  */
  pUart->DLL    = 36;                             /* 31250 Baud Rate @ 18.0 MHZ PCLK */
  pUart->FDR    = 0x10;                          /* MULVAL = 1, DIVADDVAL = 0  */
  pUart->DLM    = 0;                             /* High divisor latch = 0         */
  pUart->LCR    = 0x03;                          /* DLAB = 0                       */

  pUart->IER = 1;       // enable UART1 RDA (Receive Date Available) interrupts 
}

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void UART0_IRQHandler (void)  {
  volatile uint32_t dummy;
  UART_TypeDef *pUart;
  volatile char	 IIR;
  struct buf_st	 *p;

  pUart = UART0;

	/*
	 * Repeat while there is at least one interrupt source.
	 */
	 
	while ( ( (IIR = pUart->IIR) & 0x01 ) == 0 )	  // Interrupt Identification Register
	{
		switch( IIR & 0x0E )
		{
			case 0x06:		/* Receive Line Status */
			dummy = pUart->LSR;  /* Just clear the interrupt source */
			break;

			case 0x04:		/* Receive Data Available */
 		  
			rx_current=pUart->RBR;
			if (rx_current & 0x80)	   // check if last bit is set
			{
					// its a statusbyte	   
					status ++;
					// delete buffer
					memset(rx_buffer, 0, sizeof(rx_buffer)/sizeof(short));
					rx_buffer[0]=rx_current;	
					data=1;
			}
			else
			{
					// its a databyte
					rx_buffer[data]=rx_current;
					data ++;
			}		 

		 	dummy = pUart->LSR;  /* Just clear the interrupt source */

			break;
			case 0x0C:		/* Character Time-Out */
			p = &rbuf;

			if ( ( (p->in - p->out) & ~(RBUF_SIZE - 1 ) ) == 0 )
			{
				p->buf[ p->in & (RBUF_SIZE - 1 ) ] = pUart->RBR;
		   
				/* Mecanism for inc cmd nb */
				if( cpt == 0)
				{
					cpt = p->buf[ p->in & (RBUF_SIZE - 1 ) ];
					cpt++;
		   		}
				else
				{
					cpt--;
					if ( cpt == 0 )
					{
				//		monitorNbCmd++;
					}
				}
				p->in++;
			}
			break;

			case 0x02: /* THRE (Transmit Holding Register Empty) Interrupt */
			p = &tbuf;

			if ( p->in != p->out )
			{
				pUart->THR = p->buf[ p->out & (TBUF_SIZE - 1) ];
				p->out++;
				tx_restart = 0;
			}
			else
			{
				tx_restart = 1;
			}
			break;

			case 0x00:			/* Modem Interrupt */
				//dummy = pUart->MSR;  /* Just clear the interrupt source */
				break;

			default:
				break;
		}
	}


  //temp = pUart->LSR;
  //temp = pUart->RBR;
 	
  dummy = UART0->ICR;                              /* clear interrupts */

}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_putChar (int uart, int c) {
  UART_TypeDef *pUart;

  pUart = (uart == 0) ? UART0 : (UART_TypeDef *)UART1;
  while (!(pUart->LSR & 0x20));
  return (pUart->THR = c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_getChar (int uart) {
  UART_TypeDef *pUart;

  pUart = (uart == 0) ? UART0 : (UART_TypeDef *)UART1;
  while (!(pUart->LSR & 0x01));
  return (pUart->RBR);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (non blocking read)
 *----------------------------------------------------------------------------*/
int SER_getChar_nb (int uart) {
  UART_TypeDef *pUart;

  pUart = (uart == 0) ? UART0 : (UART_TypeDef *)UART1;
  if (pUart->LSR & 0x01)
    return (pUart->RBR);
  else
    return 0;
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER_putString (int uart, unsigned char *s) {

  while (*s != 0) {
   SER_putChar(uart, *s++);
  }
}
