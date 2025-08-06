#ifndef HEAD
#define HEAD
//#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "User_SMS_STS.h"
#include "SCServo.h"
#include "io_retargetToUart.h"
#include "math.h"
#include "task.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "stdlib.h"

#define GREEN  //红光模式还是绿光模式，主要是串口接受的不一样

extern uint8_t Q_NO;
extern uint8_t mode1;
extern uint8_t mode2;
extern int16_t spe1;
extern int16_t spe2;
extern uint8_t ERROR_FLAG;

#ifdef RED
extern uint8_t dot1x,dot1y,dot2x,dot2y,dot3x,dot3y,dot4x,dot4y;
extern int16_t tar_pos1,tar_pos2;

#endif // RED
extern uint8_t red_x,red_y;

void Servo_Start(void);
void Statemachine_Start(void);

#endif // !HEAD