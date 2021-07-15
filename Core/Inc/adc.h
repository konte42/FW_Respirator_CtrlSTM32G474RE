/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "CommonDefinitions.h"
/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc4;

/* USER CODE BEGIN Private defines */

#define ADC_STARTCHAN	0
#define ADC_STOPCHAN	4
#define ADC_FILTER_N	16
//#define ADC_DOUBLE_BUFFERING

#ifdef AVR
#define ADC_CH_FLOW			ADC_STARTCHAN
#define ADC_CH_PRESSURE		(ADC_STARTCHAN+1)
#define ADC_CH_MOTOR_CURRENT	(ADC_STARTCHAN+2)
#define ADC_CH_POSITION		(ADC_STARTCHAN+3)
#define ADC_CH_FLOW_N		(ADC_STARTCHAN+4)
#else
#define ADC_CH_FLOW				    0	//Channel numbers are critical for setting up DMA
#define ADC_CH_PRESSURE			  	2
#define ADC_CH_MOTOR_CURRENT		3
#define ADC_CH_POSITION				4
#define ADC_CH_FLOW_N		    	1
#endif

/* USER CODE END Private defines */

void MX_ADC2_Init(void);
void MX_ADC4_Init(void);

/* USER CODE BEGIN Prototypes */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

void ADC_Init();
char ADC_scan_complete();
uint16_t *ADC_results_p(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
