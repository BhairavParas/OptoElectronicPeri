#include "DAC_api.h"
#include "SPI_api.h"
#include "StdSharc.h"

/*
void DacSpiConfigure(void)
{
  unsigned char writeBuffer[4] = {0};
  SpiInit();
}
*/
void Init_SPI_DAC(void)
{
  unsigned char writeBuffer[4] = {0};
  SpiInit();
}

void Write_DAC(int DACChannel, int data)
{
  unsigned char writeBuffer[4] = {0};

  if((DACChannel < 0) || ((DACChannel > 7) && (DACChannel != 0x0F)))
    return;
  if(data>32767)
 	  data=32767;
   else if(data<-32768)
 	  data=-32768;
  data=32767-data;
  writeBuffer[0] = 0x03;
  writeBuffer[1] = (DACChannel << 4) | ((data & 0xF000) >> 12);
  writeBuffer[2] = (data & 0x0FF0) >> 4;
  writeBuffer[3] = (data & 0x000F) << 4;
  DacLowSync();
  SpiWriteBuffer(writeBuffer, 4);
  DacSyncHigh();
}

void DacLowSync(void)
{
	sysreg_bit_clr(sysreg_FLAGS, FLG2);
	asm("nop;");
	asm("nop;");
}
void DacSyncHigh(void)
{
	sysreg_bit_set(sysreg_FLAGS, FLG2);
	asm("nop;");
	asm("nop;");
}




