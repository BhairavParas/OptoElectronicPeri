#include "UART_FOV_api.h"
#include "StdSharc.h"

 Sport *SportArray[];
#define PCLK        132000000
#define QUARTCLK    14745600

char DebugPortTansmiterBuffer[24]={0};
int DebugPortTransmitDataLength=24;
char DebugPortReceiveBuffer[24]={0};

char GciPortTransmiterBuffer[16]={0};
int GciPortTransmitDataLength=16;
char GciPortReceiveBuffer[16]={0};

bool RECEIVEDATA=false;
bool GCIRECEIVEDATA=false;

static void getch(Sport sp, unsigned char *ch);
static void putch(Sport sp, unsigned char ch);

void QUART_RxIsr(int sig);
void Serial_RxIsr_Insert(Sport *sp);

void SerialPort_Configure(Sport *sp,
                          long baseAddress,
                          unsigned char wordLength,
                          bool stopBits,
                          unsigned char parity,
                          int iBaudRate)
{
  /**
   * Validate input parameters
   */
  if(wordLength > UARTWLS8)
    return;
  if(parity > UART_ODD_PARITY)
    return;

  if(parity == UART_EVEN_PARITY)
    parity = 0x0C;
  else if(parity == UART_ODD_PARITY)
    parity = 0x04;
    
  /**
   * Set the register addresses
   */ 
  sp->m_linecontrol_Register =
    (volatile unsigned char *)baseAddress + UART_LCRREG_OFFSET;
  sp->m_fifocontrol_Register =
    (volatile unsigned char *)baseAddress + UART_FCRREG_OFFSET;
  sp->m_devicelatchL_Register =
    (volatile unsigned char *)baseAddress + UART_DLLREG_OFFSET;
  sp->m_devicelatchM_Register =
    (volatile unsigned char *)baseAddress + UART_DLMREG_OFFSET;
  sp->m_modemcontrol_Register =
    (volatile unsigned char *)baseAddress + UART_MCRREG_OFFSET;
  sp->m_linestatus_Register =
    (volatile unsigned char *)baseAddress + UART_LSRREG_OFFSET;
  sp->m_modemstatus_Register =
    (volatile unsigned char *)baseAddress + UART_MSRREG_OFFSET;
  sp->m_transmit_Register =
    (volatile unsigned char *)baseAddress + UART_THRREG_OFFSET;
  sp->m_receive_Register =
    (volatile unsigned char *)baseAddress + UART_RBRREG_OFFSET;
  sp->m_interrupt_Register =
    (volatile unsigned char *)baseAddress + UART_IERREG_OFFSET;
  sp->m_interruptIdent_Register =
    (volatile unsigned char *)baseAddress + UART_IIRREG_OFFSET;
  
  sp->m_iFifoTriggerLevel = UART_FIFO_DEPTH;
  ResetFifo(sp);

  /**
   * 1. Clear LCR
   * 2. Set Baud as required
   * 3. Set LCR as per input parameters
   * 4. Enable receive interrupt
   */
  
  *(sp->m_linecontrol_Register) = 0;
  sp->m_iBaudRate = iBaudRate;
  SetBaudRate(sp, iBaudRate);
  *(sp->m_linecontrol_Register) = wordLength | parity | (stopBits << 2);
  *(sp->m_interrupt_Register) = UARTRBFIE | UARTTBEIE;

    if(baseAddress <= 0x4000)
    {
      *(volatile unsigned char *)(baseAddress + 0x204) = UARTEN;       //enables UART in receive mode
      *(volatile unsigned char *)(baseAddress + 0x304) = UARTEN;       //enables UART in core driven mode
    }
    else
    {
      *(sp->m_modemcontrol_Register) |= 0x08;
    }
    parity = *(sp->m_receive_Register);   // This is a dummy read to clear any data in RX reg
    DisableInternalLoopBack(*sp);
    parity=*(sp->m_interruptIdent_Register);
}

