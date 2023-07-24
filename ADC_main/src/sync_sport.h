#ifndef __SPORT_H
#define __SPORT_H

#include "discreteIO.h"
/*
 * sport.h
 *
 *  Created on: 12-May-2016
 *      Author: t.deepak
 */

#ifndef SPORT_H_
#define SPORT_H_


//#include "../src/eotsf.h"
//#include "header.h"


typedef enum
{
	SPORT0 = 0,
	SPORT1,
	SPORT2,
	SPORT3,
	SPORT4,
	SPORT5,
	SPORT6,
	SPORT7
} E_sportnumber;

typedef enum
{
	SPORT_success=0,
	SPORT_numfailure,
	SPORT_channelfailure,
	SPORT_numberchannelfailure,
	SPORT_wordlfailure,
	SPORT_clockfailure,
	SPORT_writesuccess,
	SPORT_writefailure,
	SPORT_readsuccess,
	SPORT_readfailure,
	SPORT_initfailure
}E_sportresult;

typedef enum
{
	CHANNEL_A = 0,
	CHANNEL_B,
	CHANNEL_A_B
}E_sportchannel;

typedef enum
{
//	SPORT0_A=0,
//	SPORT0_B=1,
	SPORT1_A=0,
	SPORT1_B=1,
	//SPORT2_A=0,
	//SPORT2_B=1,
	SPORT3_A=0,
	SPORT3_B=1,
	//SPORT4_A=0,
	//SPORT4_B=1,
	SPORT5_A=0,
	SPORT5_B
}E_sportnum_channel;

#endif

E_sportresult Sportinit(E_sportnumber sportnumber, E_sportchannel sportchannel, unsigned int clkinmhz);
//unsigned short SPORTRXDATA(E_sportnumber sportnumber, E_sportnum_channel sportchannel);
//int8u SPORTRXDATA(E_sportnumber sportnumber, E_sportnum_channel sportchannel);
//int8u * SPORTRXDATA(E_sportnumber sportnumber, E_sportnum_channel sportchannel,unsigned char lengthbytes);
//E_sportresult SPORTTXDATA(unsigned char sportnumber,unsigned char sportchannel, unsigned char data,unsigned char lengthbytes);


E_sportresult SPORTTXDATA(unsigned char sportnumber,unsigned char sportchannel, unsigned char data[],unsigned char lengthbytes);
//E_sportresult SPORTRXDATA(unsigned char sportnumber,unsigned char sportchannel, unsigned char *data);
E_sportresult SPORTRXDATA(unsigned char sportnumber,unsigned char sportchannel,unsigned char lengthbytes, unsigned char *array);
unsigned char SPORTRXDATAbyte(E_sportnumber sportnumber, E_sportnum_channel sportchannel);


//#endif /* SPORT_H_ */

//void Sport3_Init(void);
void InitSport3(void);
//void Sport5_Init(void);
void InitSport5(void);

void Sport_35_LoopBack(void);

void Sport3_TxByte(unsigned char data);

void Sport5_TxByte(unsigned char data);

void Sport3_RxByte(unsigned char *data);

void Sport5_RxByte(unsigned char *data);

void Sport1_RxByte_cha(signed short *data, unsigned char no_of_Chan);
//void Sport1_RxByte_cha(signed short *data, unsigned char no_of_Chan,signed short *data1, unsigned char no_of_Chan1);

//void Sport1_Init(void);
void InitSport1(void);

void Sport1_RxByte1(int *data1);
int GetCommnData(unsigned char *ucBuf, unsigned int count);
int SportDmaStat(void);


#endif
