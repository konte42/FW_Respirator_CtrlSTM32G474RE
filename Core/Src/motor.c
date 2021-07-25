/*
 * motor.c
 *
 * Created: 2.4.2020 10:59:50
 * updated for FEspirator_V3 by Kristan Pivk
 *  Author: Kristjan
 */ 
#include "motor.h"
#include "adc.h"
#include "gpio.h"
#include "fdcan.h"


void motor_Init()
{
  if(CAN_XCP_INIT() != HAL_OK)
  {
	  Error_Handler();
  }
}


void motor_SetPower(float power)	// -100 - 100
{
	if(fdcan_state == FDCAN_FREE)
	{
		if(HAL_GPIO_ReadPin(SW_VDIH_GPIO_Port, SW_VDIH_Pin)) {trq = MOTOR_MIN_TORQUE;}
		else if(HAL_GPIO_ReadPin(SW_IZDIH_GPIO_Port, SW_IZDIH_Pin)) {trq = 0;}
		else trq = MOTOR_MAX_TORQUE * (power/100);

		write_trq();
	}
	else return;
}


float motor_GetPosition()	//0 - 100 = normal open - normal closed
{
	if (is_SWA()) return -50;
	int32_t raw=*(ADC_results_p()+ADC_CH_POSITION);
	return (float)(MOTOR_POS_RAW_OPEN-raw)/(float)(MOTOR_POS_RAW_OPEN - MOTOR_POS_RAW_CLOSED)*100.0 ;
}


float motor_GetCurrent()
{
	#warning "Toka ne moremo brati v verziji 3"
	return 0;	// mA
}


float motor_GetPower()	// 0 - 100
{
	float power=0;
	float trq_temp;

	if(trq < 0)
	{
		trq_temp = - trq;
	}
	else trq_temp = trq;

	if (trq_temp>=MOTOR_MIN_TORQUE)
	{
		power = (trq_temp/MOTOR_MAX_TORQUE) * 100;
	}

	return power;
}



