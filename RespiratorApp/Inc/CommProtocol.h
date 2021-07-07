/*
 * CommProtocol.h
 *
 * Created: 3. 04. 2020 17:51:10
 *  Author: maticpi
 */ 


#ifndef COMMPROTOCOL_H_
#define COMMPROTOCOL_H_

#include <string.h>
#include "CommonDefinitions.h"
#include "ErrorHandling.h"
#include "Metrics.h"

typedef struct PROC_MSG_STATE_T {
  uint8_t state;
  char param;
  int32_t value;
} ProcMsgState_t;

int PrepareStatusMessage(uint32_t timestamp, int16_t Flow, int16_t Pressure, int16_t Volume, int16_t MotorPosition,\
int16_t MotorCurrent, int16_t MotorDutyCycle, uint16_t BreathCounter, uint8_t Status, ErrCodes_t Error, float target_value, char *p_msg);
int ReportAllCurrentSettings(char *p_msg, int MAX_LENGTH, RespSettings_t *Settings);
int PrepareMetricsMessage(char *p_msg, int MAX_LENGTH, BreathCycleMetrics_t* p_stat);
int PrepareStatisticsMessage(char *p_msg, int MAX_LENGTH, MetricsStatistics_t* p_stat);

void ProcessMessages(char data, RespSettings_t* Settings, ProcMsgState_t* pstate, uint8_t* newdata);
#endif /* COMMPROTOCOL_H_ */
