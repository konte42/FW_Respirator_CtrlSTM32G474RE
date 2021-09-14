/**
  ******************************************************************************
  * @file    fdcan.c
  * @brief   This file provides code for the configuration
  *          of the FDCAN instances.
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

/* Includes ------------------------------------------------------------------*/
#include "fdcan.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

FDCAN_HandleTypeDef hfdcan2;

/* FDCAN2 init function */
void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = DISABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = DISABLE;
  hfdcan2.Init.NominalPrescaler = 17;
  hfdcan2.Init.NominalSyncJumpWidth = 1;
  hfdcan2.Init.NominalTimeSeg1 = 10;
  hfdcan2.Init.NominalTimeSeg2 = 9;
  hfdcan2.Init.DataPrescaler = 17;
  hfdcan2.Init.DataSyncJumpWidth = 1;
  hfdcan2.Init.DataTimeSeg1 = 10;
  hfdcan2.Init.DataTimeSeg2 = 9;
  hfdcan2.Init.StdFiltersNbr = 1;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */
  //inicializira se filter za CAN povezavo - initialization for filters
  hfdcan2_filter.IdType = FDCAN_STANDARD_ID;
  hfdcan2_filter.FilterIndex = 0;
  hfdcan2_filter.FilterType = FDCAN_FILTER_MASK; //filtrira se z masko in s tem se lahko doloci, kateri ID so se sprejemlnjivi poleg nasega
  hfdcan2_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //sprejeto sporocilo poslje v Rx FIFO 0
  hfdcan2_filter.FilterID1 = 0x63D;//ID nasega procesorja
  hfdcan2_filter.FilterID2 = 0x7FF;//maska
  if (HAL_FDCAN_ConfigFilter(&hfdcan2, &hfdcan2_filter) != HAL_OK)
  {
	Error_Handler();
  }

  //rejects all non-matching identifiers - zavrze vsa sporocila, ki niso namenjena nasemu procesorju in zavrze vsa remote sporocila, moznost FDCAN_ACCEPT_IN_RX_FIFO0
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK)
  {
    Error_Handler();
  }

  //zazene CAN
  if(HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
  {
	Error_Handler();
  }

#ifdef FDCAN_WITH_INTERRUPTS
  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }
