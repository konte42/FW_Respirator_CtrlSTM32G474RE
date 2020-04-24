/*
 * ErrorHandling.c
 *
 * Created: 3. 04. 2020 18:31:24
 *  Author: maticpi
 */ 
#include <string.h>
#include "ErrorHandling.h"
#include "gpio.h"

//Figure out where it would make sense to put ErrorReporter and put one there
#include "main.h"
extern UART_HandleTypeDef hlpuart1;

int buzzerState=0;
int buzzerMute=0; //

void ErrorBuzzer()
{
  if (buzzerMute > 0)
  {
    BUZZ_Off();
    buzzerMute--;
    buzzerState=0;
  }
  else
  {
    if (buzzerState == 1) //Warning beep once a second
    {
      if ((HAL_GetTick() % 1000) < 200) BUZZ_On();
      else BUZZ_Off();
    }
    else if (buzzerState > 1)
    {
      BUZZ_On();
    }
    else
    {
      BUZZ_Off();
    }
  }
}

void IncError(ErrStatistics_t *err)
{
  err->cnt++;
  if (err->cnt > err->ErrMaxCount) err->cnt = err->ErrMaxCount;

  if (err->status == ERR_STATUS_OK)
  {
      if (err->cnt > err->WarningHighThreshold)
      {
        err->status = ERR_STATUS_WARNING;
        if (buzzerState < 2) buzzerState = 1;
      }
  }
  else if (err->status == ERR_STATUS_WARNING)
  {
    if (err->cnt > err->ErrorHighThreshold)
    {
      err->status = ERR_STATUS_ERROR;
      buzzerState = 2;
    }
    else buzzerState = 1;
  }
  else buzzerState = 2;

  if (err->status == ERR_STATUS_WARNING)
  {
    ReportError(err->warning,GetErrorString(err->warning));
  }
  else if (err->status == ERR_STATUS_ERROR)
  {
    ReportError(err->error,GetErrorString(err->error));
  }
  else
  {
    ReportError(err->info,GetErrorString(err->info));
  }
}

void DecError(ErrStatistics_t *err)
{
  err->cnt--;
  if (err->cnt < 0) err->cnt = 0;

  if (err->status == ERR_STATUS_WARNING)
  {
    if (err->cnt < err->WarningLowThreshold)
    {
      err->status = ERR_STATUS_OK;
    }
  }
  else if (err->status == ERR_STATUS_ERROR)
  {
    if (err->cnt < err->ErrorLowThreshold)
    {
      err->status = ERR_STATUS_WARNING;
    }
  }

  if (err->status == ERR_STATUS_WARNING)
  {
    ReportError(err->warning,GetErrorString(err->warning));
  }
  else if (err->status == ERR_STATUS_ERROR)
  {
    ReportError(err->error,GetErrorString(err->error));
  }
}

char* GetErrorString(ErrCodes_t errorCode)
{
  char* ptr_str;
  switch (errorCode)
  {
    case Warning_Limits_PeakPreassure: ptr_str=FSH("Warning:Peak Insp. Preasure! "); break;
    case Error_Limits_PeakPreassure: ptr_str=FSH("Error:Peak Insp. Preasure! "); break;
    default: ptr_str=NULL; break;
  }
  return ptr_str;
}

ErrQueue_t DefaultErrorQueue;

void ErrQueue_Init(struct ERR_QUEUE *ErrQueue)
{
	ErrQueue->QueueIn = 0;
	ErrQueue->QueueOut = 0;
	ErrQueue->QueueNum = 0;
}

void ErrQueue_StoreErr (ErrCodes_t ErrCode, struct ERR_QUEUE *ErrQueue)
{
#if ERROR_QUEUE_STOP_WHEN_FULL
	if (ErrQueue->QueueNum < ERROR_QUEUE_LENGTH)
#endif
	{
		ErrQueue->ErrorQueue[ErrQueue->QueueIn] = ErrCode;
		ErrQueue->QueueNum++;
		ErrQueue->QueueIn++;
		if (ErrQueue->QueueIn >= ERROR_QUEUE_LENGTH) ErrQueue->QueueIn = 0;
	}
}

ErrCodes_t ErrQueue_GetErr (ErrCodes_t* ErrCode, struct ERR_QUEUE *ErrQueue)
{
	if(ErrQueue->QueueNum <= 0)
  {
	  *ErrCode = NoError;
	  return ErrQueueEmpty;
  }

	*ErrCode = ErrQueue->ErrorQueue[ErrQueue->QueueOut];
	ErrQueue->QueueNum--;
	ErrQueue->QueueOut++;
	
	if (ErrQueue->QueueOut >= ERROR_QUEUE_LENGTH) ErrQueue->QueueOut = 0;
	return NoError;
}

uint8_t ErrQueue_NumberOfErrors(struct ERR_QUEUE *ErrQueue)
{
	return ErrQueue->QueueNum;
}

#if USE_DEFAULT_ERROR_REPORTER
void ReportError(ErrCodes_t ErrCode, char ErrMsg[])
{
	ErrQueue_StoreErr(ErrCode,&DefaultErrorQueue);
	//printf("Error %d: %s,ErrCode,ErrMsg);
	//HAL_UART_Transmit_IT(&hlpuart1, (uint8_t*)ErrMsg, strlen(ErrMsg));
}
#endif
