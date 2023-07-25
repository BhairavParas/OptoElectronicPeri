/*
 * Int_Test.c
 *
 *  Created on: 06-Feb-2017
 *      Author: arjun.k
 */
////header files//

#include "StdSharc.h"
//#include "Loop250us.h"
//#include "sync_sport.h"
//#include "spi.h"
//#include "dac.h"
//#include "Loop250us.h"
//#include "interrupt.h"
//#include "flash.h"
#include <services/int/adi_int.h>
#include "UART_FOV_api.h"
#include "INIT_FOV_api.h"
union Char2Float
{
	char CharValue;
	float FloatValue;
};
union Char2Float MemoryData;


float TiFovEncoderOffset;
float CcdWnFovEncoderOffset;
float CcdMFovEncoderOffset;


void IsrTimer0(int sig);
//void IsrDpi(int sig);
void InitApp(void)
{
  Init_PLL();
  Init_SRU();
  //Init_ParallelPort();
  Init_Serial();
  //Init_Timer();
  //Init_Interrupts();
  //SpiInit();//For boot flash,data flash and DAC
  //DacSpiConfigure();
 // Sport1_Init();//For ADC
 // ServoSensorInitialization();
  //Start_Timer();
  //SpiBInit();//For temperature sensor
  //Sport3_Init();
  //Sport5_Init();
}

#if 0
static void ServoSensorInitialization(void)
{
	unsigned char CalibDataFromFlash[4]={0};
	int x;

	sysreg_bit_clr(sysreg_MODE1,IRPTEN);

	x=Flash_ReadBuffer(DATA_FLASH,0x00000,CalibDataFromFlash,4);
	MemoryData.CharValue=(CalibDataFromFlash[3]<<24)|(CalibDataFromFlash[2]<<16)|(CalibDataFromFlash[1]<<8)|(CalibDataFromFlash[0]);
	TiFovEncoderOffset=MemoryData.FloatValue;

	x=Flash_ReadBuffer(DATA_FLASH,0x010000,CalibDataFromFlash,4);
	MemoryData.CharValue=(CalibDataFromFlash[3]<<24)|(CalibDataFromFlash[2]<<16)|(CalibDataFromFlash[1]<<8)|(CalibDataFromFlash[0]);
	CcdWnFovEncoderOffset=MemoryData.FloatValue;

	x=Flash_ReadBuffer(DATA_FLASH,0x020000,CalibDataFromFlash,4);
	MemoryData.CharValue=(CalibDataFromFlash[3]<<24)|(CalibDataFromFlash[2]<<16)|(CalibDataFromFlash[1]<<8)|(CalibDataFromFlash[0]);
	CcdMFovEncoderOffset=MemoryData.FloatValue;
	sysreg_bit_set(sysreg_MODE1,IRPTEN);
}

#endif

static void Init_PLL(void)
{
  int iIndex = 0;
  *pPMCTL =	PLLBP | PLLM21 | INDIV  |DIVEN ;
  for(iIndex = 0; iIndex < 65535; iIndex ++);
  *pPMCTL &= ~(PLLBP);
}

