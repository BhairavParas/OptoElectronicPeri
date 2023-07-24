#ifndef _ADC_H
#define _ADC_H

#include "discreteIO.h"
#include "sync_sport.h"

#define ad7606operating_freq 0x00080004 


void adcsportconfigure(void);


unsigned short readadc7606data(E_sportnumber SPORTNUM,E_sportnum_channel CHANNEL_NUM);
void ReadAdcdata(void);
//unsigned short readad7606data(unsigned char SPORTNUM,unsigned char CHANNEL_NUM);



void Startadc7606Conversion(void);
void checkbusyflag(void);
void enableoutputlines(void);
void disableoutputlines(void);
unsigned short readadc7606data1(unsigned char ucChannel);
/*

enum ADCChannels
{
	ANALOG1=0,
	ANALOG2,
	AZ_GYRO,
	EL_GYRO,
	AZ_RESOL,
	EL_RESOL,
	POS15VOL,
	NEG15VOL
};
*/

/* ADC */	
void ADC_SPI_configure(void);
//unsigned int ReadAdcData(int iChannel);
void AssertAdcChipSelect(void);
void DeAssertAdcChipSelect(void);

/* MUX */
void StartAdcConversion(void);
int WaitForBusyLOW(void);
int SelectMuxChannels(int iChannel);

#endif


