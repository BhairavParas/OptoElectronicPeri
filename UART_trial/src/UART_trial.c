/*****************************************************************************
 * UART_trial.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "UART_trial.h"
#include "uart_api.h"
#include "StdSharc.h"
/** 
 * If you want to use command program arguments, then place them in the following string. 
 */

char debugBuffer[DEBUG_BUFFER_SIZE] = "Debug buffer data\r\n";
char deblurBuffer[DEBUG_BUFFER_SIZE] = "Deblur buffer data";
char spareBuffer[DEBUG_BUFFER_SIZE] = "spare buffer data";
char readData[DEBUG_BUFFER_SIZE] = {0};

int main(void)
{
	/**
	 * Initialize managed drivers and/or services that have been added to 
	 * the project.
	 * @return zero on success 
	 */

	adi_initComponents();
	/*****Configure clock for ADSP21369*****/
	Init_PLL();
	/***************************************/


	/*****Maps peripherals to external pins*****/
	Init_SRU();
	/*******************************************/

	/* Begin adding your custom code here */

	/* Initiate the Serial communication */
	Init_Serial();



	while(1){

		/*Prints*/
		WriteData_SPort(DebugPort, "Its Debug Print \r\n", 18);
		WriteData_SPort(DebugPort, (unsigned char *)debugBuffer,18);

		delay(1000); // create delay in ms
#if 0
		WriteData_SPort(DeblurPort, "Its DeblurPort Print \r\n", 18);
		WriteData_SPort(DeblurPort, (unsigned char *)deblurBuffer,10);
		//delay(100); // create delay in ms

		WriteData_SPort(SPARE1, "Its SPARE Print \r\n", 18);
		WriteData_SPort(SPARE1, (unsigned char *)spareBuffer,10);
		//delay(1000); // create delay in ms
#endif
	}

	return 0;
}


static void Init_Serial(void)
{

/******************************UART1 DEBUG PORT*******************************/
	SerialPort_Configure(&DebugPort, 0x4000,UARTWLS8,0,UART_NO_PARITY,115200);
/***************************UART0 DEBLUR CARD PORT****************************/
	SerialPort_Configure(&DeblurPort,0x3C00,UARTWLS8,0,UART_NO_PARITY,115200);
/*****************************************************************************/
/**********************************QUART UART*********************************/
	SerialPort_Configure(&SPARE1,0x200010,UARTWLS8,0,UART_NO_PARITY,115200);
/*****************************************************************************/
  //SerialPort_Configure(&RL_STB,   0x200008,   UARTWLS8, 0, UART_ODD_PARITY,115200);// 460800);//RL 422
  //SerialPort_Configure(&INS_SEN,   0x200018,   UARTWLS8, 0, UART_ODD_PARITY, 115200);//ins sen 232
  //SerialPort_Configure(&OPT_MMI,   0x200020,   UARTWLS8, 0, UART_ODD_PARITY, 115200);//mmi 422 // 230400
  SetSerialFIFOTrigger(&DebugPort,1);
  SetSerialFIFOTrigger(&DeblurPort,1);
  SetSerialFIFOTrigger(&SPARE1,1);
  //SetSerialFIFOTrigger(&RL_STB,    1);
  //SetSerialFIFOTrigger(&INS_SEN,   1);
  //SetSerialFIFOTrigger(&OPT_MMI,   1);

}
static void delay(uint32_t delayCount)
{
	delayCount = 26315.789 * delayCount;

  /* delayCount = 1 => 38ns delay */
  while(delayCount--);
}

