/*
 * ErrorHandling.h
 *
 * Created: 3. 04. 2020 18:32:05
 *  Author: maticpi
 */ 


#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_

#include <inttypes.h>
#include <stddef.h>

#define USE_DEFAULT_ERROR_REPORTER	1
//Default Error reporter just stores errors in an Error Queue
//Copy "void ReportError(ErrCodes_t ErrCode, char ErrMsg[])"
//function and make your own hardware/application specific version

//FlashStringHelper
#define FSH(x)  x
//#define FSH(x)  PSTR(x) //For AVR - strings stored in flash
//#define FSH(x)  NULL  //Remove all strings

//Error codes
typedef enum ErrCodes
{
	NoError = 0x00,
	//0x01 - 0x1F: Informational messages
  ErrQueueEmpty = 0x01,    //This is ok - no errors
  DbgMsg,

	//0x20 - 0x4F: Warnings (Medical)
  Limits_InsufficientVolume = 0x20,
  Limits_VolumeTooHigh,

	//0x50 - 0x7F: Errors (Medical)

  //0x80 - 0x9F: Communication errors
  ComRxUnknownParameter = 0x80,
  ComRxNoSpaceAfterParam,
  ComRxExpectingNumber,
  ComRxNoEtx,
  ComRxUnexpectedStx,
  //Parameter limits
  ComRxUnknownMode,

  ComRxRampOutsideLimits,
  ComRxInspTmOutsideLimits,
  ComRxExpTmOutsideLimits,
  ComRxPEEPOutsideLimits,
  
  ComRxPeakInspPressureOutsideLimits,
  ComRxMinInspPressureOutsideLimits,
  ComRxTargetPressureOutsideLimits,
  ComRxVolumeOutsideLimits,

  ComRxInspiriaTriggerPressureOutsideLimits,
  ComRxETS_OutsideLimits,

  ComRxBreathRateLimitMinOtsideLimits,
  ComRxBreathRateLimitMaxOtsideLimits,

  ComRxPIDPOutsideLimits,
  ComRxPIDIOutsideLimits,
  ComRxPIDDOutsideLimits,
  ComRxPIDmaxErrOutsideLimits,
  ComRxPIDmaxSumErrOutsideLimits,
  ComRxPIDmaxOutOutsideLimits,
  ComRxPIDminOutOutsideLimits,

  //0xA0 - 0xBF: System errors
  ComRxBuffOverrun = 0xA0,
  ComRxUnknownState,

  //0xC0 - 0xFF: System errors - critical
	ActuatorCtrlUnknownMode = 0xC0,
	ModeUnknownMode,
	ModeC_VCV_UnknownState,
	ModeC_PCV_UnknownState,
	ModeCPAP_PS_UnknownState
} ErrCodes_t;

#define ERROR_QUEUE_LENGTH	50
#define ERROR_QUEUE_STOP_WHEN_FULL	1

typedef struct ERR_QUEUE{
	ErrCodes_t ErrorQueue[ERROR_QUEUE_LENGTH]; //Array with ERROR_QUEUE_LENGTH stored errors
	uint8_t QueueIn;						//Index for writing into buffer
	uint8_t QueueOut;						//Index for reading from buffer
	uint8_t QueueNum;						//Number of stored data in buffer
} ErrQueue_t;

ErrQueue_t DefaultErrorQueue;

void ErrQueue_Init(struct ERR_QUEUE *ErrQueue);
void ErrQueue_StoreErr (ErrCodes_t ErrCode, struct ERR_QUEUE *ErrQueue);	//No point in returning error - who to? This is the error handling code.
ErrCodes_t ErrQueue_GetErr (ErrCodes_t* ErrCode, struct ERR_QUEUE *ErrQueue);
uint8_t ErrQueue_NumberOfErrors();

#if USE_DEFAULT_ERROR_REPORTER
void ReportError(ErrCodes_t ErrCode, char ErrMsg[]);
#endif

#endif /* ERRORHANDLING_H_ */
