/*****************************************************************************
 * UART_FOV_main.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "UART_FOV_main.h"
#include "UART_FOV_api.h"
#include "INIT_FOV_api.h"
#include <services/int/adi_int.h>

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
	
	char debugBuffer[20] ="Debug print\r\n";
	/* Begin adding your custom code here */
	/*initialize the setup*/
	InitApp();

	while(1){
		WriteData_SPort(DebugPort, (unsigned char *)debugBuffer, 14);
		delay(1000);
	}
	return 0;
}
static void delay(uint32_t delayCount)
{
	delayCount = 26315.789 * delayCount;

  /* delayCount = 1 => 38ns delay */
  while(delayCount--);
}
