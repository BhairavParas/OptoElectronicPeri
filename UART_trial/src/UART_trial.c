/*****************************************************************************
 * UART_trial.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "UART_trial.h"
#include "uart_api.h"
#include "StdSharc.h"
#include "INIT_api.h"
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

	Init_HW();
	/*****Configure clock for ADSP21369*****/
	//Init_PLL();
	/***************************************/


	/*****Maps peripherals to external pins*****/
	//Init_SRU();
	/*******************************************/

	/* Begin adding your custom code here */

	/* Initiate the Serial communication */
	//Init_Serial();



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



static void delay(uint32_t delayCount)
{
	delayCount = 26315.789 * delayCount;

  /* delayCount = 1 => 38ns delay */
  while(delayCount--);
}