int ReadData_SPort(Sport *sp,
                   unsigned char *pData,
                   int byteLimit)
{
  int numBytes = 0;
  int retval = 0;
  int head = 0;

  if(!pData)
    return -1;
  
  if(sp->total_bytes_in_fifo == 0)
    return 0;

  retval = numBytes = (byteLimit > sp->total_bytes_in_fifo) ? 
                      sp->total_bytes_in_fifo : byteLimit;

  while(numBytes)
  {
    head = (sp->head + 1) % UART_FIFO_DEPTH;
    *pData = sp->fifo[head];

    // Disable interrupt here
    *(sp->m_interrupt_Register) &= ~(UARTRBFIE);

    sp->head = head;
    --sp->total_bytes_in_fifo;
    if(sp->total_bytes_in_fifo < sp->m_iFifoTriggerLevel)
      sp->triggered = false;
    
    // Enable interrupts here
    *(sp->m_interrupt_Register) |= UARTRBFIE;

    ++pData;
    --numBytes;
  }
  return retval;
}

int WriteData_SPort(Sport sp,
                    unsigned char *pData,
                    int byteCount)
{
  int retVal = byteCount;

  if(!pData)
    return -1;

  while(byteCount)
  {
    putch(sp, *pData);
    ++pData;
    --byteCount;
  }
  return retVal;
}

bool Receiver_Has_Data(Sport sp)
{
  return (sp.total_bytes_in_fifo > 0);
}

int ReadLineStatusRegister(Sport sp)
{
  return *(sp.m_linestatus_Register);
}

void SetBaudRate(Sport * sp, int iBaudRate)
{
  unsigned short divisor = 0;
  
  /**
   * Get Divisor
   */
  if((int)sp->m_transmit_Register <= 0x4000)
  {
    divisor = PCLK / (16 * iBaudRate);
  }
  else
  {
    divisor = QUARTCLK / (16 * iBaudRate);
  }

  /**
   * 1. Set divisor enable latch
   * 2. Set DLL
   * 3. Set DLH
   * 4. Clear divisor enable latch
   */
  *(sp->m_linecontrol_Register) |= UARTDLAB;
  *(sp->m_devicelatchL_Register) = divisor & 0xFF;
  *(sp->m_devicelatchM_Register) = (divisor & 0xFF00) >> 8;
  *(sp->m_linecontrol_Register) &= ~(UARTDLAB);
}

int SetSerialFIFOTrigger(Sport* sp,int iTrigLen)
{
  if(iTrigLen <= 0)
    return 0;
  sp->m_iFifoTriggerLevel = iTrigLen;
  return 1;
}

void EnableInternalLoopBack(Sport sp)
{
  if((int)sp.m_transmit_Register <= 0x4000)
    return;
  *(sp.m_modemcontrol_Register) |= QUART_LOOPBACK_ENABLE; // Enable internal loop back
}

void DisableInternalLoopBack(Sport sp)
{
  if((int)sp.m_transmit_Register <= 0x4000)
    return;
  *(sp.m_modemcontrol_Register) &= ~(QUART_LOOPBACK_ENABLE); // Disable internal loop back
}

static void putch(Sport sp, unsigned char ch)
{
	while ((*(sp.m_linestatus_Register) & UARTTHRE) == 0);
	*(sp.m_transmit_Register) = ch;
	while ((*(sp.m_linestatus_Register) & UARTTHRE) == 0);
}

static void getch(Sport sp, unsigned char *ch)
{
    *ch = *(sp.m_receive_Register);
}

void QUART_LwRxIsr(int sig)
{
  int interruptSource = 4;
 // volatile unsigned char *interruptSourceAddr = (volatile unsigned char *)0x200030;
  Sport *sp;
  int interruptMask = 1;
  int interruptID = 2;

    
  // Acknowledge PF interrupt
  interruptSource = *pDAI_IRPTL_L;
   
  // Check CPLD memory to identify interrupt channel
  //interruptSource = interruptSource >> 24;
  //interruptSource = *interruptSourceAddr;

  //M
  /*
 	 if(interruptSource & SRU_EXTMISCA2_INT)
   	 {
	      sp = SportArray[interruptID];
  		    Serial_RxIsr_Insert(sp);
   	 }
 	 if(interruptSource & SRU_EXTMISCA3_INT)
   	 {
	      sp = SportArray[interruptID+1];
  		    Serial_RxIsr_Insert(sp);
   	 }  	*/

  if(interruptSource & SRU_EXTMISCA0_INT)
     	 {
  	      sp = SportArray[interruptID];
    		    Serial_RxIsr_Insert(sp);
     	 }



}


