#define __DISCRETE_IO_C

#include "StdSharc.h"
#include "discreteIO.h"
//#include"debug.h"
//extern Sport debugPort;

volatile unsigned int *inputStatus  = (volatile unsigned int *)0x2000B0;
volatile unsigned int *outputStatus = (volatile unsigned int *)0x200001;

volatile unsigned int *inputStatus1  = (volatile unsigned int *)0x2000C0;
volatile unsigned int *outputStatus1 = (volatile unsigned int *)0x200002;



bool Discrete_GetStateOfInputLine(int identifier)
{
	unsigned long int status = 0;
	if(identifier > MAX_IN_ID)
		return 0;
	//*pAMICTL1 |= 0x04;
	status = *inputStatus;
	//*pAMICTL1 &= ~(0x04);
	return (status & identifier);
}

bool Discrete_GetStateOfOutputLine(int identifier)
{
	int status = 0;
	if(identifier > MAX_OUT_ID)
		return 0;
	//*pAMICTL0 |= 0x04;
	status = *outputStatus;
	//*pAMICTL0 &= ~(0x04);
	return (status & identifier);
}

void Discrete_SetStateOfOutputLine(int identifier, bool state)
{
	static int outStatus = 0;
	//*pAMICTL0 |= 0x04;
	if(identifier >  MAX_OUT_ID)
		return;
	if(identifier == 0x001c)
	{
		if(state)
		{
			identifier=0x000C;
			outStatus =identifier;  //assign |
		}
        else
        {
        	identifier=0x0010;
        	outStatus =identifier;  //assign&
        }
    }
    else
    {
    	if(state)
    	{
    		if(identifier & 0x0010)
    			outStatus &= ~(identifier);
    		else
    			outStatus |= identifier;
    	}
    	else
    	{
    		if(identifier & 0x0010)
    			outStatus |= (identifier);
    		else
    			outStatus &= ~(identifier);
    	}
     }
	 *outputStatus = outStatus;
  //*pAMICTL0 &= ~(0x04);
}


void Discrete_SetStateOfOutputLine1(int identifier, bool state)
{
	static int outStatus1=0;
	if(identifier>0x1000)
		return;
	if(state)
	{
		outStatus1 |= identifier;
	}
	else
	{
		outStatus1 &= ~(identifier);
	}
	*outputStatus1=outStatus1;
}
