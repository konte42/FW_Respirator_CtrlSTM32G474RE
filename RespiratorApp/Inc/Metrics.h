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

typedef struct BREATH_CYCLE_METRICS_T{
  float ExpiriaTimes;
  float InspiriaTimes;
  float TidalVolumes;
  float MeanAirwayPressure;
  float PeakExpiratoryFlow;
  float RCexp;
  float BreathTriggerTimestamp;
  BreathTrigType_t BreathTriggerType;
} BreathCycleMetrics_t;

typedef struct BREATH_CYCLE_HIST_T{
  BreathCycleMetrics_t mtr[METRICS_MAX_HIST_LENGTH];
  int in;
  int out;
  int count;
}BreathCycleHist_t;

typedef union METRICS_UNION_T{
  struct {
    float AvgInspTime, MinInspTime, MaxInspTime;
    float AvgMAP,      MinMAP,     MaxMAP;
    float AvgVt,       MinVt,      MaxVt;

    float AvgExpTime,  MinExpTime, MaxExpTime;
    float AvgCycleTime, MinCycleTime, MaxCycleTime;
    float AvgBPM,      MinBPM,     MaxBPM;
    float AvgRCexp,    MinRCexp,   MaxRCexp;
    float AvgPEF,      MinPEF,     MaxPEF;
    float MinuteVolume;
  } cyc;
  struct  {
    struct {
      float AvgInspTime, MinInspTime, MaxInspTime;
      float AvgMAP,      MinMAP,     MaxMAP;
      float AvgVt,       MinVt,      MaxVt;
    } insp;
    struct  {
      float AvgExpTime,  MinExpTime, MaxExpTime;
      float AvgCycleTime, MinCycleTime, MaxCycleTime;
      float AvgBPM,      MinBPM,     MaxBPM;
      float AvgRCexp,    MinRCexp,   MaxRCexp;
      float AvgPEF,      MinPEF,     MaxPEF;
      float MinuteVolume;
    } exp;
  };
} MetricsStatistics_t;

extern BreathCycleMetrics_t Metrics;
extern MetricsStatistics_t Statistics;

void MetricsNextCycle(void);
void MetricsInit(void);
void MetricsStoreInspTime(uint32_t time);
void MetricsStoreExpTime(uint32_t time);
void MetricsStoreVt(float value);
void MetricsStoreMAP(float value);
void MetricsStorePEF(float value);
void MetricsStoreRCexp(float value);
void MetricsStoreBreathTrigger(uint32_t timestamp, BreathTrigType_t type);

#endif /* INC_METRICS_H_ */