void QUART_RxIsr(int sig)
{
  int interruptSource = 4;
  volatile unsigned char *interruptSourceAddr = (volatile unsigned char *)0x200030;
  Sport *sp;
  int interruptMask = 1;
  int interruptID = 0;
  /**
   * Acknowledge PF interrupt
   * Check CPLD memory to identify interrupt channel
   * Insert at tail
   */
   
  // Acknowledge PF interrupt
  interruptSource = *pDAI_IRPTL_H;
   
  // Check CPLD memory to identify interrupt channel
  //interruptSource = 0; dd
  //interruptSource = *interruptSourceAddr;  dd
  //while(interruptSource != 0)  dd
  //{  dd
   /* if(interruptSource & SRU_EXTMISCA2_INT)
    {
      sp = SportArray[interruptID];
      Serial_RxIsr_Insert(sp);
      interruptSource &= ~(interruptMask);
    }*/
    if(interruptSource & SRU_EXTMISCA3_INT)
    {
    	sp = SportArray[interruptID + 1];
        Serial_RxIsr_Insert(sp);
        interruptSource &= ~(interruptMask);
    	
    }
    if(interruptSource & SRU_EXTMISCA2_INT)
    {
    	sp = SportArray[interruptID];
        Serial_RxIsr_Insert(sp);
        interruptSource &= ~(interruptMask);
    	
    }
    if(interruptSource & SRU_EXTMISCA1_INT)  //B4
    {
    	sp = SportArray[interruptID + 3];
        Serial_RxIsr_Insert(sp);
        interruptSource &= ~(interruptMask);
    	
    }
       
    //interruptMask <<= 1;  DDD
   // interruptID += 1;
  //}  dd
} 

/*void UART0_RxIsr(int sig)
{
  Serial_RxIsr_Insert(sp);
}*/

/*void UART1_RxIsr(int sig)
{
 Sport *sp = SportArray[5];
 Serial_RxIsr_Insert(sp);
}*/

void Serial_RxIsr_Insert(Sport *sp)
{
  /*
   * Increment tail
   * Read a byte of data from the channel and put at tail
   * 1. Check for trigger level and set trigger flag if required
   * 2. Check for overflow warning level and set overflow warning flag if requried
   * 3. Check for overflow and set the overflow flag if required
   * 4. If overflow flag is set, set head = tail + 1
   */

  unsigned char lsr = *(sp->m_linestatus_Register);
  unsigned char ch = 0;

  if(lsr & UARTOE)
  {
    sp->rxOverrunError = true;
    ch = *(sp->m_receive_Register);
    return;
  }
  if(lsr & UARTPE)
  {
    sp->rxParityError = true;
    ch = *(sp->m_receive_Register);
    return;
  }
  if(lsr & UARTFE)
  {
    sp->rxFramingError = true;
    ch = *(sp->m_receive_Register);
    return;
  }
  if(lsr & UARTBI)
  {
    sp->rxBreakOccured = true;
    ch = *(sp->m_receive_Register);
    return;
  }

  sp->tail = (sp->tail + 1) % UART_FIFO_DEPTH;
  getch(*sp, &(sp->fifo[sp->tail]));
  ++sp->total_bytes_in_fifo;
  
  /**
   * Up on overflow, overwrite previous data. And keep head just above tail.
   */

  if(sp->total_bytes_in_fifo >= UART_FIFO_DEPTH)
  {
    sp->total_bytes_in_fifo = UART_FIFO_DEPTH;
    sp->rxOverflow = true;
    sp->head = (sp->tail + 1) % UART_FIFO_DEPTH;
  }
  if(sp->total_bytes_in_fifo >= UART_FIFO_DEPTH - 10)
  {
    sp->rxOverflowWarning = true;
  }
  else
  {
    sp->rxOverflowWarning = false;
  }
  if(sp->total_bytes_in_fifo >= sp->m_iFifoTriggerLevel)
  {
    sp->triggered = true;
  }
}

bool IsSportErroneous(Sport *sp)
{
	return (sp->rxFramingError || sp->rxBreakOccured || sp->rxParityError || sp->rxOverrunError);
}

