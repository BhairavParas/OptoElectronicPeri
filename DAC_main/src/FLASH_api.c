
#define _FLASH_C

#include "FLASH_api.h"
#include "StdSharc.h"
#include "SPI_api.h"
//#include "debug.h"
///// UART PORT/////
 void Flash_EnableWrite(unsigned char flash_type);
 void EnableFlash(unsigned char flash_type);
 void DisableFlash(unsigned char flash_type);
 unsigned char Flash_ReadStatusReg(unsigned char flash_type);

 //void SpiInitForFlash(void);
 void SpiInitForDAC(void);

 void Flash_Read(unsigned char flash_type, int address, unsigned char *data, int length);
 void Flash_Write(unsigned char flash_type, int address, unsigned char *data, int length);
 void UnProtectFlash(unsigned char, int address);
void Delay_Cycles(unsigned int DelayCount)
{
	 while(--DelayCount);
}
unsigned char Flash_ReadStatusReg(unsigned char flash_type)
{
   unsigned char status = 0;
   EnableFlash(flash_type);

   SpiWriteByte(FLASH_RDSR);
   status = SpiReadByte();

   DisableFlash(flash_type);
   return status;
}
void UnProtectFlash(unsigned char flash_type, int address)
{
 	EnableFlash(flash_type);
 	SpiWriteByte(FLASH_WREN);
 	DisableFlash(flash_type);
 	Delay_Cycles(10);
 	EnableFlash(flash_type);
 	SpiWriteByte (0x39);
 	SpiWriteByte ((address&0x00FF0000)>>16);
 	SpiWriteByte ((address&0x0000FF00)>>8);
 	SpiWriteByte ((address&0x000000FF));
 	DisableFlash(flash_type);
 	Delay_Cycles(10);
 	return ;
}

int Flash_SectorErase(unsigned char flash_type,
                      unsigned int sectorID)
{
  int address = sectorID * FLASH_SECTOR_SIZE;
  unsigned char command[4] = {0};

  UnProtectFlash(flash_type,address);
  if(flash_type > DATA_FLASH)
    return -1;

  if(address >= FLASH_SIZE)
    return -2;

  Flash_EnableWrite(flash_type);

	command[0] = FLASH_SE;
	command[1] = (address & 0xFF0000) >> 16;
	command[2] = (address & 0xFF00) >> 8;
	command[3] = address & 0xFF;

  // Enable chip select
  EnableFlash(flash_type);

  SpiWriteBuffer(command, 0x04);

  // Disable chip select
  DisableFlash(flash_type);

  Flash_WaitForWrite(flash_type);

  return 0;
}
/*
int Flash_SectorErase(unsigned char flash_type,
                       unsigned int sectorID)
{
	int address = sectorID * FLASH_SECTOR_SIZE;
	unsigned char command[4] = {0};
 	command[0] = FLASH_SE;
 	command[1] = (address & 0xFF0000) >> 16;
 	command[2] = (address & 0xFF00) >> 8;
 	command[3] = address & 0xFF;

	UnProtectFlash(flash_type,address);
	if(flash_type > DATA_FLASH)
     return -1;
	if(address >= FLASH_SIZE)
     return -2;
	EnableFlash(flash_type);
	Flash_EnableWrite(flash_type);
	SpiWriteBuffer(command, 0x04);
	DisableFlash(flash_type);
	Flash_WaitForWrite(flash_type);
   return 0;
}
*/

