#ifndef __DISCRETE_IO
#define __DISCRETE_IO

#define DISCRETE_NUM_INPUTS  13
#define DISCRETE_NUM_OUTPUTS 11

/**
 * Masks
 */

/**
 * Input masks (input identifier)
 */

#define HEALTH_100V 	0x00000001
#define HEALTH_Az28V    0x00000002
#define HEALTH_El28V    0x00000004
#define HEALTH_N15V     0x00000008
#define HEALTH_P15V     0x00000010
#define HEALTH_5V       0x00000020
#define MAX_IN_ID       HEALTH_5V

/*
#define SC_AZ_BUF         0x000001
#define SC_EL_BUF         0x000002
#define GYRO_BIT_G_BUF    0x000004
#define GYRO_BIT_X_BUF    0x000008
#define GYRO_BIT_Y_BUF    0x000010
#define HEALTH_P52VDC_BUF 0x000020
#define HEALTH_ELRF       0x000040
#define HEALTH_P15V       0x000080
#define HEALTH_CCD        0x000100
#define HEALTH_P5V        0x000200
#define HEALTH_TI         0x000400
#define HEALTH_N15V       0x000800
#define OL_AZ_BUF         0x001000
#define OL_EL_BUF         0x002000
#define MAX_IN_ID         OL_EL_BUF
*/

/**
 * Output masks (output identifier)
 */

#define EN_AZ             0x000001
#define EN_EL             0x000002
#define EN_RLY_CCD        0x000004
#define EL_RLY_ELRF       0x000008
#define EL_RLY_TI         0x000010
#define RST_AZ            0x000020
#define RST_EL            0x000040
#define P52_RST           0x000080
#define P52_ONOFF         0x000100
#define GYRO_ONOFF        0x000200
#define AZ_GYRO_MODE      0x000400
#define EL_GYRO_MODE      0x000800
#define SWITCH_VIDEO      0x001000
#define MAX_OUT_ID        SWITCH_VIDEO
/**
 * Provided externs
 */

extern volatile unsigned int *inputStatus;
extern volatile unsigned int *outputStatus;

/**
 * Provided APIs
 */

bool Discrete_GetStateOfInputLine(int identifier);
bool Discrete_GetStateOfOutputLine(int identifier);
void Discrete_SetStateOfOutputLine(int identifier, bool state);
void Discrete_SetStateOfOutputLine1(int identifier, bool state);

#endif
