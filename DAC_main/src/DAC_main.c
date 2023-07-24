/*****************************************************************************
 * DAC_main.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "DAC_main.h"
#include "INIT_api.h"
#include "DAC_api.h"
#include "discreteIO_api.h"
#include <stdint.h>
/** 
 * If you want to use command program arguments, then place them in the following string. 
 */

int main(void)
{
	/**
	 * Initialize managed drivers and/or services that have been added to 
	 * the project.
	 * @return zero on success 
	 */
	adi_initComponents();
	
	/* Begin adding your custom code here */
	Init_HW();
	int upCnt = 0;
	int downCnt = 16383;
	Discrete_SetStateOfOutputLine(1<<0,1);
	Discrete_SetStateOfOutputLine(1<<1,1);
	Discrete_SetStateOfOutputLine1(1<<0,1);//Pin54(J5)-13//working
	Discrete_SetStateOfOutputLine1(1<<1,1);//Pin37(J5)-15//working
	while(1){

		if(upCnt<=16383){
			Write_DAC(0,upCnt);
			Write_DAC(1,upCnt);
			upCnt++;
			delay(1);
		}
		if(upCnt>=16383 && downCnt>(-16383)){
			Write_DAC(0,downCnt);
			Write_DAC(1,downCnt);
			downCnt--;
			delay(1);
		}

		if(downCnt<=(-16383)){
			upCnt = 0;
			downCnt = 16383;
			Write_DAC(0,0);
			Write_DAC(1,0);
			delay(1000);

		}
		//
	}
	return 0;
}
static void delay(uint32_t delayCount)
{
	delayCount = 26315.789 * delayCount;

  /* delayCount = 1 => 38ns delay */
  while(delayCount--);
}
