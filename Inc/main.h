/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32g4xx_ll_pwr.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define FLOW_N_Pin GPIO_PIN_1
#define FLOW_N_GPIO_Port GPIOC
#define FLOW_P_Pin GPIO_PIN_0
#define FLOW_P_GPIO_Port GPIOA
#define PRESSURE_Pin GPIO_PIN_1
#define PRESSURE_GPIO_Port GPIOA
#define LPUART1_TX_Pin GPIO_PIN_2
#define LPUART1_TX_GPIO_Port GPIOA
#define LPUART1_RX_Pin GPIO_PIN_3
#define LPUART1_RX_GPIO_Port GPIOA
#define MOTOR_CURRENT_Pin GPIO_PIN_4
#define MOTOR_CURRENT_GPIO_Port GPIOA
#define LED8_Pin GPIO_PIN_4
#define LED8_GPIO_Port GPIOC
#define MOTOR_POSITION_Pin GPIO_PIN_0
#define MOTOR_POSITION_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_1
#define LED4_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOB
#define LED7_Pin GPIO_PIN_13
#define LED7_GPIO_Port GPIOB
#define LED6_Pin GPIO_PIN_14
#define LED6_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_15
#define LED5_GPIO_Port GPIOB
#define MOTOR_CS_Pin GPIO_PIN_8
#define MOTOR_CS_GPIO_Port GPIOA
#define MOTOR_SEL0_Pin GPIO_PIN_9
#define MOTOR_SEL0_GPIO_Port GPIOA
#define SWA_Pin GPIO_PIN_10
#define SWA_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB
#define MOTOR_INA_Pin GPIO_PIN_4
#define MOTOR_INA_GPIO_Port GPIOB
#define MOTOR_INB_Pin GPIO_PIN_5
#define MOTOR_INB_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