static void Init_Serial(void)
{
	/*CHIP UART*/
  SerialPort_Configure(&DebugPort, 0x4000,     UARTWLS8, 0, UART_NO_PARITY, 115200);//UART-1 422
  SerialPort_Configure(&GciPort,   0x3C00,     UARTWLS8, 0, UART_NO_PARITY, 115200);//DEBLUR CARD 232UART-0

  /*QUART*/

  /*SerialPort_Configure(&SPARE1,   0x200008,   UARTWLS8, 0, UART_ODD_PARITY,115200);// 460800);//RL 422
	SerialPort_Configure(&SPARE2,  0x200010,   UARTWLS8, 0, UART_ODD_PARITY, 115200);//spare 422
	SerialPort_Configure(&SPARE3,   0x200018,   UARTWLS8, 0, UART_ODD_PARITY, 115200);//ins sen 232
	SerialPort_Configure(&SPARE4,   0x200020,   UARTWLS8, 0, UART_ODD_PARITY, 115200);//mmi 422

	SetSerialFIFOTrigger(&debugPort, 1);
	SetSerialFIFOTrigger(&DBLR_Fov,   1);
	SetSerialFIFOTrigger(&SPARE1,   1);
	SetSerialFIFOTrigger(&SPARE2,  1);
	SetSerialFIFOTrigger(&SPARE3,   1);
	SetSerialFIFOTrigger(&SPARE4,   1);*/
}
static void Init_SRU(void)
{

	/********Configure UART1********/
	SRU2(UART1_TX_O,DPI_PB11_I);//UART1 TX
	SRU2(HIGH,DPI_PBEN11_I);
	SRU2(DPI_PB12_O,UART1_RX_I);//UART1 RX
	SRU2(LOW,DPI_PB12_I);
	SRU2(LOW,DPI_PBEN12_I);
	/********Configure UART1********/


	/********Configure UART0********/
	SRU2(UART0_TX_O,DPI_PB09_I);  //UART0 TX
	SRU2(HIGH,DPI_PBEN09_I);
	SRU2(DPI_PB10_O,UART0_RX_I);  //UART0 RX
	SRU2(LOW,DPI_PB10_I);
	SRU2(LOW,DPI_PBEN10_I);
	/********Configure UART0********/


	/********Configure SPI0********/
	SRU2(SPI_MOSI_O,DPI_PB01_I);
	SRU2(HIGH,DPI_PBEN01_I);
	SRU2(DPI_PB02_O,SPI_MISO_I);
	SRU2(LOW, DPI_PB02_I);
	SRU2(LOW,DPI_PBEN02_I);
	SRU2(SPI_CLK_O,DPI_PB03_I);
	SRU2(HIGH,DPI_PBEN03_I);
	/********Configure SPI0********/


	/********Configure SPI1********/
	SRU(SPIB_MOSI_O,DPI_PB07_I);
	SRU2(HIGH,DPI_PBEN07_I);
	SRU2(DPI_PB06_O,SPIB_MISO_I);
	SRU2(LOW,DPI_PB06_I);
	SRU2(LOW,DPI_PBEN06_I);
	SRU2(SPIB_CLK_O,DPI_PB08_I);
	SRU2(HIGH,DPI_PBEN08_I);
	/********Configure SPI1********/


	/********Configure TIMER0********/
	SRU2(TIMER0_O,DPI_PB13_I);
	SRU2(HIGH,DPI_PBEN13_I);
	/********Configure TIMER0********/


	/********Configure BootFlash CS********/
	SRU2(SPI_FLG0_O,DPI_PB05_I);
	SRU2(HIGH,DPI_PBEN05_I);
	SRU2(HIGH,DPI_PB05_I);
	/********Configure BootFlash CS********/

	/********Configure ADC CS********/
	SRU(HIGH,DAI_PB08_I);
	SRU(HIGH,PBEN08_I);
	SRU(HIGH,DAI_PB04_I);
	SRU(HIGH,PBEN04_I);
	/********Configure ADC CS********/


	/********Configure SPORT1 ADC********/
	SRU(DAI_PB05_O,SPORT1_DA_I);
	SRU(LOW,DAI_PB05_I);
	SRU(LOW,PBEN05_I);
	SRU(DAI_PB06_O,SPORT1_DB_I);
	SRU(LOW,DAI_PB06_I);
	SRU(LOW,PBEN06_I);
	SRU(SPORT1_CLK_O,DAI_PB07_I);
	SRU(HIGH,PBEN07_I);
	SRU(SPORT1_FS_O,DAI_PB08_I);
	SRU(HIGH,PBEN08_I);
	/********Configure SPORT1 ADC********/


	/********Configure SPORT5********/
	SRU(SPORT5_DA_O,DAI_PB17_I);
	SRU(HIGH,PBEN17_I);
	SRU(SPORT5_DB_O,DAI_PB18_I);
	SRU(HIGH,PBEN18_I);
	SRU(SPORT5_CLK_O,DAI_PB19_I);
	SRU(HIGH,PBEN19_I);
	SRU(SPORT5_FS_O,DAI_PB20_I);
	SRU(HIGH,PBEN20_I);
	/********Configure SPORT5********/


	/********Configure SPORT3********/
	SRU(DAI_PB11_O,SPORT3_DA_I);
	SRU(LOW,DAI_PB11_I);
	SRU(LOW,PBEN11_I);
	SRU(DAI_PB12_O,SPORT3_DB_I);
	SRU(LOW,DAI_PB12_I);
	SRU(LOW,PBEN12_I);
	SRU(DAI_PB13_O,SPORT3_CLK_I);
	SRU(LOW,DAI_PB13_I);
	SRU(LOW,PBEN13_I);
	SRU(DAI_PB14_O,SPORT3_FS_I);
	SRU(LOW,DAI_PB14_I);
	SRU(LOW,PBEN14_I)
	/********Configure SPORT3********/


	/********Routing Interrupts********/
	SRU(DAI_PB02_O,MISCA0_I);
	SRU(LOW,DAI_PB02_I);
	SRU(LOW,PBEN02_I);

	SRU(DAI_PB16_O,MISCA1_I);
	SRU(LOW,DAI_PB16_I);
	SRU(LOW,PBEN16_I);

    SRU(DAI_PB09_O,MISCA2_I);
	SRU(LOW,DAI_PB09_I);
	SRU(LOW,PBEN09_I);

	SRU(DAI_PB10_O,MISCA3_I);
	SRU(LOW,DAI_PB10_I);
	SRU(LOW,PBEN10_I);

	SRU(DAI_PB03_O,MISCA4_I);
	SRU(LOW,DAI_PB03_I);
	SRU(LOW,PBEN03_I);
	/********Routing Interrupts********/


	*pSYSCTL &= ~(0x1F0000);
	sysreg_bit_set(sysreg_FLAGS, FLG0O);
	sysreg_bit_set(sysreg_FLAGS, FLG0);
	sysreg_bit_set(sysreg_FLAGS, FLG1O);
	sysreg_bit_set(sysreg_FLAGS, FLG1);
	sysreg_bit_set(sysreg_FLAGS, FLG2O);
	sysreg_bit_set(sysreg_FLAGS, FLG2);
	sysreg_bit_set(sysreg_FLAGS, FLG3O);
	sysreg_bit_set(sysreg_FLAGS,FLG3);


	/********Configure Limit Switch interrupt********/
	SRU2(DPI_PB14_O,MISCB0_I);
//	SRU2(LOW,DPI_PB14_I);
	SRU2(LOW,DPI_PBEN14_I);
	/********Configure Limit Switch interrupt********/
}
#if 0
static void Init_Interrupts(void)
{
	*pPICR0 |= (0x02 << 0);
	adi_int_InstallHandler(ADI_CID_P0I,(ADI_INT_HANDLER_PTR)IsrTimer0,NULL,true);

	*pPICR2 |= (0x0E << 10);              //LIMIT SWITCH MOSI2    AZMAX LIM
	 *pDPI_IRPTL_FE |=  EXT_MISC_0_INT;
	//*pPICR0 |= (0x0E << 5);
	//*pDPI_IRPTL_FE |=  EXT_MISC_0_INT;
	adi_int_InstallHandler(ADI_CID_P14I,(ADI_INT_HANDLER_PTR)IsrDpi,NULL,true);

	*pPICR2 &= ~(0x3E0);
	*pPICR2 |= (0x13<<5);
	adi_int_InstallHandler(ADI_CID_P13I,(ADI_INT_HANDLER_PTR)UART0RxIsr,NULL,true);

	*pPICR3 &= ~(0xFF);
	*pPICR3 |= 0x14;
	adi_int_InstallHandler(ADI_CID_P18I,(ADI_INT_HANDLER_PTR)UART1RxIsr,NULL,true);

//	*pDAI_IRPTL_PRI = (SRU_EXTMISCA0_INT | SRU_EXTMISCA1_INT | SRU_EXTMISCA2_INT | SRU_EXTMISCA3_INT);
//	*pDAI_IRPTL_RE = (SRU_EXTMISCA0_INT | SRU_EXTMISCA1_INT | SRU_EXTMISCA2_INT | SRU_EXTMISCA3_INT);
//	*pPICR0 = 0x0000;
//	*pPICR0 |= (0x00 << 10);
  //adi_int_InstallHandler(ADI_CID_P2I,(ADI_INT_HANDLER_PTR)QUART_RxIsr,NULL,true);

  sysreg_bit_set(sysreg_MODE1,IRPTEN);
}

static void Init_ParallelPort(void)
{
	*pAMICTL0 = 0x002897CD;
	*pEPCTL &= ~(B0SD |B1SD | B2SD);
}

static void Init_Timer()
{
	*pTM0CTL=0x00;
	*pTM0CTL=TIMODEPWM | IRQEN  |PULSE | PRDCNT;
	*pTM0PRD=0x4016;
	*pTM0W=0x2008;
	//*pTMSTAT=TIM0DIS;
	//*pTMSTAT=TIM0EN;
}

static void Start_Timer()
{
	*pTMSTAT=TIM0DIS;
	*pTMSTAT=TIM0EN;
}

void IsrTimer0(int sig)
{
	*pTMSTAT |=TIM0IRQ;
	*pTMSTAT |=TIM0DIS;
	*pTMSTAT |=TIM0EN;
	 Loop();
}



#endif

