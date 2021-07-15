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
#include "XCP.h"


/* USER CODE BEGIN 0 */

/* USER CODE END 0 */



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
  hfdcan2.Init.StdFiltersNbr = 0;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */
  //inicializira se filter za CAN povezavo
  hfdcan2_filter.IdType = FDCAN_STANDARD_ID;
  hfdcan2_filter.FilterIndex = 0;
  hfdcan2_filter.FilterType = FDCAN_FILTER_MASK; //filtrira se z masko in s tem se lahko koloci, kateri ID so se sprejemlnjivi poleg nasega
  hfdcan2_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0; //sprejeto sporocilo poslje v Rx FIFO 0
  hfdcan2_filter.FilterID1 = 0x63D;//ID nasega procesorja
  hfdcan2_filter.FilterID2 = 0x7FF;//maska
  if (HAL_FDCAN_ConfigFilter(&hfdcan2, &hfdcan2_filter) != HAL_OK)
  {
	Error_Handler();
  }

  //zavrze vsa sporočila, ki niso namenjena nasemu procesorju in zavrze vsa remote sporocila
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK)
  {
    Error_Handler();
  }

  //zazene CAN
  if(HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
  {
	Error_Handler();
  }

  //omogoci prekinitev za prejeto sporocilo preko CAN
  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    Error_Handler();
  }

  //konfiguracija registra za oddajanje sporocil preko CAN
  hfdcan2_TxHeader.Identifier = MASTER_ID;
  hfdcan2_TxHeader.IdType = FDCAN_STANDARD_ID;
  hfdcan2_TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  hfdcan2_TxHeader.DataLength = FDCAN_DLC_BYTES_8;
  //hfdcan2_TxHeader.ErrorStateIndicator
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
    uint32_t dolzinaDLC;
    SenderID = hfdcan2_RxHeader.Identifier;
    dolzinaDLC = hfdcan2_RxHeader.DataLength;
//////naredi prek dolzine prejetega sporocila, vec kot FF je sporocilo?
    if (SenderID == SLAVE_ID) {

        if(fdcanRxData[0] == XCP_PID_RES){
            //all good for FF, not for actual values
            if(dolzinaDLC == 1) {
                //samo prejme potrditev
            }
            if(state == 1) {
                //reads angle
                unsigned inLength = sizeof(angle);
                signed long byte_counter = inLength - 1;

                char *inBuffer = (char *)&angle;

                for(unsigned int i = 0; i < inLength; i++) {
                    inBuffer[byte_counter] = RxData[ i+1 ];
                    byte_counter--;
                }
                // calculate trq from PID
                PID_trq();
            }
            else if(state == 2) {
                //send trq command
                signed long byte_counter = sizeof(trq) - 1;
                unsigned char message2[8] = {0};
                memset(&message2, 0, sizeof(message2));

                message2[0] = XCP_PID_CMD_DOWNLOAD;

                if((byte_counter + 1) > 6)   message2[1] = 6;
                else                         message2[1] = byte_counter + 1;

                for(int i=0; i < message2[1];i++) {
                    char *outBuffer = (char * )&trq;
                    message2[i+2] = outBuffer[byte_counter];
                    byte_counter--;
                }

                uint8_t * msg_send2 = message2;
                static uint32_t TxMailbox2;        //just a location in which TxMailbox is stored
                if(HAL_CAN_GetTxMailboxesFreeLevel(hcan)) {
                    // We can only send if a free TX mailbox is available
                    // Otherwise drop the current cycle data
                    if (HAL_CAN_AddTxMessage(hcan, &TxHeader, msg_send2, &TxMailbox2) != HAL_OK) {
                        /* Transmission request Error */
                        Error_Handler();
                    }
                }
                state = 3;
            }
            else if(state == 3) {
                //any final reception ok, default state after any send
                state = 0;
                read_angle();
            }
            else if(state == 4) {
                //any final reception ok
                //state = 0;
                //end PID if called at any time
            }
        }
        else if(RxData[0] == XCP_PID_ERR) {
            //error
            trq = 0;
            state = 99;
            CAN_XCP_write(RequestedTorque, inExtension0, sizeof(trq), (char *)&trq); //request without interrupts safety I guess??
        }
        else
        //not good, error?
        trq = 0;
        state = 42;
        CAN_XCP_write(RequestedTorque, inExtension0, sizeof(trq), (char *)&trq); //request without interrupts safety I guess??

    }
}

void CAN_XCP_connect() {        //XCP connect message, probably 0xFF00 is enough

      unsigned char message[2] = {0};

      memset(&message, 0, sizeof(message));
      message[0] = XCP_PID_CMD_CONNECT;     //0xFF
      message[1] = 0x00;                    //XCP normal mode

      uint8_t * msg_send = message;

      hfdcan2_TxHeader.DataLength = sizeof(message);
      static uint32_t TxMailbox;        //just a location in which TxMailbox is stored

      if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) {
        // We can only send if a free TX mailbox is available
        // Otherwise drop the current cycle data

        if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, msg_send, &TxMailbox) != HAL_OK)
        {
          /* Transmission request Error */
          Error_Handler();
        }
      }

#warning "mogoce potrebno spremeniti"
      while (CAN_XCP_response() != XCP_PID_RES) {
          HAL_Delay(5);
      }
}

void write_trq(void) {
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

    hfdcan2_TxHeader.DataLength = sizeof(message);
    //static can_fesp_t TxData;         //8 byte data in union to be send
    static uint32_t TxMailbox;        //just a location in which TxMailbox is stored

    if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) {
        // We can only send if a free TX mailbox is available
        // Otherwise drop the current cycle data

        if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, msg_send, &TxMailbox) != HAL_OK) {
          /* Transmission request Error */
          Error_Handler();
        }
    }
    state = 2;

}

