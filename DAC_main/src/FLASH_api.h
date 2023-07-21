#ifndef __FLASH_H
#define __FLASH_H
/**
 * Flash memory details
 */ 

#define FLASH_SIZE            4194304 				//2097152// 16 Mb (2 MBytes)
#define FLASH_PAGE_SIZE       256                  // 256 bytes
#define FLASH_SECTOR_SIZE     65536                // 65536 bytes

#define FLASH_NUM_SECTORS          (FLASH_SIZE / FLASH_SECTOR_SIZE)
#define FLASH_NUM_PAGES            (FLASH_SIZE / FLASH_PAGE_SIZE)
#define FLASH_NUM_PAGES_PER_SECTOR (FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE)

#define FLASH_DEEP_POWER_DOWN         0x00
#define FLASH_RELEASE_DEEP_POWER_DOWN 0x01

#define BOOT_FLASH 0
#define DATA_FLASH 1

/**
 * Flash instructions
 */
 
#define FLASH_WREN         0x06       // Write enable
#define FLASH_WRDI         0x04       // Write disable
#define FLASH_RDID         0x9F       // Read Identification
#define FLASH_RDSR         0x05       // Read status register
#define FLASH_WRSR         0x01       // Write status register
#define FLASH_READ         0x03       // Read data bytes
#define FLASH_FAST_READ    0x0B       // Fast read
#define FLASH_PW           0x0A       // Page write
#define FLASH_PP           0x02       // Page program
#define FLASH_SE           0xD8       // Sector erase
#define FLASH_BE           0xC7       // Bulk erase
#define FLASH_DP           0xB9       // Deep power-down
#define FLASH_RDP          0xAB       // Release from deep power-down

/**
 * Exported APIs
 */ 
 
void Flash_Write(unsigned char flash_type, int address, unsigned char *data, int length);
void Flash_Read(unsigned char flash_type, int address, unsigned char *data, int length);
void Flash_Init(void);
                   
int Flash_ReadByte(unsigned char flash_type,
                   unsigned int  address,
                   unsigned char *data);

int Flash_WriteByte(unsigned char flash_type,
                    unsigned int  address,
                    unsigned char data);
int Flash_ReadBuffer(unsigned char flash_type,
                     unsigned int  address,
                     unsigned char *data,
                     unsigned int  length);

int Flash_WriteBuffer(unsigned char flash_type,
                      unsigned int  address,
                      unsigned char *data,
                      unsigned int  length);

int Flash_SectorErase(unsigned char flash_type,
                      unsigned int sectorID);

int Flash_BulkErase(unsigned char flash_type);

int Flash_SetPowerMode(unsigned char flash_type,
                       unsigned char mode);

int Flash_ReadID(unsigned char flash_type, unsigned char *flashID);
unsigned char Flash_ReadStatusReg(unsigned char flash_type);
void Flash_EnableWrite(unsigned char flash_type);
void Flash_WaitForWrite(unsigned char flash_type);

//void SpiInitForFlash(void);
void Init_SPI_Flash(void);

void SpiInitForDAC(void);

#endif // __FLASH_H
