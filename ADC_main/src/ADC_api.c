#include "ADC_api.h"
#include "StdSharc.h"

int AdcData[8]={0};
void StartAdcConversion(void)
{
	SRU(LOW,DAI_PB04_I);
    SRU(HIGH,DAI_PB04_I);
}
void CheckBusyFlag(void)
{
	while((*pDAI_PIN_STAT & 0x00000004)!= 0);
}
void EnableOutputLines(void)
{
	SRU(LOW,DAI_PB08_I);
}
void DisableOutputLines()
{
	SRU(HIGH,DAI_PB08_I);
}
void ReadAdcdata()
{
	int i;
	int ch;
	int dummy;
	unsigned short data=0;
	StartAdcConversion();
	CheckBusyFlag();
	while((*pSPCTL1 & 0xc0000000) == 0x0);
		dummy = (*pRXSP1A);

	while((*pSPCTL1 & 0xc0000000) == 0x0);
	 	dummy = (*pRXSP1A);
	for(i=0;i<=7;i++)
	{
		while((*pSPCTL1 & 0xc0000000) == 0x0);
			AdcData[i] = (*pRXSP1A);
		if(AdcData[i] > 32767)
			AdcData[i] = AdcData[i] - 65535;
	}
}
















