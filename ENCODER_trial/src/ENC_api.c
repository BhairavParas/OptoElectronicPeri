/*
 * ENC_api.c
 *
 *  Created on: 14-Jul-2023
 *      Author: Admin
 */


#include "ENC_api.h"
#include "ENCODER_trial.h"
#include "StdSharc.h"

#if 1
volatile unsigned int *enc_azData = (volatile unsigned int *)0x200070;
volatile unsigned int *enc_azData1 = (volatile unsigned int *)0x200073;
volatile unsigned int *enc_elData = (volatile unsigned int *)0x200071;
volatile unsigned int *enc_elData1 = (volatile unsigned int *)0x200074;
volatile unsigned int *enc_out_azData = (volatile unsigned int *)0x200072;
volatile unsigned int *enc_out_azData1 = (volatile unsigned int *)0x200075;

#endif

#if 0
unsigned int encAZL = 0;
unsigned int encAZH = 0;
unsigned int encELL = 0;
unsigned int encELH = 0;
unsigned int encAZOutL = 0;
unsigned int encAZOutH = 0;

volatile unsigned int *enc_azData = &encAZL;
volatile unsigned int *enc_azData1 = &encAZH;
volatile unsigned int *enc_elData = &encELL;
volatile unsigned int *enc_elData1 = &encELH;
volatile unsigned int *enc_out_azData = &encAZOutL;
volatile unsigned int *enc_out_azData1 = &encAZOutH;
#endif

void ReadEncoderData(void)
{

	char ch;
	unsigned long int data = 0x00000000;
	float EncoderOffset;

	data = (*enc_azData)&0x00FFFFFF;
	data = (data |((*enc_azData1&0xFF)<<24));
	//InAzPosition = data;
	InAzPosition = data*0.00000008382;
	InAzPosition += InAzOffset;
	if(InAzPosition > 180)
		InAzPosition -= 360;
	else if(InAzPosition < -180)
		InAzPosition += 360;

	data = (*enc_elData)&0x00FFFFFF;
	data = (data |((*enc_elData1&0xFF)<<24));
	InElPosition = data*0.00000008382;
	InElPosition += InElOffset;
	if(InElPosition > 180)
		InElPosition -= 360;
	else if(InElPosition < -180)
		InElPosition += 360;

	data = (*enc_out_azData)&0x00FFFFFF;
	data = (data |((*enc_out_azData1&0xFF)<<24));
	OutAzPosition = data*0.00000008382;
	OutAzPosition += OutAzOffset;
	if(OutAzPosition > 180)
		OutAzPosition -= 360;
	else if(OutAzPosition < -180)
		OutAzPosition += 360;
}

/*
float OutAzEncoderData_HMI;
OutAzEncoderData_HMI=OutAzEncoderData;
*/
