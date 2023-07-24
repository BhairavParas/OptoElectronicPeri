#define _SPORT_C

#include "StdSharc.h"
#include "sync_sport.h"

#define WORD_SIZE 1
#define BUF_SIZE 1024
#define TCB_SIZE 4
#define PCI_MASK 0x7FFFF
#define LAST_ELMNT 3


static unsigned char m_aucDmasport_buf[BUF_SIZE]={0};
static unsigned int m_auisport5_tcb[TCB_SIZE] = {0, BUF_SIZE, WORD_SIZE, (unsigned int)m_aucDmasport_buf};

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

