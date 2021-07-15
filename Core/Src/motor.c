/*
 * motor.c
 *
 * Created: 2.4.2020 10:59:50
 *  Author: Kristjan
 */ 
#include "motor.h"
#include "adc.h"
#include "gpio.h"
#include "fdcan.h"

extern TIM_HandleTypeDef htim3;
MotorDir_t MotorDir;
uint8_t state;

void motor_Init()
{
#if 0
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MOTOR_SEL0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, MOTOR_INA_Pin|MOTOR_INB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PtPin */
//  GPIO_InitStruct.Pin = SWB_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(SWB_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = MOTOR_SEL0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
//  GPIO_InitStruct.Pin = SWA_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(SWA_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
  GPIO_InitStruct.Pin = MOTOR_INA_Pin|MOTOR_INB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
//  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

//  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  MotorDir = MOTOR_DIR_UNDEFINED;
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
 #endif
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
#warning "manjka implementacija IRQ za detekcijo koncnih stikal motorja. S tem motorjem ne moremo doseci zelenega v tej funkciji, ker potrebujemo cca 10 ms za prenos ukaza motorju"
	/*
	switch(GPIO_Pin)
	{
	case SWA_Pin:						//SWA - jaws locked
		TIM3->CCR2 = MOTOR_MIN_DC;
		break;
	case SWB_Pin:						//SWB - jaws opened
		TIM3->CCR2 = 0;
		break;
	default:
		break;
	}*/
}

void motor_SetPower(float power)	// -100 - 100
{
	/*
	uint16_t dc;

	if (power > 0)
	{
		if (power > 100) power = 100;
		if (MotorDir!=MOTOR_DIR_VDIH) motor_SetDir(MOTOR_DIR_VDIH);
		dc = (uint16_t)(power / 100.0 * (MOTOR_MAX_DC-MOTOR_MIN_DC)) + MOTOR_MIN_DC;
		motor_SetPWM(dc);
	}
	else if (power < 0)
	{
		power = -power;
		if (power > 100) power = 100;
		if (MotorDir!=MOTOR_DIR_IZDIH) motor_SetDir(MOTOR_DIR_IZDIH);
		dc = (uint16_t)(power / 100.0 * MOTOR_MAX_DC);
		motor_SetPWM(dc);
	}
	else  //if (speed = 0) stop, maintain direction and use appropriate MIN duty cycle
	{
      if (MotorDir==MOTOR_DIR_VDIH) motor_SetPWM(MOTOR_MIN_DC);
      else motor_SetPWM(0);
	}
	*/
	float trq;
	trq = MOTOR_MAX_TORQUE * (power/100);

	if(state == 0 )
	{

	}
}

void motor_SetPWM(uint16_t dutyCycle)
{
	if (dutyCycle>MOTOR_MAX_DC) dutyCycle = MOTOR_MAX_DC;
	if ( MotorDir==MOTOR_DIR_VDIH) // ko stiskamo, ne sme DC nikoli pasti na nic, tudi ce pride do konca
	{
		//if (!HAL_GPIO_ReadPin(SWA_GPIO_Port, SWA_Pin)) { TIM3->CCR2 = dutyCycle; }
	  if (motor_GetPosition() < 100) { TIM3->CCR2 = dutyCycle; }
	  else TIM3->CCR2 = MOTOR_MIN_DC;
	}
	else	//izdih
	{
		//if (!HAL_GPIO_ReadPin(SWB_GPIO_Port, SWB_Pin)) { TIM3->CCR2 = dutyCycle; }
		if (motor_GetPosition() > 0) { TIM3->CCR2 = dutyCycle; }
		else TIM3->CCR2=0;
	}
}

float motor_GetPosition()	//0 - 100 = normal open - normal closed
{
#ifdef PROTOTYPE_V1
#elif defined(PROTOTYPE_V2)
  if (is_SWA()) return -50;
#else
#error Prototype version not defined.
#endif
	int32_t raw=*(ADC_results_p()+ADC_CH_POSITION);
	return (float)(MOTOR_POS_RAW_OPEN-raw)/(float)(MOTOR_POS_RAW_OPEN - MOTOR_POS_RAW_CLOSED)*100.0 ;
}

float motor_GetCurrent()
{
	#warning "Toka ne moremo brati v verziji 3"
	float raw=*(ADC_results_p()+ADC_CH_MOTOR_CURRENT);
	return raw*0.624179;	// mA
}

float motor_GetPower()	// 0 - 100
{
	float power=0;

	int16_t pwm;
	pwm = motor_GetPWM();

	if (pwm>=MOTOR_MIN_DC)
	{
		power = (float) (pwm-MOTOR_MIN_DC)* 100.0 / (MOTOR_MAX_DC-MOTOR_MIN_DC);
	}

	return power;
}

int16_t motor_GetPWM()
{
	/*
	if (TIM3->CCR2==0) return 0;
	if (MotorDir == MOTOR_DIR_VDIH) return TIM3->CCR2;
	else return -TIM3->CCR2;
	*/
}

void motor_SetDir(MotorDir_t direction)
{
	/*if (MotorDir!=direction)
	{
		switch (direction)
		{
			case MOTOR_DIR_VDIH:
				HAL_GPIO_WritePin(MOTOR_INA_GPIO_Port, MOTOR_INA_Pin, 0);
				HAL_Delay(1);	//TODO: replace 1ms delay with 10us delay
				//_delay_us(10);	//dead time
				HAL_GPIO_WritePin(MOTOR_INB_GPIO_Port, MOTOR_INB_Pin, 1);
				HAL_GPIO_WritePin(MOTOR_SEL0_GPIO_Port, MOTOR_SEL0_Pin, 0);
				MotorDir=MOTOR_DIR_VDIH;
			break;

			case MOTOR_DIR_IZDIH:
				HAL_GPIO_WritePin(MOTOR_INB_GPIO_Port, MOTOR_INB_Pin, 0);
				HAL_Delay(1);	//TODO: replace 1ms delay with 10us delay
				//_delay_us(10);	//dead time
				HAL_GPIO_WritePin(MOTOR_INA_GPIO_Port, MOTOR_INA_Pin, 1);
				HAL_GPIO_WritePin(MOTOR_SEL0_GPIO_Port, MOTOR_SEL0_Pin, 1);
				MotorDir=MOTOR_DIR_IZDIH;
			break;

			default: //do nothing
			break;
		}
	}*/
}

