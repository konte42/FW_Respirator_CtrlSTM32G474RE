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
#define MOTOR_POS_RAW_OPEN	820	// --> 0
#define MOTOR_POS_RAW_CLOSED 740	// -->?
#define MOTOR_POS_BREAKINGPOINT_OPEN 885
#define MOTOR_POS_BREAKINGPOINT_CLOSED 726
#define MOTOR_POS_OPEN	0		// pos: \ /
#define MOTOR_POS_CLOSED	(MOTOR_POS_RAW_OPEN - MOTOR_POS_RAW_CLOSED) // pos: ||

// maksimalna vredfnost registra za dutycylce
#define MOTOR_MAX_DC	1023	//before it was 1023
#define MOTOR_MIN_DC	150		//
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
void motor_SetDutyCycle(uint16_t dutyCycle);
void motor_SetDir(MotorDir_t direction);
int16_t motor_GetPosition();
int16_t motor_GetCurrent();
int16_t motor_GetPWM();
#endif /* MOTOR_H_ */
