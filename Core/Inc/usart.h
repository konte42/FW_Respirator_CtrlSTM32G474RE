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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* USER CODE END Includes */

extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
/*
FTDI231x Baud rate limitations:
Baud Rate = 3000000 / (n + x)
where �?n’ can be any integer between 2 and 16,384 ( = 214 ) and �?x’ can be a sub-integer of the value 0,
0.125, 0.25, 0.375, 0.5, 0.625, 0.75, or 0.875. When n = 1, x = 0, i.e. baud rate divisors with values
between 1 and 2 are not possible.

STM32G4 baud rate limitations: BRR=SysClk/Baudrate
*/

/*Spremeni glede na velikost podatkov*/
#define UART_BUFFER_SIZE 128

typedef struct
{
  unsigned char buffer[UART_BUFFER_SIZE];
  unsigned int head;
  unsigned int tail;
  volatile int count;
} ring_buffer;

typedef enum {
	UART_EOF	= -1,
	UART_OK		= 0
} UART_Status_t;
/* USER CODE END Private defines */

void MX_LPUART1_UART_Init(void);
void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
/* Incilizacija UARTA */
void UART0_Init();

/* Incilizacija krožnega medpomnilnika */
void Ringbuf_Init (void);

/*Shrani v krožni medpomnilnik*/
void store_char(unsigned char c, ring_buffer *buffer);

/*V oddajni medpomnilnik zapiše tekst str*/
int UART0_SendStr(const char *s);

/*V oddajni medpomnilnik zapiše num byte-ov podatkovnega niza data*/
int UART0_SendBytes(const char *s, int num);

/*Vrne število znakov v oddajnem medpomnilniku*/
int UART0_numTxBytes();

/*Vrne število znakov v sprejemnem medpomnilniku*/
int UART0_numRxBytes();

/*Vrne 0, če v medpomnilniku še ni novega podatka, karkoli drugega, če v sprejemnem medpomnilniku že čaka nov podatek*/
int UART0_DataReady(void);

/*Odpošlje en znak preko serijskega vmesnika*/
UART_Status_t UART0_put(const char c);

/*Prebere podatek iz sprejemnega medpomnilnika in ga zapiše na naslov »data«*/
UART_Status_t UART0_GetByte(char *data);

/*ISR funkcija. POMEMBNO!!! Potrebno jo je vključiti v glavni program*/
void UART_ISR (UART_HandleTypeDef *huart);
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
