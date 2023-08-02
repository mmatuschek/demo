#ifndef SINTABLE_H
#define SINTABLE_H

#define SINTABLE_ANZAHL					  256							   	 /* Anzahl der Sinuswerte */
#define SINTABLE_AUSGABE_BITS			  10						         /* Aufloesung vom DAC (DAC = Digital Analog Converter)	*/
#define SINTABLE_AUSGABE_MULTIPLIKATOR	  (((1<<SINTABLE_AUSGABE_BITS)-1)/2) /* statt shift-left waere auch pow(2,SINTABLE_AUSGABE_BITS) moeglich */

extern unsigned short sinTable[SINTABLE_ANZAHL];

void SinTable_calculate(void);


#endif	/* SINTABLE_H */
