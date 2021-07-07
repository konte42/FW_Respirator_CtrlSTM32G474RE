/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */


//#define LED1_On()	HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)
//#define LED1_Off()	HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET)
//#define LED1_Tgl()	HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin)

#define LED1_On()	{}
#define LED1_Off()	{}
#define LED1_Tgl()	{}

#define LED2_On()	{}
#define LED2_Off()	{}
#define LED2_Tgl()	{}

#define LED3_On()	{}
#define LED3_Off()	{}
#define LED3_Tgl()	{}

#define LED4_On()	{}
#define LED4_Off()	{}
#define LED4_Tgl()	{}

#define LED5_On()	{}
#define LED5_Off()	{}
#define LED5_Tgl()	{}

#define LED6_On()	{}
#define LED6_Off()	{}
#define LED6_Tgl()	{}

#define LED7_On()	{}
#define LED7_Off()	{}
#define LED7_Tgl()	{}

#define BUZZ_On()  dummy()
#define BUZZ_Off()  dummy()
#define BUZZ_Tgl()  dummy()

#warning "Koncna stikala se ne delujejo!!!"
#define is_SWA() (1)
#define is_SWB() (1)

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
