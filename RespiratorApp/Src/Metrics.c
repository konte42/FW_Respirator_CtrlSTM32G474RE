/*
 * Metrics.c
 *
 *  Created on: 23. apr. 2020
 *      Author: maticpi
 */

#include "Metrics.h"

BreathCycleMetrics_t Metrics;
BreathCycleHist_t BreathCycleHistory;
MetricsStatistics_t Statistics;

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
  uint32_t cycTime;

  uint32_t SumCycT=0;
  uint32_t SumInspT=0;
  uint32_t SumExpT=0;
  float SumTidalVolume=0;
  float SumRCexpiria=0;
  float SumMeanAirwayPressure=0;
  float SumPeakExpiratoryFlow=0;

  uint32_t MinCycT=UINT32_MAX;
  uint32_t MinInspT=UINT32_MAX;
  uint32_t MinExpT=UINT32_MAX;
  float MinTidalVolume=1e6;
  float MinRCexpiria=1e6;
  float MinMeanAirwayPressure=1e6;
  float MinPeakExpiratoryFlow=1e6;

  uint32_t MaxCycT=0;
  uint32_t MaxInspT=UINT32_MAX;
  uint32_t MaxExpT=UINT32_MAX;
  float MaxTidalVolume=-1e6;
  float MaxRCexpiria=-1e6;
  float MaxMeanAirwayPressure=-1e6;
  float MaxPeakExpiratoryFlow=-1e6;

  //Calculate averages
  idx_out=BreathCycleHistory.out;
  for (i=0; i<(BreathCycleHistory.count+1); i++)
  {
    cycTime = BreathCycleHistory.mtr[idx_out].InspiriaTimes + BreathCycleHistory.mtr[idx_out].ExpiriaTimes;
    SumCycT += cycTime;
    SumInspT += BreathCycleHistory.mtr[idx_out].InspiriaTimes;
    SumExpT +=  BreathCycleHistory.mtr[idx_out].ExpiriaTimes;
    SumTidalVolume += BreathCycleHistory.mtr[idx_out].TidalVolumes;
    SumRCexpiria += BreathCycleHistory.mtr[idx_out].RCexp;
    SumMeanAirwayPressure += BreathCycleHistory.mtr[idx_out].MeanAirwayPressure;
    SumPeakExpiratoryFlow += BreathCycleHistory.mtr[idx_out].PeakExpiratoryFlow;

    if (cycTime < MinCycT) MinCycT = cycTime;
    if (BreathCycleHistory.mtr[idx_out].InspiriaTimes < MinInspT) MinInspT = BreathCycleHistory.mtr[idx_out].InspiriaTimes;
    if (BreathCycleHistory.mtr[idx_out].ExpiriaTimes < MinExpT) MinExpT = BreathCycleHistory.mtr[idx_out].ExpiriaTimes;
    if (BreathCycleHistory.mtr[idx_out].TidalVolumes < MinTidalVolume) MinTidalVolume = BreathCycleHistory.mtr[idx_out].TidalVolumes;
    if (BreathCycleHistory.mtr[idx_out].RCexp < MinRCexpiria) MinRCexpiria = BreathCycleHistory.mtr[idx_out].RCexp;
    if (BreathCycleHistory.mtr[idx_out].MeanAirwayPressure < MinMeanAirwayPressure) MinMeanAirwayPressure = BreathCycleHistory.mtr[idx_out].MeanAirwayPressure;
    if (BreathCycleHistory.mtr[idx_out].MeanAirwayPressure < MinPeakExpiratoryFlow) MinPeakExpiratoryFlow = BreathCycleHistory.mtr[idx_out].MeanAirwayPressure;

    if (cycTime > MaxCycT) MaxCycT = cycTime;
    if (BreathCycleHistory.mtr[idx_out].InspiriaTimes > MaxInspT) MaxInspT = BreathCycleHistory.mtr[idx_out].InspiriaTimes;
    if (BreathCycleHistory.mtr[idx_out].ExpiriaTimes > MaxExpT) MaxExpT = BreathCycleHistory.mtr[idx_out].ExpiriaTimes;
    if (BreathCycleHistory.mtr[idx_out].TidalVolumes > MaxTidalVolume) MaxTidalVolume = BreathCycleHistory.mtr[idx_out].TidalVolumes;
    if (BreathCycleHistory.mtr[idx_out].RCexp > MaxRCexpiria) MaxRCexpiria = BreathCycleHistory.mtr[idx_out].RCexp;
    if (BreathCycleHistory.mtr[idx_out].MeanAirwayPressure > MaxMeanAirwayPressure) MaxMeanAirwayPressure = BreathCycleHistory.mtr[idx_out].MeanAirwayPressure;
    if (BreathCycleHistory.mtr[idx_out].PeakExpiratoryFlow > MaxPeakExpiratoryFlow) MaxPeakExpiratoryFlow = BreathCycleHistory.mtr[idx_out].PeakExpiratoryFlow;

    idx_out++;
    if (idx_out >= METRICS_MAX_HIST_LENGTH) idx_out = 0;
  }

  Statistics.cyc.MinuteVolume = SumTidalVolume / SumCycT * 60000.0;

  Statistics.cyc.AvgCycleTime = SumCycT/(BreathCycleHistory.count+1);
  Statistics.cyc.MinCycleTime = MinCycT;
  Statistics.cyc.MaxCycleTime = MaxCycT;

  Statistics.cyc.AvgInspTime = SumInspT/(BreathCycleHistory.count+1);
  Statistics.cyc.MinInspTime = MinInspT;
  Statistics.cyc.MaxInspTime = MaxInspT;

  Statistics.cyc.AvgExpTime = SumExpT/(BreathCycleHistory.count+1);
  Statistics.cyc.MinExpTime = MinExpT;
  Statistics.cyc.MaxExpTime = MaxExpT;

  if (Statistics.cyc.AvgCycleTime > 0) Statistics.cyc.AvgBPM = 60000.0 / Statistics.cyc.AvgCycleTime;
  else Statistics.cyc.AvgBPM = -1;
  if (MinCycT > 0) Statistics.cyc.MinBPM = 60000.0 / MinCycT;
  else Statistics.cyc.MinBPM = -1;
  if (MaxCycT > 0) Statistics.cyc.MaxBPM = 60000.0 / MaxCycT;
  else Statistics.cyc.MaxBPM = -1;

  Statistics.cyc.AvgVt = SumTidalVolume/(BreathCycleHistory.count+1);
  Statistics.cyc.MinVt = MinTidalVolume;
  Statistics.cyc.MaxVt = MaxTidalVolume;

  Statistics.cyc.AvgRCexp = SumRCexpiria/(BreathCycleHistory.count+1);
  Statistics.cyc.MinRCexp = MinRCexpiria;
  Statistics.cyc.MaxRCexp = MaxRCexpiria;

  Statistics.cyc.AvgMAP = SumPeakExpiratoryFlow/(BreathCycleHistory.count+1);
  Statistics.cyc.MinMAP = MinMeanAirwayPressure;
  Statistics.cyc.MaxMAP = MaxMeanAirwayPressure;

  Statistics.cyc.AvgPEF = SumPeakExpiratoryFlow/(BreathCycleHistory.count+1);
  Statistics.cyc.MinPEF = MinPeakExpiratoryFlow;
  Statistics.cyc.MaxPEF = MaxPeakExpiratoryFlow;

  //IncrementIndex
  BreathCycleHistory.in++;
  if (BreathCycleHistory.in >= METRICS_MAX_HIST_LENGTH) BreathCycleHistory.in = 0;
  BreathCycleHistory.count++;
  if (BreathCycleHistory.count > METRICS_MAX_HIST_LENGTH)
  {
    BreathCycleHistory.out++;
    if (BreathCycleHistory.out >= METRICS_MAX_HIST_LENGTH) BreathCycleHistory.out = 0;
    BreathCycleHistory.count--;
  }
  while (SumCycT > METRICS_MAX_HIST_TIME)
  {
    SumCycT -= (BreathCycleHistory.mtr[BreathCycleHistory.out].InspiriaTimes + BreathCycleHistory.mtr[BreathCycleHistory.out].ExpiriaTimes);
    BreathCycleHistory.out++;
    if (BreathCycleHistory.out >= METRICS_MAX_HIST_LENGTH) BreathCycleHistory.out = 0;
    BreathCycleHistory.count--;
  }
}

