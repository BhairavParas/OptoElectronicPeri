/*
 * INIT_FOV_api.h
 *
 *  Created on: 24-Jul-2023
 *      Author: Lenovo
 */

#ifndef INIT_FOV_API_H_
#define INIT_FOV_API_H_

Sport GciPort;
Sport DebugPort;

void InitApp(void);
static void Init_PLL(void);
static void Init_ParallelPort(void);
static void Init_SRU(void);
static void Init_Serial(void);
static void Init_Interrupts(void);
static void Init_Timer(void);
static void Start_Timer(void);
static void ServoSensorInitialization(void);

#endif /* INIT_FOV_API_H_ */
