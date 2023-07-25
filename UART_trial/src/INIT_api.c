#include "INIT_api.h"

#include "StdSharc.h"
//#include "submarine.h"
//#include "encoder.h"
#include "uart_api.h"
//#include "discreteIO.h"
//#include "loop500us.h"
//#include "dac.h"
//#include "flash.h"
//#include "sync_sport.h"
//#include "spi.h"
//#include "fsm.h"
//#include "debug.h"
#include <services/int/adi_int.h>


//union Float2Char MemoryRead;

//extern Sport DeblurPort; //On-chip UART0
//extern Sport DebugPort;  //On-chip UART1
//extern Sport SPARE1;   	 //SPARE1 QUart1 - B

float AzHarmonizeOffsetFlash=0;
float ElHarmonizeOffsetFlash=0;
float AzTiltOffsetFlash=0;
float ElTiltOffsetFlash=0;

/********FUNCTION DECLERATION********/

//extern int INAZMOTORON;
//extern int INELMOTORON;
//extern int OUTAZMOTORON;

void Init_HW(void)
{
/*****Configure clock for ADSP21369*****/
	Init_PLL();
/***************************************/

/*****Maps peripherals to external pins*****/
	Init_SRU();
/*******************************************/

/*****Configure parallel port interface*****/
	//Init_ParallelPort();
/*******************************************/

/*****Configure serial UART interfaces*****/
	Init_Serial();
/****************************************/

/*****Configure SPI interface for DAC*****/
	//Init_SPI_DAC();
	//DacSpiConfigure();
/*****************************************/

/*****Configure software timer*****/
	//Init_Timer();
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

//	SRU2(HIGH,DPI_PB08_I);

/*****Configure SPI for flash*****/
	//Init_SPI_Flash();
	//SpiInitForFlash();
/*********************************/

/*****Initialize servo sensors*****/
	//ServoSensorInitialization();
	//Init_Servo_Sensors();
/**********************************/
	//Init_FlagVariable();

	//for(InitTimerCount=0;InitTimerCount<50000000;InitTimerCount++);
	//POST();
	//Discrete_SetStateOfOutputLine(1<<0,0);
	//Periscope.Flag.INAZMOTORON = false;
	//INAZMOTORON = false;
	//Write_DAC(0,0);

	//Discrete_SetStateOfOutputLine(1<<1,0);
	//Periscope.Flag.INELMOTORON = false;
	//INELMOTORON = false;
	//Write_DAC(1,0);

	//Discrete_SetStateOfOutputLine1(1<<0,0);//Pin54(J5)-13//working
	//Discrete_SetStateOfOutputLine1(1<<1,0);//Pin37(J5)-15//working
//	Periscope.Flag.OUTAZMOTORON = false;
	//OUTAZMOTORON = false;
	//Write_DAC(4,0);

	//Timer_Start();
	//Init_Fsm();
}