#endif


  //konfiguracija registra za oddajanje sporocil preko CAN
  hfdcan2_TxHeader.Identifier = MASTER_ID;
  hfdcan2_TxHeader.IdType = FDCAN_STANDARD_ID;
  hfdcan2_TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  hfdcan2_TxHeader.DataLength = FDCAN_DLC_BYTES_8;
  hfdcan2_TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  hfdcan2_TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  hfdcan2_TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
  hfdcan2_TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  hfdcan2_TxHeader.MessageMarker = 0;

  /* USER CODE END FDCAN2_Init 2 */

}

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(fdcanHandle->Instance==FDCAN2)
  {
  /* USER CODE BEGIN FDCAN2_MspInit 0 */

  /* USER CODE END FDCAN2_MspInit 0 */
    /* FDCAN2 clock enable */
    __HAL_RCC_FDCAN_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN2 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
  /* USER CODE BEGIN FDCAN2_MspInit 1 */

  /* USER CODE END FDCAN2_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN2)
  {
  /* USER CODE BEGIN FDCAN2_MspDeInit 0 */

  /* USER CODE END FDCAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_FDCAN_CLK_DISABLE();

    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* FDCAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
  /* USER CODE BEGIN FDCAN2_MspDeInit 1 */

  /* USER CODE END FDCAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//v ISR za FDCAN program preveri vse FDCAN prekinitve in jih pobrise. Ce bo problem s casom,
//se lahko tu prepise potrebne stvari in tako stkajsa ISR
void HAL_FDCAN_RxFifo0Callback (FDCAN_HandleTypeDef * hfdcan, uint32_t RxFifo0ITs)
{

	if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &hfdcan2_RxHeader, fdcanRxData) != HAL_OK)
	{
		Error_Handler();
	}


    uint16_t SenderID;
    uint16_t dolzinaDLC;
    SenderID = hfdcan2_RxHeader.Identifier;
    dolzinaDLC = hfdcan2_RxHeader.DataLength;
//////naredi prek dolzine prejetega sporocila, vec kot FF je sporocilo?
    if (SenderID == SLAVE_ID)
    {
        if(fdcanRxData[0] == XCP_PID_RES)
        {
            //all good for FF, not for actual values
            if(dolzinaDLC == 1)
            {
                //samo prejme potrditev
            }
            if(fdcan_state == FDCAN_FIRST_MSG_RESPONSE)
            {
                //send trq command
            	int byte_counter = sizeof(trq) - 1;
                unsigned char message2[8] = {0};
                memset(&message2, 0, sizeof(message2));

                message2[0] = XCP_PID_CMD_DOWNLOAD;

                if((byte_counter + 1) > 6)   message2[1] = 6;
                else                         message2[1] = byte_counter + 1;

                for(int i=0; i < message2[1];i++)
                {
                    char *outBuffer = (char * )&trq;
                    message2[i+2] = outBuffer[byte_counter];
                    byte_counter--;
                }


                uint8_t * msg_send2 = message2;
                if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &hfdcan2_TxHeader, msg_send2) != HAL_OK)
                {
                    /* Transmission request Error */
                    Error_Handler();
                }
            }
            fdcan_state = FDCAN_SECOND_MSG_RESPONSE;
        }
        else if(fdcan_state == FDCAN_SECOND_MSG_RESPONSE) {
            //any final reception ok, default state after any send
            fdcan_state = FDCAN_FREE;
        }
        else return;
    }

    // TODO: do something else with these two errors
    else if(fdcanRxData[0] == XCP_PID_ERR)
    {
        //error
        trq = 0;
        fdcan_state = FDCAN_ERROR_WHEN_TRANSMITTING;
        CAN_XCP_write(RequestedTorque, inExtension0, sizeof(trq), (char *)&trq); //request without interrupts safety I guess??
    }
    else
    {
    	//not good, error?
    	trq = 0;
    	fdcan_state = FDCAN_UNKNOWN_ERROR;
        CAN_XCP_write(RequestedTorque, inExtension0, sizeof(trq), (char *)&trq); //request without interrupts safety I guess??

    }
}

//namenjen je ugotavljanju problemov s prozenjem prekinitev za CAN takoj po aktivaciji
//int prmejdun = 0;

void write_trq(void)
{
    //trq request, first command
    unsigned char message[8] = {0};
    unsigned inAddress = RequestedTorque;

    memset(&message, 0, sizeof(message));
    message[0] = XCP_PID_CMD_SET_MTA;
    message[3] = inExtension0; //inExtension not
    message[4] = (unsigned char)(inAddress >> 24);
    message[5] = (unsigned char)(inAddress >> 16);
    message[6] = (unsigned char)(inAddress >> 8);
    message[7] = (unsigned char)(inAddress >> 0);

    uint8_t * msg_send = message;     //to reši vse nadaljne zadeve

    //hfdcan2_TxHeader.DataLength = sizeof(message);
    //static can_fesp_t TxData;         //8 byte data in union to be send

    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &hfdcan2_TxHeader, msg_send) != HAL_OK) {
        /* Transmission request Error */
        Error_Handler();
    }

    //tu aktivirane prekinitve, ko se je ugotavljalo njihovo prozenje, uporabljeno skupaj s prmejdun
    /*if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
      Error_Handler();
    }*/

    fdcan_state = FDCAN_FIRST_MSG_RESPONSE;
    //prmejdun = 1;
}


