/**
  ******************************************************************************
  * @file    fdcan.h
  * @brief   This file contains all the function prototypes for
  *          the fdcan.c file
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
#ifndef __FDCAN_H__
#define __FDCAN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern FDCAN_HandleTypeDef hfdcan2;

/* USER CODE BEGIN Private defines */
#warning "definiraj maksimalni in minimalni navor"
#define MOTOR_MAX_TORQUE
#define MOTOR_MIN_TORQUE

FDCAN_FilterTypeDef hfdcan2_filter;
FDCAN_TxHeaderTypeDef hfdcan2_TxHeader;
FDCAN_RxHeaderTypeDef hfdcan2_RxHeader;
uint8_t fdcanTxData[8];
uint8_t fdcanRxData[8];
fdcan_State_t fdcan_state;


/* USER CODE END Private defines */

void MX_FDCAN2_Init(void);

/* USER CODE BEGIN Prototypes */

void CAN_Send();
char CAN_XCP_INIT();
void CAN_XCP_connect();
void CAN_XCP_write(unsigned inAddress, unsigned char inExtension, unsigned inLength, char *outBuffer);
void CAN_XCP_read(unsigned inAddress, unsigned char inExtension, unsigned inLength, char *inBuffer);
void write_trq(void);
void read_angle(void);

int CAN_XCP_response();

typedef enum
{
	FDCAN_FREE,
	FDCAN_FIRST_MSG_RESPONSE,
	FDCAN_SECOND_MSG_RESPONSE,
	FDCAN_TRANSMIT_ERROR,
	MOTOR_INIT
} fdcan_State_t;


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __FDCAN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
