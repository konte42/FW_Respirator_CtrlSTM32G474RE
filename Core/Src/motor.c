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


void motor_SetPower(float power)	// -100 - 100, +navor je vdih, -navor je izdih
{
#ifdef FDCAN_WITH_INTERRUPTS
	if(fdcan_state == FDCAN_FREE)
	{
		if(HAL_GPIO_ReadPin(SW_VDIH_GPIO_Port, SW_VDIH_Pin)==0&&power>0) {trq = MOTOR_MIN_TORQUE;}
		else if(HAL_GPIO_ReadPin(SW_IZDIH_GPIO_Port, SW_IZDIH_Pin)==0&&power<0) {trq = 0;}
		else trq = MOTOR_MAX_TORQUE * (power/100);

		if (trq>MOTOR_MAX_TORQUE){
			Error_Handler();
		}

		write_trq();
	}
	else Error_Handler();
#else
	if(fdcan_state == FDCAN_FREE)
	{
		float trq_no_it=0;

		if(power>0)
		{
			if (motor_GetPosition()>=100) trq_no_it = MOTOR_MIN_TORQUE;
			else if (power<100) trq_no_it = MOTOR_MAX_TORQUE * (power/100);
			else trq_no_it = MOTOR_MAX_TORQUE;
		}
		else if(power<0)
		{
			if (motor_GetPosition()<=0) trq_no_it = 0;
			else if (power>-100) trq_no_it = MOTOR_MAX_TORQUE * (power/100);
			else trq_no_it = -MOTOR_MAX_TORQUE;
		}

		trq = trq_no_it;

//		if (trq>MOTOR_MAX_TORQUE || trg<-MOTOR_MAX_TORQUE)
//		{
//			Error_Handler();
//		}

		CAN_XCP_write(RequestedTorque, 0 , 4 , (char *)&trq_no_it);
	}
	else Error_Handler();

#endif
}


float motor_GetPosition()	//0 - 100 = normal open - normal closed
{
	//static float pos_prev;

	if (is_SWA()) return 0;
	else if (is_SWB()) return 100;
	int32_t raw=*(ADC_results_p()+ADC_CH_POSITION);

	// to je odvisno od tega, kakšna je orientacija senzorja pozicije
	if (raw < MOTOR_POS_RAW_CLOSED ) raw = MOTOR_POS_RAW_CLOSED;
	if (raw > MOTOR_POS_RAW_OPEN ) raw = MOTOR_POS_RAW_OPEN;

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



