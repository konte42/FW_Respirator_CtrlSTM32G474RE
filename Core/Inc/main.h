/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "XCP.h"
#include <string.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void dummy();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWR_SRC_BAT_Pin GPIO_PIN_0
#define PWR_SRC_BAT_GPIO_Port GPIOA
#define OFF_Pin GPIO_PIN_1
#define OFF_GPIO_Port GPIOA
#define SW_IZDIH_Pin GPIO_PIN_0
#define SW_IZDIH_GPIO_Port GPIOB
#define SW_VDIH_Pin GPIO_PIN_1
#define SW_VDIH_GPIO_Port GPIOB
#define ON_OFF_STATE_Pin GPIO_PIN_10
#define ON_OFF_STATE_GPIO_Port GPIOB
#define TIPKA_LED_Pin GPIO_PIN_11
#define TIPKA_LED_GPIO_Port GPIOB
#define FLOW_Pin GPIO_PIN_14
#define FLOW_GPIO_Port GPIOB
#define PRESSURE_Pin GPIO_PIN_15
#define PRESSURE_GPIO_Port GPIOB
#define PWR_SRC_AC_Pin GPIO_PIN_10
#define PWR_SRC_AC_GPIO_Port GPIOA
#define USB_SENS_Pin GPIO_PIN_5
#define USB_SENS_GPIO_Port GPIOB
#define Ajs_KratekStikNaGND_Pin GPIO_PIN_8
#define Ajs_KratekStikNaGND_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define TRAP_LOOP_DELAY_LENGTH 4000000
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
