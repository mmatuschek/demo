#include <math.h>
#include "SinTable.h"

#define	PI						  3.14159265358979323846

unsigned short sinTable[SINTABLE_ANZAHL];

void SinTable_calculate(void)
{
	int i;
	for(i = 0; i<SINTABLE_ANZAHL ;i++)
	{
		double ergebnis = sin(((float)i/SINTABLE_ANZAHL)*2*PI);
		sinTable[i]=(unsigned int)((ergebnis+1)*SINTABLE_AUSGABE_MULTIPLIKATOR);	
	}
}
