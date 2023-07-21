#ifndef __SPI_H
#define __SPI_H

void SpiInit(void);

unsigned char SpiReadByte(void);
unsigned short SpiReadWord(void);

void SpiReadBuffer(unsigned char *readBuffer, unsigned short count);
void SpiReadWordBuffer(unsigned short *readBuffer, unsigned short count);

void SpiWriteByte(unsigned char ch);
void SpiWriteWord(unsigned short word);

void SpiWriteBuffer(unsigned char *writeBuffer, unsigned short count);
void SpiWriteWordBuffer(unsigned short *writeBuffer, unsigned short count);

/************************************************************************/

void SpiBInit(void);

unsigned char SpiBReadByte(void);
unsigned short SpiBReadWord(void);

void SpiBReadBuffer(unsigned char *readBuffer, unsigned short count);
void SpiBReadWordBuffer(unsigned short *readBuffer, unsigned short count);

void SpiBWriteByte(unsigned char ch);
void SpiBWriteWord(unsigned short word);

void SpiBWriteBuffer(unsigned char *writeBuffer, unsigned short count);
void SpiBWriteWordBuffer(unsigned short *writeBuffer, unsigned short count);

#endif //__SPI_H
