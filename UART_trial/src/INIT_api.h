#ifndef INIT_API_H_
#define INIT_API_H_


void Init_HW(void);
static void Init_PLL(void);
static void Init_ParallelPort(void);
static void Init_SRU(void);
static void Init_Serial(void);
static void Init_Interrupts(void);

static void Init_Sport(void);

static void Init_Timer(void);

static void Init_FlagVariable(void);

bool POST(void);

//static void ServoSensorInitialization(void);
static void Init_Servo_Sensors(void);
static void Init_Fsm(void);


static void Timer_Start(void);
void IsrTimer0(int sig);

#endif /* INIT_API_H_ */
