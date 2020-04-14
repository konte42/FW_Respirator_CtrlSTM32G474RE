/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
   typedef enum {
   	UART_EOF	= -1,
   	UART_OK		= 0
   } UART_Status_t;

   //   int UART0_SendStr(char str[]);
   int UART0_SendBytes(char data[], int num);
   int UART0_DataReady();
   UART_Status_t UART0_GetByte(char* data);
   UART_Status_t UART0_put(char data);
   int UART0_numTxBytes();
   int UART0_numRxBytes();
   char UART0_PreviewRxByte(int index);

   #define TB_SIZE0	100
   extern volatile char TxBuf0[TB_SIZE0];
   extern int TBin0;
   extern volatile int TBout0;
   extern volatile int TBnum0;

   #define RB_SIZE0	100
   extern volatile char RxBuf0[RB_SIZE0];
   extern int RBin0;
   extern volatile int RBout0;
   extern volatile int RBnum0;
/* USER CODE END Includes */

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_LPUART1_UART_Init(void);
void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
