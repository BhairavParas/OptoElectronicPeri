/*
 * uart_api.h
 *
 *  Created on: 14-Jul-2023
 *      Author: Admin
 */

#ifndef UART_API_H_
#define UART_API_H_



#endif /* UART_API_H_ */

#ifndef _SERIALPORT_H
#define _SERIALPORT_H

#define UART_FIFO_DEPTH 256

#define UART_NO_PARITY    0x00
#define UART_EVEN_PARITY  0x01
#define UART_ODD_PARITY   0x02

		/* Control registers */
#define 		UART_LCRREG_OFFSET  0x003
#define 		UART_FCRREG_OFFSET  0x002
#define 		UART_DLLREG_OFFSET  0x000
#define 		UART_DLMREG_OFFSET  0x001
#define 		UART_MCRREG_OFFSET  0x004

		/* Status register */
#define 		UART_LSRREG_OFFSET  0x005
#define 		UART_MSRREG_OFFSET  0x006

		/* Transmit and receive registers */
#define 		UART_THRREG_OFFSET  0x000
#define 		UART_RBRREG_OFFSET  0x000

		/* Interrupt Registers */
#define 		UART_IERREG_OFFSET  0x001
#define 		UART_IIRREG_OFFSET  0x002

		/* set FIFO trigger level */
#define 		UART_01BYTE_FIFO    0x01
#define 		UART_04BYTE_FIFO    0x41
#define 		UART_08BYTE_FIFO    0x81
#define 		UART_14BYTE_FIFO    0xC1

#define 		TX_EMPTY            0x20
#define 		RX_DATA             0x01

#define     QUART_LOOPBACK_ENABLE   0x10

struct SerialPort
{
	volatile unsigned char *m_linecontrol_Register;
	volatile unsigned char *m_fifocontrol_Register;
	volatile unsigned char *m_devicelatchL_Register;
	volatile unsigned char *m_devicelatchM_Register;
	volatile unsigned char *m_modemcontrol_Register;

	volatile unsigned char *m_linestatus_Register;
	volatile unsigned char *m_modemstatus_Register;

	volatile unsigned char *m_transmit_Register;
	volatile unsigned char *m_receive_Register;
	volatile unsigned char *m_interrupt_Register;
	volatile unsigned char *m_interruptIdent_Register;

	volatile int m_iFifoTriggerLevel;
	volatile bool rxOverflow;
	volatile bool rxOverflowWarning;
	volatile int m_iBaudRate;
	unsigned char fifo[UART_FIFO_DEPTH];
	volatile int head;
	volatile int tail;
	volatile int total_bytes_in_fifo;
	volatile bool triggered;
	volatile bool rxFramingError;
	volatile bool rxBreakOccured;
	volatile bool rxParityError;
	volatile bool rxOverrunError;
};

typedef struct SerialPort Sport;


Sport *SportArray[];
#define PCLK        132000000
#define QUARTCLK    14745600

Sport DeblurPort;   // DEBLUR CARD  port:  On-chip UART0
Sport DebugPort; 	// DEBUG port		:  On-chip UART1
Sport RL_STB;    	// ROLL port		:  QUart1 - A
Sport SPARE1;   	// SPARE1 port		:  QUart1 - B
Sport INS_SEN;   	// INS SENSOR port	:  QUart1 - C
Sport OPT_MMI;   	// OPTRONICS_MMI port: QUart1 - D



void SerialPort_Configure(Sport *sp,
                          long baseAddress,
                          unsigned char wordLength, // See NOTE1
                          bool stopBits,            // See NOTE2
                          unsigned char parity,     // See NOTE3
                          int iBaudRate);

/**
 * NOTE1: wordLength = UARTWLS5 or UARTWLS6 or UARTWLS7 or UARTWLS8
 */

/**
 * NOTE2: stopBits =
 * 0 => 1 stop bit
 * 1 => 2 stop bits
 */

/**
 * NOTE3: parity = UART_NO_PARITY or UART_EVEN_PARITY or UART_ODD_PARITY
 */
void Serial_GetData(Sport *sp, unsigned char *buffer, int count);
int WriteData_SPort(Sport sp,
                    unsigned char *pData,
                    int byteCount);

int ReadData_SPort(Sport *sp,
                   unsigned char *pData,
                   int byteLimit);
void Init_Sport(void);

bool Receiver_Has_Data(Sport sp);

int ReadLineStatusRegister(Sport sp);

void SetBaudRate(Sport * sp, int iBaudRate);

void EnableInternalLoopBack(Sport sp);

void DisableInternalLoopBack(Sport sp);

int SetSerialFIFOTrigger(Sport* sp,int iTrigLen);

bool IsSportErroneous(Sport *sp);

void ResetFifo(Sport *sp);

void UART0_RxIsr(int sig);
void QUART_RxIsr(int sig);
void Serial_RxIsr_Insert(Sport *sp);
void QUART_LwRxIsr(int sig);
void UART1_RxIsr(int sig);
//void DebugPortIsr(int sig);

void motor_cntr(unsigned ucdata);
void  OnBoardSupply_Deblr(void );

//void     DebugPrint(Sport sp, char *msgBuffer, ...);
//void DebugPrint(char *msgBuffer, ... );
void putch1(Sport sp, unsigned char ch);

#endif
