/*****************************************************************************
 * ENCODER_trial.h
 *****************************************************************************/

#ifndef __ENCODER_TRIAL_H__
#define __ENCODER_TRIAL_H__

/* Add your custom header content here */

#endif /* __ENCODER_TRIAL_H__ */


float InAzPosition;
float InAzOffsetI;
float InAzOffsetM;
float InAzOffsetF;
float InAzOffset;

float InElPosition;
float InElOffsetI;
float InElOffsetM;
float InElOffsetF;
float InElOffset;

float OutAzPosition;
float OutAzOffsetI;
float OutAzOffsetM;
float OutAzOffsetF;
float OutAzOffset;

int TrackerAzError;
int TrackerElError;

static void Init_PLL(void);
static void Init_SRU(void);
static void Init_ParallelPort(void);
static void Init_Timer(void);
static void Init_Interrupts(void);
static void Init_Sport(void);
static void Timer_Start(void);
void delay(unsigned int);
