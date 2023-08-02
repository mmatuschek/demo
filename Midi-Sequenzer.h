#ifndef MIDISEQUENZER_H
#define MIDISEQUENZER_H

#define LOWEST_FREQ (10.74)

#ifndef TRUE
	#define TRUE	1
#endif
#ifndef FALSE
	#define FALSE	0
#endif	


/*-------------------------- Prototype Declarations --------------------------*/

//void init_lcd(void);
void init_adc0(void);
void init_ursinus(void);
int getkey(void);
int sendchar(int ch);

/*-------------------------------- Variables --------------------------------*/

// Display Output String
extern int recieved[200];
extern int x;
extern int y;
extern unsigned dummy;
extern short AD_last;                          /* Last converted value  of analog digital converter (POT1)             */

extern unsigned short rx_buffer[100];
extern unsigned char data;
extern unsigned char status;
extern unsigned URSINUS;

extern int n;
extern int max;
extern int z;
extern int i;
extern int *zahlen;

extern unsigned int toggler;

extern const unsigned short tones[];

extern unsigned char currentTone;
extern unsigned short tickCounter;
extern unsigned char iStateEntry;

/*------------------------------------------------------------------------------
Notes:

The length of the receive and transmit buffers must be a power of 2.

Each buffer has a next_in and a next_out index.

If next_in = next_out, the buffer is empty.

(next_in - next_out) % buffer_size = the number of characters in the buffer.
------------------------------------------------------------------------------*/
#define TBUF_SIZE   512	     /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/
#define RBUF_SIZE   512      /*** Must be a power of 2 (2,4,8,16,32,64,128,256,512,...) ***/


extern unsigned int moncompteur;



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
struct buf_st
{
	unsigned int	in;					/* Next In Index	*/
	unsigned int	out;				/* Next Out Index	*/
	char			buf [RBUF_SIZE];	/* Buffer			*/
};
#define SIO_RBUFLEN ((unsigned short)(rbuf.in - rbuf.out))
#define SIO_TBUFLEN ((unsigned short)(tbuf.in - tbuf.out))

/******************************************************************************/
/*------------------------------ main function -------------------------------*/
/******************************************************************************/
//  31 - 28	  27 - 24   23 - 20   19 - 16   15 - 12   11 - 08   07 - 04   03 - 00
//  0 0 0 0   0 0 0 0   0 0 0 0   0 0 0 0	0 0 0 0	  0 0 0 0   0 0 0 0	  0 0 0 0 


#endif /* MIDISEQUENZER_H */
