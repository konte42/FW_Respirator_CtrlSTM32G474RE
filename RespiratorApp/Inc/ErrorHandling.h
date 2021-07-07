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

#define DEFAULT_WARNING_THERSHOLD_LOW   1
#define DEFAULT_WARNING_THERSHOLD_HIGH  2
#define DEFAULT_ERROR_THERSHOLD_LOW     3
#define DEFAULT_ERROR_THERSHOLD_HIGH    4
#define DEFAULT_ERR_MAX_COUNT 5

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
	NoError = 0x0000,
	//0x0001 - 0x001F: Informational messages
  ErrQueueEmpty = 0x0001,    //This is ok - no errors
  DbgMsg = 0x0002,

  Info_Limits_MaxPosition = 0x0010,
  Info_Battery_Not_Fully_Charged,

  //0x0020 - ...: Warnings
  Warning_OnBatteryPower = 0x0020,

  //0x00A0 - 0x00BF: Communication errors
  ComRxUnknownParameter = 0x00A0,
  ComRxNoSpaceAfterParam,
  ComRxExpectingNumber,
  ComRxNoEtx,
  ComRxUnexpectedStx,
  //Parameter limits
  ComRxUnknownMode,

  ComRxMuteTimeOutsideLimits,
  ComRxRampOutsideLimits,
  ComRxInspTimeOutsideLimits,
  ComRxExpTimeOutsideLimits,
  ComRxPEEPOutsideLimits,
  ComRxPeakInspPressureOutsideLimits,
  ComRxMinInspPressureOutsideLimits,
  ComRxTargetPressureOutsideLimits,
  ComRxVolumeOutsideLimits,
  ComRxMotorPowerOutsideLimits,
  ComRxMotorSpeedOutsideLimits,
  ComRxInspiriaTriggerPressureOutsideLimits,
  ComRxETS_OutsideLimits,
  ComRxApneaTimeLimitMaxOtsideLimits,
  ComRxTidalVolumeLimitMinOtsideLimits,
  ComRxTidalVolumeLimitMaxOtsideLimits,
  ComRxBreathRateLimitMinOutsideLimits,
  ComRxBreathRateLimitMaxOutsideLimits,
  ComRxMinuteVolumeLimitMinOtsideLimits,
  ComRxMinuteVolumeLimitMaxOtsideLimits,
  ComRxPIDPOutsideLimits,
  ComRxPIDIOutsideLimits,
  ComRxPIDDOutsideLimits,
  ComRxPIDmaxErrOutsideLimits,
  ComRxPIDmaxSumErrOutsideLimits,
  ComRxPIDmaxOutOutsideLimits,
  ComRxPIDminOutOutsideLimits,

  //0x00D0 - 0x00DF: System errors
  ComRxBuffOverrun = 0x00D0,
  ComRxUnknownState,
  Error_BatteryAlmostEmpty,
  Error_UnknownGlobalSystemError,

  //0x00E0 - 0x00FF: System errors - critical
  ActuatorCtrlUnknownMode = 0x00E0,
  ModeUnknownMode,
  ModeCMV_UnknownState,
  ModePCV_UnknownState,
  ModePS_UnknownState,

  //0x1000 - 0x15FF: Info (Medical)
  InfoCycleEND_InspTime             = 0x1000,
  Info_Limits_MinTidalVolume,
  Info_Limits_MaxTidalVolume,
  Info_Limits_PeakPressure,
  Info_Limits_MinPressure,
	//0x2000 - 0x1FFF: Warnings (Medical)
  Warning_Limits_PeakPreassure      = 0x2000,
  Warning_Limits_MinPressure,
  Warning_Limits_MinTidalVolume,
  Warning_Limits_MaxTidalVolume,
	//0x3000 - ...: Errors (Medical)
  Error_Limits_PeakPreassure        = 0x3000,
  Error_Limits_MinPressure,
  Error_Limits_MinTidalVolume,
  Error_Limits_MaxTidalVolume,
} ErrCodes_t;

typedef enum {
  ErrPowerSupply          = 0x00,
  ErrPowerNoBattery       = 0x01,
  ErrMotorPositionSensor  = 0x02,
  ErrFlowSensor           = 0x03,
  ErrPressureSensor       = 0x04
} GlobalErrors_t;

typedef struct GLOBAL_ERR_STATUS_T{
  uint32_t  errors1;
} GlobalErrStatus_t;

GlobalErrStatus_t GlobalErrorStatus;

#define GLOBAL_ERROR_STATUS_SIZE  sizeof(GlobalErrStatus_t)

typedef enum ERR_STATUS_T{
  ERR_STATUS_OK = 0x00,
  ERR_STATUS_WARNING = 0x01,
  ERR_STATUS_ERROR = 0x10
} ErrStatus_t;

typedef struct ERR_STATISTICS_T{
  int cnt;
  ErrStatus_t status;
  int WarningLowThreshold;
  int WarningHighThreshold;
  int ErrorLowThreshold;
  int ErrorHighThreshold;
  int ErrMaxCount;
  ErrCodes_t info;
  ErrCodes_t warning;
  ErrCodes_t error;
} ErrStatistics_t;  //Errors that need to happen a few times

#define ERROR_QUEUE_LENGTH	50
#define ERROR_QUEUE_STOP_WHEN_FULL	1

typedef struct ERR_QUEUE{
	ErrCodes_t ErrorQueue[ERROR_QUEUE_LENGTH]; //Array with ERROR_QUEUE_LENGTH stored errors
	uint8_t QueueIn;						//Index for writing into buffer
	uint8_t QueueOut;						//Index for reading from buffer
	uint8_t QueueNum;						//Number of stored data in buffer
} ErrQueue_t;

ErrStatistics_t  Err_Limit_PeakPreasure;
ErrStatistics_t  Err_Limit_MaxTidalVolume;
ErrStatistics_t  Err_Limit_MinPressure;
ErrStatistics_t  Err_Limit_MinTidalVolume;

void IncError(ErrStatistics_t *err);
void DecError(ErrStatistics_t *err);
void ClrError(ErrStatistics_t *err);
char* GetErrorString(ErrCodes_t errorCode);

extern ErrQueue_t DefaultErrorQueue;
extern int buzzerMute;

void ErrorStatusUpadate(GlobalErrors_t err, ErrStatus_t status);
void ErrQueue_Init(struct ERR_QUEUE *ErrQueue);
void ErrQueue_StoreErr (ErrCodes_t ErrCode, struct ERR_QUEUE *ErrQueue);	//No point in returning error - who to? This is the error handling code.
ErrCodes_t ErrQueue_GetErr (ErrCodes_t* ErrCode, struct ERR_QUEUE *ErrQueue);
uint8_t ErrQueue_NumberOfErrors();
void ErrorBuzzer();

#if USE_DEFAULT_ERROR_REPORTER
void ReportError(ErrCodes_t ErrCode, char ErrMsg[]);
#endif

#endif /* ERRORHANDLING_H_ */