#if 0
static void Init_Sport(void)
{
	InitSport3();
	InitSport5();
	InitSport1();
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

bool POST(void)
{
	bool SupplyHealth, SensorData, MotorCurrent;
	SupplyHealth = SupplyHealthStatus();
	SensorData = SensorDataStatus();
	MotorCurrent = MotorsCurrentStatus();
	if(SupplyHealth && SensorData && MotorCurrent)
		return true;
	else
		return false;
}


void IsrTimer0(int sig)
{
	*pTMSTAT |=TIM0IRQ;
	*pTMSTAT |=TIM0DIS;
	*pTMSTAT |=TIM0EN;
	Loop500us();
}

static void Init_Servo_Sensors(void)
{
	unsigned char CalibDataFromFlash[4] = {0};
	int x;

	InAzOffsetI  = INAZENCOFFSET;
	InElOffsetI  = INELENCOFFSET;
	OutAzOffsetI = OUTAZENCOFFSET;

	sysreg_bit_clr(sysreg_MODE1,IRPTEN);

	x = Flash_ReadBuffer(DATA_FLASH,0x00000,CalibDataFromFlash,4);
	MemoryRead.CharValue = (CalibDataFromFlash[3]<<24)|(CalibDataFromFlash[2]<<16)|(CalibDataFromFlash[1]<<8)|(CalibDataFromFlash[0]);
	GyroXDrftF = MemoryRead.FloatValue;

	x = Flash_ReadBuffer(DATA_FLASH,0x00100,CalibDataFromFlash,4);
	MemoryRead.CharValue = (CalibDataFromFlash[3]<<24)|(CalibDataFromFlash[2]<<16)|(CalibDataFromFlash[1]<<8)|(CalibDataFromFlash[0]);
	GyroYDrftF = MemoryRead.FloatValue;

	x = Flash_ReadBuffer(DATA_FLASH,0x00200,CalibDataFromFlash,4);
	MemoryRead.CharValue = (CalibDataFromFlash[3]<<24)|(CalibDataFromFlash[2]<<16)|(CalibDataFromFlash[1]<<8)|(CalibDataFromFlash[0]);
	InAzOffsetF = MemoryRead.FloatValue;

	x = Flash_ReadBuffer(DATA_FLASH,0x00300,CalibDataFromFlash,4);
	MemoryRead.CharValue = (CalibDataFromFlash[3]<<24)|(CalibDataFromFlash[2]<<16)|(CalibDataFromFlash[1]<<8)|(CalibDataFromFlash[0]);
	InElOffsetF = MemoryRead.FloatValue;

	x = Flash_ReadBuffer(DATA_FLASH,0x00400,CalibDataFromFlash,4);
	MemoryRead.CharValue = (CalibDataFromFlash[3]<<24)|(CalibDataFromFlash[2]<<16)|(CalibDataFromFlash[1]<<8)|(CalibDataFromFlash[0]);
	OutAzOffsetF = MemoryRead.FloatValue;

	GyroXDrft = GyroXDrftF;
	GyroYDrft = GyroYDrftF;

	InAzOffset = InAzOffsetI + InAzOffsetF;
	InElOffset = InElOffsetI + InElOffsetF;
	OutAzOffset = OutAzOffsetI + OutAzOffsetF;

	sysreg_bit_set(sysreg_MODE1,IRPTEN);
}

static void Init_Fsm(void)
{
	FsmPortInitBuffer[0]=0xAA;
	FsmPortInitBuffer[1]=0x0A;
	FsmPortInitBuffer[2]=0x00;
	FsmPortInitBuffer[3]=0x00;
	FsmPortInitBuffer[4]=0x00;
	FsmPortInitBuffer[5]=0x00;
	FsmPortInitBuffer[6]=0x00;
	FsmPortInitBuffer[7]=0x00;
	FsmPortInitBuffer[8]=0x0A;
	FsmPortInitBuffer[9]=0xEE;
	DebugPrint(SPARE1,"%c%c%c%c%c%c%c%c%c%c",
			FsmPortInitBuffer[0],FsmPortInitBuffer[1],FsmPortInitBuffer[2],FsmPortInitBuffer[3],FsmPortInitBuffer[4],
			FsmPortInitBuffer[5],FsmPortInitBuffer[6],FsmPortInitBuffer[7],FsmPortInitBuffer[8],FsmPortInitBuffer[9]);
}


static void Init_FlagVariable(void)
{
	GONOGO = false;
	SERVOSOFTON = false;

	INAZMOTORON = false;
	INELMOTORON = false;
	OUTAZMOTORON = false;

	SERVOINIT = false;

	SYSTEMOPERATION = false;

	ModeSystem = SysNull;
	ModeOperation = OpNull;
	ModeCalibration = CalibNull;
	ModeMaintenance = MMNull;

	SYSTEMOFF = false;
	TRACKLOOP = false;

	SAVEOFFSET = false;
	SAVEOFFSETFSM = false;

	CCDFOCUS = false;
	TIFOCUS =false;
	SWUPLOAD = false;
	LOGDATA = false;

	INITIALALIGN = false;
	SURRFORWARDLIMIT =false;
	SURRREVERSELIMIT =false;

	PosModeFlagAz = true;
	MODECHANGEENABLE = true;
	QLOOKOVER = false;
	QLOOKOVERFIRST =false;
	QLOOKSTATUSBIT = true;
	SPSMODEFLAG = false;
	SPSMODEALIGN = false;

	AZRATEZEROFLAG =false;
	STABMODEINIT = false;
	POSMODEINIT = false;
	QLOOKMODEINIT = false;

	SURRAZMOVEMENT = false;
	SURRELMOVEMENT = false;
	SURRFLAGELMOVEMENT = false;
	SURRFLAGELMOVEMENT1 = true;
	ELANGLEUPDATE = true;

	ELSTABDIRUP = false;
	ELSTABDIRDOWN = false;
	AZSTABDIRLEFT = false;
	AZSTABDIRRIGHT = false;
	ADRIFT = false;
	ADRIFTACK = true;
}
#endif
/*
static void Init_FlagVariable(void)
{
		Periscope.Flag.GONOGO = false;
		Periscope.Flag.SERVOSOFTON = false;

		Periscope.Flag.INAZMOTORON = false;
		Periscope.Flag.INELMOTORON = false;
		Periscope.Flag.OUTAZMOTORON = false;

		Periscope.Flag.SERVOINIT = false;

		Periscope.Flag.SYSTEMOPERATION = false;

		Periscope.Mode.System = SysNull;
		Periscope.Mode.Operation = OpNull;
		Periscope.Mode.Calibration = CalibNull;
		Periscope.Mode.Maintenance = MMNull;

		Periscope.Flag.SYSTEMOFF = false;
		Periscope.Flag.TRACKLOOP = false;

		Periscope.Flag.SAVEOFFSET = false;
		Periscope.Flag.SAVEOFFSETFSM = false;

		Periscope.Flag.CCDFOCUS = false;
		Periscope.Flag.TIFOCUS =false;
		Periscope.Flag.SWUPLOAD = false;
		Periscope.Flag.LOGDATA = false;

		Periscope.Flag.INITIALALIGN = false;
		Periscope.Flag.SURRFORWARDLIMIT =false;
		Periscope.Flag.SURRREVERSELIMIT =false;

		Periscope.Flag.PosModeFlagAz = true;
		Periscope.Flag.MODECHANGEENABLE = true;
		Periscope.Flag.QLOOKOVER = false;
		Periscope.Flag.QLOOKSTATUSBIT = true;
		Periscope.Flag.SPSMODEFLAG = false;
		Periscope.Flag.SPSMODEALIGN = false;

		Periscope.Flag.AZRATEZEROFLAG =false;
		Periscope.Flag.STABMODEINIT = false;
		Periscope.Flag.POSMODEINIT = false;
		Periscope.Flag.QLOOKMODEINIT = false;

		Periscope.Flag.SURRAZMOVEMENT = false;
		Periscope.Flag.SURRELMOVEMENT = false;
		Periscope.Flag.SURRFLAGELMOVEMENT = false;
		Periscope.Flag.SURRFLAGELMOVEMENT1 = true;
		Periscope.Flag.ELANGLEUPDATE = true;

		Periscope.Flag.ELSTABDIRUP = false;
		Periscope.Flag.ELSTABDIRDOWN = false;
		Periscope.Flag.AZSTABDIRLEFT = false;
		Periscope.Flag.AZSTABDIRRIGHT = false;

		Periscope.Flag.ADRIFT =false;
}
*/
