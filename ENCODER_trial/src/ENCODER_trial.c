/*****************************************************************************
 * ENCODER_trial.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "ENCODER_trial.h"
#include "ENC_api.h"
#include "StdSharc.h"

void main(void)
{
	/**
	 * Initialize managed drivers and/or services that have been added to 
	 * the project.
	 * @return zero on success 
	 */
	adi_initComponents();
	
	/* Begin adding your custom code here */
	/*****Configure clock for ADSP21369*****/
		Init_PLL();
	/***************************************/

	/*****Maps peripherals to external pins*****/
		Init_SRU();
	/*******************************************/

	/*****Configure parallel port interface*****/
		Init_ParallelPort();
	/*******************************************/

	/*****Configure software timer*****/
		Init_Timer();
	/**********************************/
	/*****Configure periphral interrupts*****/
		//Init_Interrupts();
	/****************************************/
		/*
		Sport3_Init();
		Sport5_Init();
		Sport1_Init();
		*/

	/*****Configure sport interface*****/
		//Init_Sport();
	/***********************************/


		Timer_Start();

	while (1){
		ReadEncoderData();
		//delay(1);
	}
}

void delay(unsigned int delayCount)
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

static void Init_ParallelPort(void)
{
	*pAMICTL0 = 0x00289FCD;
	*pEPCTL &= ~(B0SD |B1SD | B2SD);
}
static void Init_Timer()
{
	*pTM0CTL=0x00;
	*pTM0CTL=TIMODEPWM | IRQEN  |PULSE | PRDCNT;

	//*pTM0PRD=0x8032;
	*pTM0PRD=0x802C;
	*pTM0W=0x4016;
/*
	*pTM0PRD=0x4016;//4KHz
	*pTM0W=0x200B;
*/
	*pTMSTAT=TIM0DIS;
	*pTMSTAT=TIM0EN;
}

static void Timer_Start()
{
	*pTMSTAT=TIM0DIS;
	*pTMSTAT=TIM0EN;
}
#if 0
static void Init_Interrupts(void)
{
  *pPICR2 &= ~(0x3E0);
  *pPICR2 |= (0x13<<5);
  adi_int_InstallHandler(ADI_CID_P13I,(ADI_INT_HANDLER_PTR)UART0_RxIsr,NULL,true);

  *pPICR3 &= ~(0xFF);
  *pPICR3 |= 0x14;
  adi_int_InstallHandler(ADI_CID_P18I,(ADI_INT_HANDLER_PTR)UART1_RxIsr,NULL,true);

  *pDAI_IRPTL_PRI = SRU_EXTMISCA0_INT;
  *pDAI_IRPTL_RE = SRU_EXTMISCA0_INT;

  *pPICR0 &=~0x7C00;
  adi_int_InstallHandler(ADI_CID_P2I,(ADI_INT_HANDLER_PTR)QUART_RxIsr,NULL,true);


  *pPICR0 |= (0x02 << 0);
  adi_int_InstallHandler(ADI_CID_P0I,(ADI_INT_HANDLER_PTR)IsrTimer0,NULL,true);

  sysreg_bit_set(sysreg_MODE1,IRPTEN);
}

static void Init_Sport(void)
{
	InitSport3();
	InitSport5();
	InitSport1();
}
void InitSport3(void)
{

	*pDIV3 = 0x00080003;
	*pSPCTL3 = SPEN_A | SLEN8 | FSR | IFS | BHD | SPTRAN | ICLK | LAFS; //transmitter //transmitter
	//*pSPCTL3 &=0xFFFFFE7F;
}
void InitSport5(void)
{
	m_auisport5_tcb[0]=((unsigned int)(m_auisport5_tcb + LAST_ELMNT)) & PCI_MASK; // make TCB to point itself
	//*pSPCTL5 = SPEN_A | SLEN8 | FSR | BHD ; //receiver
	*pCPSP5A = ((unsigned int)(m_auisport5_tcb + LAST_ELMNT)) & PCI_MASK;
	*pDIV1 = 0x00080003;
	*pSPCTL5 = SPEN_A | SLEN8 | FSR | BHD | SDEN_A | SCHEN_A | LAFS; //receiver

}
void InitSport1(void)
{
	*pDIV1 = 0x00080003;
	*pSPCTL1 =SPEN_A| SPEN_B |SLEN16 | FSR | LAFS| BHD |IFS | ICLK | LFS  ; //receiver
}
#endif
