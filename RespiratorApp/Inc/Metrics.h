/*
 * Metrics.h
 *
 *  Created on: 23. apr. 2020
 *      Author: maticpi
 */

#ifndef INC_METRICS_H_
#define INC_METRICS_H_

#include <inttypes.h>
#include "CommonDefinitions.h"

#ifndef METRICS_MAX_HIST_LENGTH
#define METRICS_MAX_HIST_LENGTH   10
#warning "METRICS_MAX_HIST_LENGTH not defined. Using default value of 10"
#endif

#ifndef METRICS_MIN_HIST_LENGTH
#define METRICS_MIN_HIST_LENGTH   5
#warning "METRICS_MIN_HIST_LENGTH not defined. Using default value of 5"
#endif

#ifndef METRICS_MAX_HIST_TIME
#define METRICS_MAX_HIST_TIME   60000
#warning "METRICS_MAX_HIST_TIME not defined. Using default value of 60s (60000)"
#endif

typedef enum BREATH_TRIGGER_TYPE_T{
  TRIG_TYPE_TIME,
  TRIG_TYPE_PRESSURE_TRIGGER
} BreathTrigType_t;

typedef struct BREATH_CYCLE_HIST_T{
  uint32_t cycleTimes[METRICS_MAX_HIST_LENGTH];
  float TidalVolumes[METRICS_MAX_HIST_LENGTH];
  float MeanAirwayPressure[METRICS_MAX_HIST_LENGTH];
  float PeakExpiratoryFlow[METRICS_MAX_HIST_LENGTH];
  float RCexp[METRICS_MAX_HIST_LENGTH];
  BreathTrigType_t BreathTriggerType[METRICS_MAX_HIST_LENGTH];
  uint32_t BreathTriggerTimestamp[METRICS_MAX_HIST_LENGTH];
  int in;
  int out;
  int count;
}BreathCycleHist_t;

extern float AvgCycleTime, MinCycleTime, MaxCycleTime;
extern float AvgBPM,      MinBPM,     MaxBPM;
extern float AvgVt,       MinVt,      MaxVt;
extern float AvgRCexp,    MinRCexp,   MaxRCexp;
extern float AvgMAP,      MinMAP,     MaxMAP;
extern float AvgPEF,      MinPEF,     MaxPEF;

void MetricsNextCycle(void);
void MetricsInit(void);
void MetricsStoreCycTime(uint32_t time);
void MetricsStoreVt(float value);
void MetricsStoreMAP(float value);
void MetricsStorePEF(float value);
void MetricsStoreRCexp(float value);
void MetricsStoreBreathTrigger(uint32_t timestamp, BreathTrigType_t type);


#endif /* INC_METRICS_H_ */