char CAN_XCP_INIT(unsigned inAddress, unsigned char inExtension, unsigned inLength, char *outBuffer)
{
	fdcan_state = FDCAN_MOTOR_INIT;

    CAN_XCP_connect();
    //init
    unsigned char SetUp = 1;
    uint32_t SetUp2 = 1;
    CAN_XCP_write(Motorcontrol_Mode, 0, 4, (char *)&SetUp2);
    CAN_XCP_write(Motorcontrol_Mode_S, 1, 4, (char *)&SetUp2);
    CAN_XCP_write(MTC_Switch, 0, 1, (char *)&SetUp);
    CAN_XCP_write(MTC_S_Switch, 1, 1, (char *)&SetUp);

    HAL_Delay(500);

    unsigned char message[8] = {0};
	signed long byte_counter = inLength - 1;
	//unsigned long byte_counter_old = 0;

	CAN_XCP_CLEAR();

	memset(&message, 0, sizeof(message));
	message[0] = XCP_PID_CMD_SET_MTA;		//F6
	message[3] = inExtension;
	message[4] = (unsigned char)(inAddress >> 24);
	message[5] = (unsigned char)(inAddress >> 16);
	message[6] = (unsigned char)(inAddress >> 8);
	message[7] = (unsigned char)(inAddress >> 0);

	uint8_t *msg_send = message;     //to reši vse nadaljne zadeve

	//hfdcan2_TxHeader.DataLength = sizeof(message);
	//static can_fesp_t TxData;         //8 byte data in union to be send

	if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &hfdcan2_TxHeader, msg_send) != HAL_OK)
	{
	  /* Transmission request Error */
	  Error_Handler();
	}

	HAL_Delay(3);
	CAN_XCP_CLEAR();

	if (CAN_XCP_response() != XCP_PID_RES) {
		//Error_Handler();
		return;
	}

    fdcan_state = FDCAN_FREE;
    return HAL_OK; //all good
}


void CAN_XCP_connect()
{        //XCP connect message, probably 0xFF00 is enough

	unsigned char message[8] = {0};

    memset(&message, 0, sizeof(message));
    message[0] = XCP_PID_CMD_CONNECT;     //0xFF
    message[1] = 0x00;                    //XCP normal mode

    uint8_t *msg_send = message;

    //hfdcan2_TxHeader.DataLength = sizeof(message);


    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &hfdcan2_TxHeader, msg_send) != HAL_OK)
    {
        /* Transmission request Error */

        Error_Handler();
    }

    /*while (motor_init_msg_recieved_flag == 0) {}
    if(motor_init_msg_recieved_flag) {motor_init_msg_recieved_flag=0;}*/
    //HAL_Delay(1);

    while (CAN_XCP_response() != XCP_PID_RES) {
        //HAL_Delay(1);
    }
}


void CAN_XCP_write(unsigned inAddress, unsigned char inExtension, unsigned inLength, char *outBuffer)
{        //XCP write msg, address, extension 0 or 1, length? buffer?

	unsigned char message[8] = {0};
    signed long byte_counter = inLength - 1;
    //unsigned long byte_counter_old = 0;

    CAN_XCP_CLEAR();

    memset(&message, 0, sizeof(message));
    message[0] = XCP_PID_CMD_SET_MTA;
    message[3] = inExtension;
    message[4] = (unsigned char)(inAddress >> 24);
    message[5] = (unsigned char)(inAddress >> 16);
    message[6] = (unsigned char)(inAddress >> 8);
    message[7] = (unsigned char)(inAddress >> 0);

    uint8_t *msg_send = message;     //to reši vse nadaljne zadeve

    //hfdcan2_TxHeader.DataLength = sizeof(message);
    //static can_fesp_t TxData;         //8 byte data in union to be send

    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &hfdcan2_TxHeader, msg_send) != HAL_OK)
    {
      /* Transmission request Error */
      Error_Handler();
    }

    /*while (motor_init_msg_recieved_flag == 0) {}
    if(motor_init_msg_recieved_flag) {motor_init_msg_recieved_flag=0;}*/
    HAL_Delay(1);
    while (CAN_XCP_response() != XCP_PID_RES) {

    }

      //now the response was positive and we can send actual data we want

    unsigned char message2[8] = {0};
    memset(&message2, 0, sizeof(message2));

    message2[0] = XCP_PID_CMD_DOWNLOAD;

    if((byte_counter + 1) > 6)   message2[1] = 6;
    else                         message2[1] = byte_counter + 1;

    for(int i=0; i < message2[1];i++)
    {
    	message2[i+2] = outBuffer[byte_counter];
        byte_counter--;
    }

    uint8_t * msg_send2 = message2;
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &hfdcan2_TxHeader, msg_send2) != HAL_OK)
    {
    	/* Transmission request Error */
        Error_Handler();
    }


    /*while (motor_init_msg_recieved_flag == 0) {}
    if(motor_init_msg_recieved_flag) {motor_init_msg_recieved_flag=0;}
    *///while (CAN_XCP_response() != XCP_PID_RES) {
    //    HAL_Delay(1);
    //}
}

