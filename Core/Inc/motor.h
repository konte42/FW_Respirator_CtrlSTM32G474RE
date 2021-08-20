/*
 * motor.h
 *
 * Created: 2.4.2020 10:59:59
 *  Author: Kristjan
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "main.h"

#include "CommonDefinitions.h"
// ******************************
// Uporabniske nastavitve
// ******************************

// meritev pozicije motorja - kanal ADC
#define MOTOR_POS_CH	ADC_CH_POSITION
//#define MOTOR_POS_RAW_OPEN	820	// --> 0
//#define MOTOR_POS_RAW_CLOSED 740	// -->?
//#define MOTOR_POS_BREAKINGPOINT_OPEN 885
//#define MOTOR_POS_BREAKINGPOINT_CLOSED 726


#define MOTOR_POS_RAW_OPEN  43800 // --> 0
#define MOTOR_POS_RAW_CLOSED  6500 // -->100
#define MOTOR_POS_BREAKINGPOINT_OPEN  0
#define MOTOR_POS_BREAKINGPOINT_CLOSED 65535


#define MOTOR_POS_OPEN	0		// pos: \ /
#define MOTOR_POS_CLOSED	(MOTOR_POS_RAW_OPEN - MOTOR_POS_RAW_CLOSED) // pos: ||

// TODO: "definiraj maksimalni in minimalni navor, 0.7 in 0.01 so simbolicne"
#warning "definiraj maksimalni in minimalni navor, 0.7 in 0.01 so simbolicne"
#define MOTOR_MAX_TORQUE	0.4
#define MOTOR_MIN_TORQUE	0.08	//minimalni potrebni navor, da bo držal balon stisnjen

// ******************************
// Konec Uporabniskih nastavitev
// ******************************

#define MOTOR_OK 1
#define MOTOR_ERR 0

void motor_Init();
void motor_SetPower(float power);	//-100 - 100
float motor_GetPosition();	// %
float motor_GetCurrent();	// mA
float motor_GetPower(); //0 - 100
#endif /* MOTOR_H_ */
