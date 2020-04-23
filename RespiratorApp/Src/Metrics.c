/*
 * Metrics.c
 *
 *  Created on: 23. apr. 2020
 *      Author: maticpi
 */

#include "Metrics.h"

BreathCycleHist_t BreathCycleHistory;
float AvgCycleTime, MinCycleTime, MaxCycleTime;
float AvgBPM,   MinBPM,   MaxBPM;
float AvgVt,    MinVt,    MaxVt;
float AvgRCexp, MinRCexp, MaxRCexp;
float AvgMAP,   MinMAP,   MaxMAP;
float AvgPEF,   MinPEF,   MaxPEF;
float MinuteVolume;

void MetricsInit(void)
{
  BreathCycleHistory.in=0;
  BreathCycleHistory.out=0;
  BreathCycleHistory.count=0;
}

void MetricsNextCycle(void)
{
  int i;
  int idx_out;

  uint32_t SumCycT=0;
  float SumTidalVolume=0;
  float SumRCexpiria=0;
  float SumMeanAirwayPressure=0;
  float SumPeakExpiratoryFlow=0;

  uint32_t MinCycT=UINT32_MAX;
  float MinTidalVolume=1e6;
  float MinRCexpiria=1e6;
  float MinMeanAirwayPressure=1e6;
  float MinPeakExpiratoryFlow=1e6;

  uint32_t MaxCycT=0;
  float MaxTidalVolume=-1e6;
  float MaxRCexpiria=-1e6;
  float MaxMeanAirwayPressure=-1e6;
  float MaxPeakExpiratoryFlow=-1e6;

  //Calculate averages
  idx_out=BreathCycleHistory.out;
  for (i=0; i<(BreathCycleHistory.count+1); i++)
  {
    SumCycT += BreathCycleHistory.cycleTimes[idx_out];
    SumTidalVolume += BreathCycleHistory.TidalVolumes[idx_out];
    SumRCexpiria += BreathCycleHistory.RCexp[idx_out];
    SumMeanAirwayPressure += BreathCycleHistory.MeanAirwayPressure[idx_out];
    SumPeakExpiratoryFlow += BreathCycleHistory.PeakExpiratoryFlow[idx_out];

    if (BreathCycleHistory.cycleTimes[idx_out] < MinCycT) MinCycT = BreathCycleHistory.cycleTimes[idx_out];
    if (BreathCycleHistory.TidalVolumes[idx_out] < MinTidalVolume) MinTidalVolume = BreathCycleHistory.TidalVolumes[idx_out];
    if (BreathCycleHistory.RCexp[idx_out] < MinRCexpiria) MinRCexpiria = BreathCycleHistory.RCexp[idx_out];
    if (BreathCycleHistory.MeanAirwayPressure[idx_out] < MinMeanAirwayPressure) MinMeanAirwayPressure = BreathCycleHistory.MeanAirwayPressure[idx_out];
    if (BreathCycleHistory.MeanAirwayPressure[idx_out] < MinPeakExpiratoryFlow) MinPeakExpiratoryFlow = BreathCycleHistory.MeanAirwayPressure[idx_out];

    if (BreathCycleHistory.cycleTimes[idx_out] > MaxCycT) MaxCycT = BreathCycleHistory.cycleTimes[idx_out];
    if (BreathCycleHistory.TidalVolumes[idx_out] > MaxTidalVolume) MaxTidalVolume = BreathCycleHistory.TidalVolumes[idx_out];
    if (BreathCycleHistory.RCexp[idx_out] > MaxRCexpiria) MaxRCexpiria = BreathCycleHistory.RCexp[idx_out];
    if (BreathCycleHistory.MeanAirwayPressure[idx_out] > MaxMeanAirwayPressure) MaxMeanAirwayPressure = BreathCycleHistory.MeanAirwayPressure[idx_out];
    if (BreathCycleHistory.PeakExpiratoryFlow[idx_out] > MaxPeakExpiratoryFlow) MaxPeakExpiratoryFlow = BreathCycleHistory.PeakExpiratoryFlow[idx_out];

    idx_out++;
    if (idx_out > METRICS_MAX_HIST_LENGTH) idx_out = 0;
  }

  MinuteVolume = SumTidalVolume / SumCycT * 60000.0;

  AvgCycleTime = SumCycT/(BreathCycleHistory.count+1);
  MinCycleTime = MinCycT;
  MaxCycleTime = MaxCycT;

  if (AvgCycleTime > 0) AvgBPM = 60000.0 / AvgCycleTime;
  else AvgBPM = -1;
  if (MinCycT > 0) MinBPM = 60000.0 / MinCycT;
  else MinBPM = -1;
  if (MaxCycT > 0) MaxBPM = 60000.0 / MaxCycT;
  else MaxBPM = -1;

  AvgVt = SumTidalVolume/(BreathCycleHistory.count+1);
  MinVt = MinTidalVolume;
  MaxVt = MaxTidalVolume;

  AvgRCexp = SumRCexpiria/(BreathCycleHistory.count+1);
  MinRCexp = MinRCexpiria;
  MaxRCexp = MaxRCexpiria;

  AvgMAP = SumPeakExpiratoryFlow/(BreathCycleHistory.count+1);
  MinMAP = MinMeanAirwayPressure;
  MaxMAP = MaxMeanAirwayPressure;

  AvgPEF = SumPeakExpiratoryFlow/(BreathCycleHistory.count+1);
  MinPEF = MinPeakExpiratoryFlow;
  MaxPEF = MaxPeakExpiratoryFlow;

  //IncrementIndex
  BreathCycleHistory.in++;
  if (BreathCycleHistory.in > METRICS_MAX_HIST_LENGTH) BreathCycleHistory.in = 0;
  BreathCycleHistory.count++;
  if (SumCycT > METRICS_MAX_HIST_TIME)
  {
    BreathCycleHistory.out++;
    if (BreathCycleHistory.out > METRICS_MAX_HIST_LENGTH) BreathCycleHistory.out = 0;
    BreathCycleHistory.count--;
  }
}

void MetricsStoreCycTime(uint32_t time)
{
  BreathCycleHistory.cycleTimes[BreathCycleHistory.in]=time;
}

void MetricsStoreVt(float value)
{
  BreathCycleHistory.TidalVolumes[BreathCycleHistory.in]=value;
}

void MetricsStoreMAP(float value)
{
  BreathCycleHistory.MeanAirwayPressure[BreathCycleHistory.in]=value;
}

void MetricsStorePEF(float value)
{
  BreathCycleHistory.PeakExpiratoryFlow[BreathCycleHistory.in]=value;
}

void MetricsStoreRCexp(float value)
{
  BreathCycleHistory.RCexp[BreathCycleHistory.in]=value;
}

void MetricsStoreBreathTrigger(uint32_t timestamp, BreathTrigType_t type)
{
  BreathCycleHistory.BreathTriggerTimestamp[BreathCycleHistory.in]=timestamp;
  BreathCycleHistory.BreathTriggerType[BreathCycleHistory.in]=type;
}