static void Init_PLL(void)
{
  int iIndex = 0;
  *pPMCTL =	PLLBP | PLLM21 | INDIV  |DIVEN ;
  for(iIndex = 0; iIndex < 65535; iIndex ++);
  *pPMCTL &= ~(PLLBP);
}
static void Init_SRU(void)
{
/**************UART1 Configure***************/
	SRU2(UART1_TX_O,DPI_PB11_I);
	SRU2(HIGH,DPI_PBEN11_I);

	SRU2(DPI_PB12_O,UART1_RX_I);
	SRU2(LOW,DPI_PB12_I);
	SRU2(LOW,DPI_PBEN12_I);
/********************************************/

/**************UART0 Configure***************/
	SRU2(UART0_TX_O,DPI_PB09_I);
	SRU2(HIGH,DPI_PBEN09_I);

	SRU2(DPI_PB10_O,UART0_RX_I);
	SRU2(LOW,DPI_PB10_I);
	SRU2(LOW,DPI_PBEN10_I);
/********************************************/

/***************SPI Configure****************/
    SRU2(SPI_MOSI_O,DPI_PB01_I);
	SRU2(HIGH,DPI_PBEN01_I);

	SRU2(DPI_PB02_O,SPI_MISO_I);
	SRU2(LOW, DPI_PB02_I);
	SRU2(LOW,DPI_PBEN02_I);

	SRU2(SPI_CLK_O,DPI_PB03_I);
	SRU2(HIGH,DPI_PBEN03_I);
/********************************************/

/***************SPIB Configure***************/
	SRU2(DPI_PB06_O,SPIB_MISO_I);
	SRU2(LOW,DPI_PB06_I);
	SRU2(LOW,DPI_PBEN06_I);

	SRU(SPIB_MOSI_O,DPI_PB07_I);
	SRU2(HIGH,DPI_PB07_I);

	SRU2(SPIB_CLK_O,DPI_PB08_I);
	SRU2(HIGH,DPI_PB08_I);
/********************************************/

/**************TIMER0 Configure**************/
	SRU2(TIMER0_O,DPI_PB13_I);
	SRU2(HIGH,DPI_PBEN13_I);
/********************************************/

/**************TIMER1 Configure**************/
	SRU2(TIMER1_O,DPI_PB14_I);
	SRU2(HIGH,DPI_PBEN14_I);
/********************************************/

/**************BTFLSH Configure**************/
	SRU2(SPI_FLG0_O,DPI_PB05_I);
	SRU2(HIGH,DPI_PBEN05_I);
	SRU2(HIGH,DPI_PB05_I);
/********************************************/

/********************************************/
	SRU(HIGH,DAI_PB08_I);
	SRU(HIGH,PBEN08_I);
/********************************************/

/**************SPORT1 Configure**************/
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
/********************************************/

/**************SPORT1 Configure**************/
	SRU(SPORT3_DA_O,DAI_PB11_I);
	SRU(HIGH,PBEN11_I);

	SRU(SPORT3_DB_O,DAI_PB12_I);
	SRU(HIGH,PBEN12_I);

	SRU(SPORT3_CLK_O,DAI_PB13_I);
	SRU(HIGH,PBEN13_I);

	SRU(SPORT3_FS_O,DAI_PB14_I);
	SRU(HIGH,PBEN14_I);
/********************************************/

/**************SPORT1 Configure**************/
	SRU(DAI_PB17_O,SPORT5_DA_I);
	SRU(LOW,DAI_PB17_I);
	SRU(LOW,PBEN17_I);

	SRU(DAI_PB18_O,SPORT5_DB_I);
	SRU(LOW,DAI_PB18_I);
	SRU(LOW,PBEN18_I);

	SRU(DAI_PB19_O,SPORT5_CLK_I);
	SRU(LOW,DAI_PB19_I);
	SRU(LOW,PBEN19_I);

	SRU(DAI_PB20_O,SPORT5_FS_I);
	SRU(LOW,DAI_PB20_I);
	SRU(LOW,PBEN20_I);
/********************************************/

/*****System Control Register Configure******/
	*pSYSCTL &= ~(0x1F0000);
/********************************************/

	sysreg_bit_set(sysreg_FLAGS, FLG0O);
	sysreg_bit_set(sysreg_FLAGS, FLG0);

	sysreg_bit_set(sysreg_FLAGS, FLG1O);
	sysreg_bit_set(sysreg_FLAGS, FLG1);

	sysreg_bit_set(sysreg_FLAGS, FLG2O);
	sysreg_bit_set(sysreg_FLAGS, FLG2);

	sysreg_bit_set(sysreg_FLAGS, FLG3O);
	sysreg_bit_set(sysreg_FLAGS,FLG3);

/************Spare1IntConfigure**************/
	SRU(DAI_PB02_O,MISCA0_I);
	SRU(LOW,DAI_PB02_I);
	SRU(LOW,PBEN02_I);
/********************************************/

/**************RollIntConfigure**************/
	SRU(DAI_PB16_O,MISCA1_I);
	SRU(LOW,DAI_PB16_I);
	SRU(LOW,PBEN16_I);
/********************************************/

/*************MMIIntConfigure****************/
    SRU(DAI_PB09_O,MISCA2_I);
	SRU(LOW,DAI_PB09_I);
	SRU(LOW,PBEN09_I);
/********************************************/

/**************InsIntConfigure***************/
	SRU(DAI_PB10_O,MISCA3_I);
	SRU(LOW,DAI_PB10_I);
	SRU(LOW,PBEN10_I);
/********************************************/
	SRU(LOW,DAI_PB03_I);
	SRU(LOW,PBEN03_I);

	SRU(HIGH,DAI_PB01_I);
	SRU(HIGH,PBEN01_I);

    SRU(HIGH,DAI_PB04_I);
	SRU(HIGH,PBEN04_I);

	SRU2(DPI_PB07_O,MISCB2_I);
    SRU2(LOW,DPI_PBEN07_I);
	SRU2(HIGH,DPI_PBEN08_I);
}

