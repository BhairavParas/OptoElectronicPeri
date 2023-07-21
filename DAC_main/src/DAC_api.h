#ifndef __DAC_H
#define __DAC_H

#include "StdSharc.h"

void Write_DAC(int DACChannel, int data);
void Write_DACWord(int DACChannel, int data);

//void DacSpiConfigure(void);
void Init_SPI_DAC(void);

void DacSpiWordConfigure(void);

void DacLowSync(void);

void DacSyncHigh(void);

void DacWrite_MultiChannel(unsigned short *dacValues, unsigned char mask);
void DacWrite_MultiChannelWord(unsigned short *dacValues, unsigned char mask);

void DacWrite_MultiChannel_SoftwareLDAC(unsigned short *dacValues, unsigned char mask);
void DacWrite_MultiChannelWord_SoftwareLDAC(unsigned short *dacValues, unsigned char mask);

#endif
