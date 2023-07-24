/*****************************************************************************
 * ADC_main.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "ADC_main.h"
#include "ADC_api.h"
#include "INIT_api.h"
#include "UART_api.h"
/** 
 * If you want to use command program arguments, then place them in the following string. 
 */
extern int AdcData[8];

int main(void)
{
	/**
	 * Initialize managed drivers and/or services that have been added to 
	 * the project.
	 * @return zero on success 
	 */
	adi_initComponents();
	int InnerAzCurrentSense = 0 , InnerElCurrentSense = 0 , OuterAzCurrentSense = 0 ;
	float GyroXX = 0.0 , GyroBaseYY = 0.0 , GyroYY = 0.0 ;
	int ch6 = 0 , ch7 = 0 ;
	Init_HW();
	/* Begin adding your custom code here */
	while(1){
		ReadAdcdata();
		InnerAzCurrentSense= AdcData[0];//0.1 times load current
		InnerElCurrentSense= AdcData[1];
		OuterAzCurrentSense= AdcData[2];
		GyroXX = (float)(AdcData[3]);	/****Base Azimuth with gain****/
		//GyroBaseYY = (float)AdcData[1];	/****Base elevation with gain****/
		GyroBaseYY = (float)AdcData[4];	/****Base elevation with gain****/
		GyroYY = (float)(AdcData[5]); /****Mirror elevation with gain****/
		ch6 = AdcData[6];
		ch7 = AdcData[7];
		delay(1);
	}

	return 0;
}
static void delay(uint32_t delayCount)
{
	delayCount = 26315.789 * delayCount;

  /* delayCount = 1 => 38ns delay */
  while(delayCount--);
}