void CAN_XCP_write_trq(unsigned inAddress, unsigned char inExtension, unsigned inLength, char *outBuffer)
{        //XCP write msg, address, extension 0 or 1, length? buffer?

	unsigned char message[8] = {0};
    signed long byte_counter = inLength - 1;
    //unsigned long byte_counter_old = 0;

    CAN_XCP_CLEAR();

    memset(&message, 0, sizeof(message));
    message[0] = XCP_PID_CMD_SET_MTA;		//F6
    message[3] = inExtension;
    message[4] = (unsigned char)(inAddress >> 24);
    message[5] = (unsigned char)(inAddress >> 16);
    message[6] = (unsigned char)(inAddress >> 8);
    message[7] = (unsigned char)(inAddress >> 0);

    uint8_t *msg_send = message;     //to reši vse nadaljne zadeve

    //hfdcan2_TxHeader.DataLength = sizeof(message);
    //static can_fesp_t TxData;         //8 byte data in union to be send

    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &hfdcan2_TxHeader, msg_send) != HAL_OK)
    {
      /* Transmission request Error */
      Error_Handler();
    }

    HAL_Delay(3);
    CAN_XCP_CLEAR();

    if (CAN_XCP_response() != XCP_PID_RES) {
        //Error_Handler();
        return;
    }

      //now the response was positive and we can send actual data we want

    unsigned char message2[8] = {0};
    memset(&message2, 0, sizeof(message2));

    message2[0] = XCP_PID_CMD_DOWNLOAD;	//F0

    if((byte_counter + 1) > 6)   message2[1] = 6;
    else                         message2[1] = byte_counter + 1;

    for(int i=0; i < message2[1];i++)
    {
    	message2[i+2] = outBuffer[byte_counter];
        byte_counter--;
    }

    uint8_t * msg_send2 = message2;
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &hfdcan2_TxHeader, msg_send2) != HAL_OK)
    {
    	/* Transmission request Error */
        Error_Handler();
    }

    HAL_Delay(1);

    //while (CAN_XCP_response() != XCP_PID_RES) {
    //    HAL_Delay(1);
    //}
}



//response for commands
int CAN_XCP_response()
{    //done by interrupt, from now on used for init only
    //get response
    //rework this return thingie
	int i=0;

	while(HAL_FDCAN_GetRxFifoFillLevel(&hfdcan2, FDCAN_RX_FIFO0) == 0)
    {
    	i++;
    	HAL_Delay(1);
    	if (i>15) return XCP_PID_RES;
    }

    /*uint8_t recieved[8] = {0};
    uint32_t naslovi[10] = {0};
    int tabela_i = 0;
    int rakec = 0;
    uint32_t zafilanost = HAL_FDCAN_GetRxFifoFillLevel(&hfdcan2, FDCAN_RX_FIFO0);
    while(rakec=0 || tabela_i<10){
    	HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, &hfdcan2_RxHeader, recieved);
    	naslovi[tabela_i] = hfdcan2_RxHeader.Identifier;
    	tabela_i++;
    }*/


    uint8_t * message = fdcanRxData;
    if(HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, &hfdcan2_RxHeader, message) != HAL_OK)
    {
        /* Reception Error */
        Error_Handler();
    }

    uint16_t SenderID = hfdcan2_RxHeader.Identifier;

    if (SenderID == SLAVE_ID) {
    	if(fdcanRxData[0] == XCP_PID_RES){
           //all good for FF, not for actual values
           return fdcanRxData[0];
       }
       else if(fdcanRxData[0] == XCP_PID_ERR)
       {
           //error
           return fdcanRxData[0];
       }
       else

           //not good
           return fdcanRxData[0];
     }
     return 0;
}

void CAN_XCP_CLEAR(void)
{
    //this clears the possible remaining messages in fifo which can cause trouble
    if(HAL_FDCAN_GetRxFifoFillLevel(&hfdcan2, FDCAN_RX_FIFO0) > 0) {
        uint8_t * message = fdcanRxData;
        if(HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, &hfdcan2_RxHeader, message) != HAL_OK) {
            /* Reception Error */
            Error_Handler();
        }
    }
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