int Flash_WriteBuffer(unsigned char flash_type,
                       unsigned int  address,
                       unsigned char *data,
                       unsigned int  length)
{
 	int retval = length;
 	int tLength = length;
 	unsigned char readBuffer[FLASH_PAGE_SIZE] = {0};

 	if(flash_type > DATA_FLASH)
 	  return -1;
 	if((address >= FLASH_SIZE) || (address+length-1 >= FLASH_SIZE))
 	  return -2;
 	if(!data)
 	  return -3;
   if(length == 0)
     return 0;
   do
   {
     length = (tLength > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : tLength;
     tLength -= length;
     Flash_Write(flash_type, address, data, length);
     Flash_WaitForWrite(flash_type);
     address += length;
     data += length;
   }while(tLength > 0);
   return retval;
 }

void Flash_Write(unsigned char flash_type, int address, unsigned char *data, int length)
{
	unsigned char command[4] = {0};
	command[0] = FLASH_PP;
	command[1] = (address & 0xFF0000) >> 16;
	command[2] = (address & 0xFF00) >> 8;
	command[3] = address & 0xFF;

	Flash_EnableWrite(flash_type);
	EnableFlash(flash_type);
	SpiWriteBuffer(command, 0x04);
	SpiWriteBuffer(data, length);
	DisableFlash(flash_type);
}

void Flash_EnableWrite(unsigned char flash_type)
{
  EnableFlash(flash_type);
  SpiWriteByte(FLASH_WREN);
  DisableFlash(flash_type);
  Delay_Cycles(10);
}

void EnableFlash(unsigned char flash_type)
{
	//SpiInitForFlash();
	Init_SPI_Flash();

	if(flash_type) 				// Data flash
    sysreg_bit_clr(sysreg_FLAGS, FLG1);
    else
    SRU2(LOW, DPI_PB05_I);     	// Set DPI_PB05 low
}

void DisableFlash(unsigned char flash_type)
{
  if(flash_type) 				// Data flash
    sysreg_bit_set(sysreg_FLAGS, FLG1);
  else
    SRU2(HIGH, DPI_PB05_I);     // Set DPI_PB05 low
  SpiInitForDAC();
}

void Flash_WaitForWrite(unsigned char flash_type)
{
  unsigned char status;
  do
  {
    status = Flash_ReadStatusReg(flash_type);
  }while(status & 0x01);
  if(status&0x20)
  	Delay_Cycles(10);
}
/*
void SpiInitForFlash(void)
{
  unsigned short tempData = 0;
  *pSPIBAUD = 2 << 1;				// SCLK = PCLK / (8 x BAUD); Here, PCLK = 132e6
  #define TIMOD 0x01
  #define SPI_WORDLENGTH WL8     	// WL8 or WL16

  *pSPICTL =   SPIMS | SPI_WORDLENGTH | TIMOD | MSBF;
  *pSPICTL &= ~(ILPBK);
  *pSPICTL |= SPIEN;
	#undef TIMOD
	#undef SPI_WORDLENGTH
	while (*pSPISTAT & RXS)
		tempData = *pRXSPI;
	*pSPISTAT = 0xFF;
}
*/
void Init_SPI_Flash(void)
{
  unsigned short tempData = 0;
  *pSPIBAUD = 2 << 1;				// SCLK = PCLK / (8 x BAUD); Here, PCLK = 132e6
  #define TIMOD 0x01
  #define SPI_WORDLENGTH WL8     	// WL8 or WL16

  *pSPICTL =   SPIMS | SPI_WORDLENGTH | TIMOD | MSBF;
  *pSPICTL &= ~(ILPBK);
  *pSPICTL |= SPIEN;
	#undef TIMOD
	#undef SPI_WORDLENGTH
	while (*pSPISTAT & RXS)
		tempData = *pRXSPI;
	*pSPISTAT = 0xFF;
}



void SpiInitForDAC(void)
{
  SpiInit();
}

int Flash_ReadBuffer(unsigned char flash_type,
                     unsigned int  address,
                     unsigned char *data,
                     unsigned int  length)
{
	if(flash_type > DATA_FLASH)
		return -1;
	if((address >= FLASH_SIZE) || (address+length-1 >= FLASH_SIZE))
		return -2;
	if(!data)
		return -3;
  Flash_Read(flash_type, address, data, length);
  return length;
}

void Flash_Read(unsigned char flash_type, int address, unsigned char *data, int length)
{
  unsigned char command[5] = {0};
	command[0] = FLASH_FAST_READ;
	command[1] = (address & 0xFF0000) >> 16;
	command[2] = (address & 0xFF00) >> 8;
	command[3] = address & 0xFF;
	command[4] = 0;
	EnableFlash(flash_type);
	SpiWriteBuffer(command, 0x05);
	SpiReadBuffer(data, length);
	DisableFlash(flash_type);
}

int Flash_ReadID(unsigned char flash_type, unsigned char *flashID)
{
  if(flash_type > DATA_FLASH)
    return -1;

    DisableFlash(flash_type);
  EnableFlash(flash_type);

  SpiWriteByte(FLASH_RDID);
  flashID[0] = SpiReadByte();
  flashID[1] = SpiReadByte();
  flashID[2] = SpiReadByte();

  DisableFlash(flash_type);
  return 0;
}


/*void Flash_Init(void)
{

}


int Flash_ReadByte(unsigned char flash_type,
                   unsigned int  address,
                   unsigned char *data)
{
  unsigned char command[4] = {0};

  if(flash_type > DATA_FLASH)
    return -1;

	if(address >= FLASH_SIZE)
	  return -2;

	if(!data)
	  return -3;

	command[0] = FLASH_READ;
	command[1] = (address & 0xFF0000) >> 16;
	command[2] = (address & 0xFF00) >> 8;
	command[3] = address & 0xFF;

	EnableFlash(flash_type);

	SpiWriteBuffer(command, 0x04);
  	*data = SpiReadByte();

	DisableFlash(flash_type);

	return 1;



}
int Flash_WriteByte(unsigned char flash_type,
                    unsigned int  address,
                    unsigned char data)
{
  unsigned char command[4] = {0};
  unsigned char readByte = 0;

  if(flash_type > DATA_FLASH)
    return -1;

	if(address >= FLASH_SIZE)
	  return -2;

	command[0] = FLASH_PP;
	command[1] = (address & 0xFF0000) >> 16;
	command[2] = (address & 0xFF00) >> 8;
	command[3] = address & 0xFF;
	
   Flash_EnableWrite(flash_type);
  
	EnableFlash(flash_type);

	SpiWriteBuffer(command, 0x04);
	SpiWriteByte(data);

	DisableFlash(flash_type);

	Flash_WaitForWrite(flash_type);
	
	Flash_ReadByte(flash_type, address, &readByte);
	
	if(readByte != data)
	  return -4;
	
	return 1;
}






int Flash_BulkErase(unsigned char flash_type)
{
  if(flash_type > DATA_FLASH)
    return -1;

  Flash_EnableWrite(flash_type);
 
  EnableFlash(flash_type);
  
  SpiWriteByte(FLASH_BE);
  
  DisableFlash(flash_type);
  
  Flash_WaitForWrite(flash_type);
  return 0;
}

int Flash_SetPowerMode(unsigned char flash_type,
                       unsigned char mode)
{
  unsigned char command[4] = {0};
  int eSignature = 0;
  
  if(flash_type > DATA_FLASH)
    return -1;
  
  if(mode > FLASH_RELEASE_DEEP_POWER_DOWN)
    return -5;

  EnableFlash(flash_type);

  if(mode == FLASH_DEEP_POWER_DOWN)
  {
    SpiWriteByte(FLASH_DP);
    eSignature = 0;
  }
  else
  {
    command[0] = FLASH_RDP;
    command[1] = 0;
    command[2] = 0;
    command[3] = 0;
    SpiWriteBuffer(command, 0x04);
    eSignature = SpiReadByte() & 0xFF;
  }

  DisableFlash(flash_type);
  return eSignature;
}

int Flash_ReadID(unsigned char flash_type, unsigned char *flashID)
{ 
  if(flash_type > DATA_FLASH)
    return -1;

    DisableFlash(flash_type);
  EnableFlash(flash_type);
  
  SpiWriteByte(FLASH_RDID);
  flashID[0] = SpiReadByte();
  flashID[1] = SpiReadByte();
  flashID[2] = SpiReadByte();
  
  DisableFlash(flash_type);
  return 0;
}

*/











