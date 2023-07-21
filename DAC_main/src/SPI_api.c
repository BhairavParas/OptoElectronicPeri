#include "StdSharc.h"
//#include "spi.h"
//#include "StdSharc.h"
//#include "uart.h"
//#include "debug.h"
//#include "timer.h"
//#include "adc.h"
//#include "discreteIO.h"
//#include "dblr_fov.h"
//#include<sru.h>
//#include<time.h>

void SpiInit(void)
{
	unsigned short tempData = 0;
	*pSPIBAUD = 1 << 1;
	#define TIMOD 0x01
  	#define SPI_WORDLENGTH WL8
	*pSPICTL =   SPIMS | SPI_WORDLENGTH | TIMOD | CPHASE | MSBF;
 	*pSPICTL &= ~(ILPBK);
	*pSPICTL |= SPIEN;
  	#undef TIMOD
  	#undef SPI_WORDLENGTH
	while (*pSPISTAT & RXS)
		tempData = *pRXSPI;
	*pSPISTAT = 0xFF;
}

void SpiWriteBuffer(unsigned char *writeBuffer, unsigned short count)
{
  int index = 0;
  unsigned char ch;
  if(!writeBuffer)
    return;
  while(index < count)
  {
    *pTXSPI = writeBuffer[index];
    while((*pSPISTAT & TXS) != 0);
//    while((*pSPISTAT & RXS) == 0);
    while((*pSPISTAT & RXS) == 0);
    ch = *pRXSPI;	// Dummy read to clear RX buffer
    ++index;
  }
}

void SpiWriteByte(unsigned char ch)
{
  *pTXSPI = ch;
  while((*pSPISTAT & TXS) != 0);
  while((*pSPISTAT & RXS) == 0);
  ch = *pRXSPI;		// Dummy read to clear RX buffer
}

void SpiReadBuffer(unsigned char *readBuffer, unsigned short count)
{
  int index = 0;
  if(!readBuffer)
    return;
  while(index < count)
  {
   *pTXSPI = 0;
    while((*pSPISTAT & TXS) != 0);
    while((*pSPISTAT & RXS) == 0);
    readBuffer[index] = *pRXSPI;
    ++index;
  }
}
unsigned char SpiReadByte(void)
{
  unsigned char ch;
  unsigned short retval = 0;
  *pTXSPI = 0;
  while((*pSPISTAT & TXS) != 0);
  while((*pSPISTAT & RXS) == 0);
  ch = *pRXSPI;
  return ch;
}
