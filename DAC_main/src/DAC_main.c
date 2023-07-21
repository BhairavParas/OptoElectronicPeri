/*****************************************************************************
 * DAC_main.c
 *****************************************************************************/

#include <sys/platform.h>
#include "adi_initialize.h"
#include "DAC_main.h"
#include "INIT_api.h"
#include "DAC_api.h"
#include "discreteIO_api.h"
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

	Discrete_SetStateOfOutputLine(1<<0,1);
	Discrete_SetStateOfOutputLine(1<<1,1);
	Discrete_SetStateOfOutputLine1(1<<0,1);//Pin54(J5)-13//working
	Discrete_SetStateOfOutputLine1(1<<1,1);//Pin37(J5)-15//working
	while(1){
		Write_DAC(0,2);
		Write_DAC(1,2);
	}
	return 0;
}