void CAN_XCP_write(unsigned inAddress, unsigned char inExtension, unsigned inLength, char *outBuffer) {        //XCP write msg, address, extension 0 or 1, length? buffer?

      unsigned char message[8] = {0};
      signed long byte_counter = inLength - 1;
      //unsigned long byte_counter_old = 0;

      memset(&message, 0, sizeof(message));
      message[0] = XCP_PID_CMD_SET_MTA;
      message[3] = inExtension;
      message[4] = (unsigned char)(inAddress >> 24);
      message[5] = (unsigned char)(inAddress >> 16);
      message[6] = (unsigned char)(inAddress >> 8);
      message[7] = (unsigned char)(inAddress >> 0);

      uint8_t * msg_send = message;     //to reši vse nadaljne zadeve

      hfdcan2_TxHeader.DataLength = sizeof(message);
      //static can_fesp_t TxData;         //8 byte data in union to be send
      static uint32_t TxMailbox;        //just a location in which TxMailbox is stored

      if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) {
        // We can only send if a free TX mailbox is available
        // Otherwise drop the current cycle data

        if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, msg_send, &TxMailbox) != HAL_OK)
        {
          /* Transmission request Error */
          Error_Handler();
        }
      }

#warning "tudi mogoce potrebno spremeniti"
      while (CAN_XCP_response() != XCP_PID_RES) {
          HAL_Delay(2);
      }

      //now the response was positive and we can send actual data we want
      unsigned char message2[8] = {0};
      memset(&message2, 0, sizeof(message2));

            message2[0] = XCP_PID_CMD_DOWNLOAD;

            if((byte_counter + 1) > 6)   message2[1] = 6;
            else                         message2[1] = byte_counter + 1;

            for(int i=0; i < message2[1];i++) {
                 message2[i+2] = outBuffer[byte_counter];
                 byte_counter--;
            }

            uint8_t * msg_send2 = message2;
            static uint32_t TxMailbox2;        //just a location in which TxMailbox is stored
            if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) {
              // We can only send if a free TX mailbox is available
              // Otherwise drop the current cycle data
              if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, msg_send2, &TxMailbox2) != HAL_OK)
              {
                /* Transmission request Error */
                Error_Handler();
              }
            }

            while (CAN_XCP_response() != XCP_PID_RES) {
                //HAL_Delay(10);
            }

}

void CAN_XCP_read(unsigned inAddress, unsigned char inExtension, unsigned inLength, char *inBuffer) {

    unsigned char message[8] = {0};
    signed long byte_counter = inLength - 1;

          memset(&message, 0, sizeof(message));
          message[0] = XCP_PID_CMD_SHORT_UPLOAD;
          message[1] = inLength;
          message[3] = inExtension;
          message[4] = (unsigned char)(inAddress >> 24);
          message[5] = (unsigned char)(inAddress >> 16);
          message[6] = (unsigned char)(inAddress >> 8);
          message[7] = (unsigned char)(inAddress >> 0);

          uint8_t * msg_send = message;

          TxHeader.DLC = sizeof(message);
          static uint32_t TxMailbox;

          if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan)) {
            // We can only send if a free TX mailbox is available
            // Otherwise drop the current cycle data

            if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, msg_send, &TxMailbox) != HAL_OK)
            {
              /* Transmission request Error */
              Error_Handler();
            }
          }
          //HAL_Delay(5);
    //read response
          uint16_t SenderID = 0x63D;
    while(SenderID != SLAVE_ID) {
          while(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0) {
                  //idk wait?
                  //HAL_Delay(2);
              }
              if(HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
                  {
                      /* Reception Error */
                      Error_Handler();
                  }

             SenderID = RxHeader.StdId;

             //if (SenderID == SLAVE_ID) {
                 //break;
                 /*
                 if(RxData[0] == XCP_PID_RES){
                     //all good for FF, not for actual values
                     break;
                 }
                 */
                 //else if(RxData[0] == XCP_PID_ERR) {
                     //error
                     //idk break;?
                 //}
             //}
    }

    for(unsigned int i = 0; i < inLength; i++) {
        inBuffer[byte_counter] = RxData[ i+1 ];
        byte_counter--;
    }

}


char CAN_XCP_INIT() {
    CAN_XCP_connect();
    //init
    unsigned char SetUp = 1;
    CAN_XCP_write(Motorcontrol_Mode, 0, sizeof(SetUp), (char *)&SetUp);
    CAN_XCP_write(Motorcontrol_Mode_S, 1, sizeof(SetUp), (char *)&SetUp);
    CAN_XCP_write(MTC_Switch, 0, sizeof(SetUp), (char *)&SetUp);
    CAN_XCP_write(MTC_S_Switch, 1, sizeof(SetUp), (char *)&SetUp);

    return 1; //all good
}
//response for commands
int CAN_XCP_response() {    //done by interrupt, from now on used for init only
    ////get response
        ///rework this return thingie
    while(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0) {

    }
        if(HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
            {
                /* Reception Error */
                Error_Handler();
            }

       uint16_t SenderID = RxHeader.StdId;

       if (SenderID == SLAVE_ID) {
           if(RxData[0] == XCP_PID_RES){
               //all good for FF, not for actual values
               return RxData[0];
           }
           else if(RxData[0] == XCP_PID_ERR) {
               //error
               return RxData[0];
           }
           else
               //not good
               return RxData[0];
       }
     return 0;
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