void ResetFifo(Sport *sp)
{
	sp->rxOverflow = false;
	sp->rxOverflowWarning = false;
	sp->triggered = false;
  sp->rxFramingError = false;
	sp->rxBreakOccured = false;
	sp->rxParityError = false;
	sp->rxOverrunError = false;
  sp->head = -1;
  sp->tail = -1;
  sp->total_bytes_in_fifo = 0;
}

void UART1RxIsr(int sig)
{
	static int ReceiveDataCount=0;
	static int TransmitDataCount=1;
	static bool sync=false;
	char ch;
	static unsigned char ch1;
	static unsigned char ch2;
	ch=*pUART1LSR;
	if(ch & UARTOE)
	{
		ch=*pUART1RBR;
	    return;
	}
	if(ch & UARTPE)
	{
		ch=*pUART1RBR;
		return;
	}
	if(ch & UARTFE)
	{
		ch=*pUART1RBR;
		return;
	}
	if(ch & UARTBI)
	{
		ch=*pUART1RBR;
		return;
	}
	if((ch & 0x01)==0x01)
	{
		if(sync)
		{
			DebugPortReceiveBuffer[ReceiveDataCount]=*pUART1RBR;
			ReceiveDataCount++;
		}
		else
		{
			if(ReceiveDataCount==0x00)
			{
				ch1=*pUART1RBR;
				DebugPortReceiveBuffer[ReceiveDataCount]=ch1;
				ReceiveDataCount++;
			}
			else if(ReceiveDataCount==0x01 && ch1==0xCC)
			{
				ch2=*pUART1RBR;
				DebugPortReceiveBuffer[ReceiveDataCount]=ch2;
				if(ch2==0xCC)
				{
					sync=true;
					ReceiveDataCount++;
				}
				else
				{
					ReceiveDataCount=0;
				}
			}
			else
				ReceiveDataCount=0;
		}
		ch=*pUART1LSR;
		if(ReceiveDataCount==24)
		{
			ReceiveDataCount=0;
			RECEIVEDATA=true;
			sync=false;
		}
	}
	if((ch & 0x20)==0x20)
	{
		if(--DebugPortTransmitDataLength>0)
		{
			*pUART1THR=DebugPortTansmiterBuffer[TransmitDataCount];
			TransmitDataCount++;
		}
		else
		{
			TransmitDataCount=1;
			ch=*pUART1IIR;
		}
	}
}


void UART0RxIsr(int sig)
{
	char ch0;
	static int ReceiveDataCount0=0;
	static int TransmitDataCount0=1;
	static bool Sync0=false;
	static unsigned char ch1;
	static unsigned char ch2;

	ch0=*pUART0LSR;
	if(ch0 & UARTOE)
	{
		ch0=*pUART0RBR;
	    return;
	}
	if(ch0 & UARTPE)
	{
		ch0=*pUART0RBR;
		return;
	}
	if(ch0 & UARTFE)
	{
		ch0=*pUART0RBR;
		return;
	}
	if(ch0 & UARTBI)
	{
		ch0=*pUART0RBR;
		return;
	}
	if((ch0 & 0x01)==0x01)
	{
		if(Sync0)
			GciPortReceiveBuffer[ReceiveDataCount0]=*pUART0RBR;
		else
		{
			if(ReceiveDataCount0==0x00)
			{
				ch1=*pUART0RBR;
				GciPortReceiveBuffer[ReceiveDataCount0]=ch1;
				ReceiveDataCount0++;
			}
			else if(ReceiveDataCount0==0x01 && ch1==0xCC)
			{
				ch2=*pUART0RBR;
				GciPortReceiveBuffer[ReceiveDataCount0]=ch2;
				Sync0=true;
			}
			else
				ReceiveDataCount0=0;
		}
		if(ReceiveDataCount0==15)
		{
			ReceiveDataCount0=0;
			GCIRECEIVEDATA=true;
			Sync0=false;
		}
		else
		{
			if (GciPortReceiveBuffer[0]==0xCC && GciPortReceiveBuffer[1]==0xCC)
				ReceiveDataCount0++;
		}
	}
	if((ch0 & 0x20)==0x20)
	{
		if(--GciPortTransmitDataLength>0)
		{
			*pUART0THR=GciPortTransmiterBuffer[TransmitDataCount0];
			TransmitDataCount0++;
		}
		else
		{
			TransmitDataCount0=1;
			ch0=*pUART0IIR;
		}
	}
}