void MetricsStoreInspTime(uint32_t time)
{
  BreathCycleHistory.mtr[BreathCycleHistory.in].InspiriaTimes=time;
  Metrics.InspiriaTimes=time/1000.0;
}

void MetricsStoreExpTime(uint32_t time)
{
  BreathCycleHistory.mtr[BreathCycleHistory.in].ExpiriaTimes=time;
  Metrics.ExpiriaTimes=time/1000.0;
}

void MetricsStoreVt(float value)
{
  BreathCycleHistory.mtr[BreathCycleHistory.in].TidalVolumes=value;
  Metrics.TidalVolumes=value;
}

void MetricsStoreMAP(float value)
{
  BreathCycleHistory.mtr[BreathCycleHistory.in].MeanAirwayPressure=value;
  Metrics.MeanAirwayPressure=value;
}

void MetricsStorePEF(float value)
{
  BreathCycleHistory.mtr[BreathCycleHistory.in].PeakExpiratoryFlow=value;
  Metrics.PeakExpiratoryFlow=value;
}

void MetricsStoreRCexp(float value)
{
  BreathCycleHistory.mtr[BreathCycleHistory.in].RCexp=value;
  Metrics.RCexp=value;
}

void MetricsStoreBreathTrigger(uint32_t timestamp, BreathTrigType_t type)
{
  BreathCycleHistory.mtr[BreathCycleHistory.in].BreathTriggerTimestamp=timestamp;
  BreathCycleHistory.mtr[BreathCycleHistory.in].BreathTriggerType=type;
  Metrics.BreathTriggerTimestamp=timestamp/1000.0;
  Metrics.BreathTriggerType=type;
}
