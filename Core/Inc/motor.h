/*
 * motor.h
 *
 * Created: 2.4.2020 10:59:59
 *  Author: Kristjan
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "main.h"
// ******************************
// Uporabniske nastavitve
// ******************************

// meritev pozicije motorja - kanal ADC
#define MOTOR_POS_CH	ADC_CH_POSITION
//#define MOTOR_POS_RAW_OPEN	820	// --> 0
//#define MOTOR_POS_RAW_CLOSED 740	// -->?
//#define MOTOR_POS_BREAKINGPOINT_OPEN 885
//#define MOTOR_POS_BREAKINGPOINT_CLOSED 726
#define MOTOR_POS_RAW_OPEN	(820*64)	// --> 0
#define MOTOR_POS_RAW_CLOSED (740*64)	// -->?
#define MOTOR_POS_BREAKINGPOINT_OPEN (885*64)
#define MOTOR_POS_BREAKINGPOINT_CLOSED (726*64)

#define MOTOR_POS_OPEN	0		// pos: \ /
#define MOTOR_POS_CLOSED	(MOTOR_POS_RAW_OPEN - MOTOR_POS_RAW_CLOSED) // pos: ||

// maksimalna vredfnost registra za dutycylce
#define MOTOR_MAX_DC	1023
#define MOTOR_MIN_DC	150
// ******************************
// Konec Uporabniskih nastavitev
// ******************************

#define MOTOR_OK 1
#define MOTOR_ERR 0

typedef enum
{
	MOTOR_DIR_VDIH,
	MOTOR_DIR_IZDIH,
	MOTOR_DIR_UNDEFINED
}MotorDir_t;

void motor_Init();
void motor_SetSpeed(float speed);	//-100 - 100
void motor_SetDutyCycle(uint16_t dutyCycle);
void motor_SetRawDutyCycle(uint16_t dutyCycle);
void motor_SetDir(MotorDir_t direction);
float motor_GetPosition();	// %
float motor_GetCurrent();	// mA
int16_t motor_GetPWM();
float motor_GetSpeedSetting(); //0 - 100
#endif /* MOTOR_H_ */
