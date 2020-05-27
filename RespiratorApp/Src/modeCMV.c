/*
 * CFile1.c
 *
 * Created: 2. 04. 2020 09:27:40
 *  Author: mjankovec
 */ 
//Opisi mode, da vemo kaj pocnemo

//Pri lovljenju ustreznega volumna ima prednost �AS (Target inspiratory time)
#include <stdio.h>
#include <modeCMV.h>
#include "Measure.h"
#include "GPIO.h"

float FIR(float new_x, int reset, float reset_val);

ModeStates_t dihanje_state = MODE_STATE_FIRST_RUN;
int16_t timing;
uint16_t SETinsp_time;
uint16_t SETexp_time;
uint16_t SETpramp_time;
int16_t PreStartBoostTime;
float PREP_CURRENT_START = 0;
float PREP_CURRENT_MAX = 10;
float PREP_T_TOTAL = 50;
float PREP_T_START = 10;
float PREP_T_RAMP_I = 39;//(49-PREP_T_START);

float SET_PEEP;
float MINvolume;
float MINpressure;
float MAXpressure;
float PressureSafetyMargin = 10;

float TargetVolume = 0;
float TargetFlow = 0;
float StartFlow = 0;
float Pramp_rate = 0;
float InertiaVolume = 0;

uint16_t Measured_ExpTime=0;
uint16_t Measured_InspTime=0;
uint16_t Measured_PrampTime=0;
uint16_t Measured_PreStartTime=0;
float MaxMeasuredVolume;
float sumAirwayPressure;
float peakExpFlow;

#define MAX_PRAMP_TIME  200

void modeCMV(RespSettings_t* Settings, MeasuredParams_t* Measured, CtrlParams_t* Control)
{

  Control->status = dihanje_state;  // shrani stanje dihanja

  //State machine starts with exhalation.
  //At the end of exhalation all the settings are reloaded into local copies.
  //Local settings are used during the rest of the cycle,
  //so that incompatible settings can not be loaded at the wrong time

  switch (dihanje_state)
  {
    case MODE_STATE_FIRST_RUN:  //First time: init local settings, etc
      ClrError(&Err_Limit_PeakPreasure);
      ClrError(&Err_Limit_MaxTidalVolume);
      ClrError(&Err_Limit_MinPressure);
      ClrError(&Err_Limit_MinTidalVolume);
      SETexp_time = 0;
      SET_PEEP = Settings->PEEP/10.0;
      timing=0;
      dihanje_state=MODE_STATE_EXP_START;
    //break;  //BRAKE is not needed here.

    case MODE_STATE_EXP_START: // zacetek vdiha, preveri, ce so klesce narazen, sicer jih daj narazen
      Control->mode = CTRL_PAR_MODE_TARGET_POSITION;
      Control->target_position  = 0;
      Control->target_flow = 0;
      FIR(0,1,0);
      Measured_InspTime=timing;
      if (Measured->volume_t > MaxMeasuredVolume) MaxMeasuredVolume = Measured->volume_t;
      peakExpFlow=0;
      timing=0;
      //Report Inspiria Statistics
      MetricsStoreInspTime(Measured_InspTime+Measured_PrampTime);
      MetricsStoreMAP(sumAirwayPressure/(Measured_InspTime+Measured_PrampTime));
      dihanje_state=MODE_STATE_EXP_ZERO_POS_WAIT;
    break;

    case MODE_STATE_EXP_ZERO_POS_WAIT: // cakaj, da so klesce narazen
      timing += TIME_SLICE_MS;
      if (Measured->volume_t > MaxMeasuredVolume) MaxMeasuredVolume = Measured->volume_t;
      if (Measured->flow < peakExpFlow) peakExpFlow = Measured->flow;
      if (Control->mode == CTRL_PAR_MODE_STOP)
      {
        //Report TidalVolume after the motor reached starting position
        MetricsStoreVt(MaxMeasuredVolume);  //Volume might still continue to rise a bit after stopping the motor
        Control->ReportReady |= (1<<CTR_REPRDY_INSP);
        dihanje_state=MODE_STATE_EXP_WAIT;
      }
    break;

    case MODE_STATE_EXP_WAIT: // cakaj na naslednji vdih
      timing += TIME_SLICE_MS;
      if (Measured->flow < peakExpFlow) peakExpFlow = Measured->flow;
      if (timing > (SETexp_time-Measured_PreStartTime))
      {
        Measured_ExpTime=timing;
        //TODO: Report Expiria Statistics Here
        MetricsStoreExpTime(Measured_ExpTime);
        MetricsStorePEF(peakExpFlow);
                //MetricsStoreRCexp();
        MetricsStoreBreathTrigger(HAL_GetTick(), TRIG_TYPE_TIME);
        MetricsNextCycle();
        Control->ReportReady |= (1<<CTR_REPRDY_EXP);
        Measured->volume_mode = VOLUME_RESET;
        dihanje_state=MODE_STATE_INSP_INIT;
      }
    break;

    case MODE_STATE_INSP_INIT: //start inspiratory cycle
      //Reload all settings, if needed change mode, etc.
      if (Settings->new_mode != MODE_CMV)
      {
        Settings->current_mode = Settings->new_mode;
        dihanje_state = MODE_STATE_FIRST_RUN;
        break;
      }

      MINpressure =  Settings->limit_InspPressure_min/10.0;
      MAXpressure = Settings->limit_PeakInspPressure/10.0;
      MINvolume = Settings->limit_tidal_volume_min;
      SETinsp_time = Settings->target_inspiria_time;
      SETexp_time = Settings->target_expiria_time;
      SETpramp_time = Settings->target_Pramp_time;
      SET_PEEP = Settings->PEEP/10.0;
      TargetVolume = Settings->target_tidal_volume;     //

      TargetFlow = (TargetVolume*1.4+0) / SETinsp_time * 60.0;         //  ml/ms * 60 = l/min
      Pramp_rate = TargetFlow/SETpramp_time;
      StartFlow = 0;

      Measured_PreStartTime=0;
      Measured_PrampTime=0;
      Measured_InspTime=0;
      Measured_ExpTime=0;
      MaxMeasuredVolume=0;
      sumAirwayPressure=0;

      //start cycle
      Measured->volume_mode = VOLUME_INTEGRATE;
      Control->BreathCounter++;
      Control->mode=CTRL_PAR_MODE_DUMMY_REGULATE_VOLUME_PID_RESET;
      Control->target_volume = 0;
      Control->target_flow = StartFlow;
      Control->target_power = PREP_CURRENT_START;
      PreStartBoostTime = PREP_T_TOTAL;
      timing=0;
      dihanje_state=MODE_STATE_INSP_PREP_1;
    break;

// Motor pre-ignition  ///////////////////////////////////////////////////
    case MODE_STATE_INSP_PREP_1: //pre-start (get the motor going)
      timing += TIME_SLICE_MS;
      if (timing >= PREP_T_START)
      {
        dihanje_state=MODE_STATE_INSP_PREP_2;
      }
      break;

    case MODE_STATE_INSP_PREP_2: //pre-start (get the motor going)
      timing += TIME_SLICE_MS;
      Control->target_power = PREP_CURRENT_START+(PREP_CURRENT_MAX-PREP_CURRENT_START)*(float)(timing-PREP_T_START)/(float)PREP_T_RAMP_I;
      if (timing >= (PREP_T_START+PREP_T_RAMP_I))
      {
        Control->target_power = PREP_CURRENT_MAX;
        dihanje_state=MODE_STATE_INSP_PREP_3;
      }
      break;

    case MODE_STATE_INSP_PREP_3: //pre-start (get the motor going)
      timing += TIME_SLICE_MS;
      if (timing >= PREP_T_TOTAL)
      {
        Control->mode=CTRL_PAR_MODE_REGULATE_FLOW;
        Control->target_flow = FIR(StartFlow,0,0);
        SETexp_time = SETexp_time - PreStartBoostTime;
        Measured_PreStartTime=timing;
        timing=0;
        dihanje_state=MODE_STATE_INSP_PRAMP;
      }
      //TODO: FIGURE OUT EXACTLY WHAT TO DO WHEN FLOW CAN NOT BE ACHIEVED
      if (Measured->pressure > (MAXpressure - PressureSafetyMargin))
      {
        Control->mode = CTRL_PAR_MODE_REGULATE_PRESSURE;
        Control->target_pressure = FIR(MAXpressure-PressureSafetyMargin,0,0);
        SETexp_time = SETexp_time - (PreStartBoostTime + timing);
        timing = 0;
        //IncError(&Err_Limit_PeakPreasure);
        dihanje_state = MODE_STATE_EXP_START;  //Abort!!!
      }
      break;
//P-ramp  ///////////////////////////////////////////////////
    case MODE_STATE_INSP_PRAMP: //P-ramp
      timing += TIME_SLICE_MS;
      Control->target_flow = FIR(StartFlow + Pramp_rate*timing,0,0);
      sumAirwayPressure += Measured->pressure;
      if (timing >= SETpramp_time)  // gremo v constant pressure
      {
        Control->target_flow = FIR(TargetFlow,0,0);
        Measured_PrampTime=timing;
        timing = 0;
        dihanje_state=MODE_STATE_INSP_CONST_F;
      }
      Control->target_volume += Control->target_flow/60.0;
      break;

//Constant Flow ////////////////////////////////////////
    case MODE_STATE_INSP_CONST_F: //cakaj da mine INHALE_TIME ali da motor pride do konca
      timing += TIME_SLICE_MS;
      Control->target_flow=FIR(TargetFlow,0,0);
      Control->target_volume += Control->target_flow/60.0;
      if (Measured->volume_t > MaxMeasuredVolume) MaxMeasuredVolume = Measured->volume_t;
      sumAirwayPressure += Measured->pressure;
      // ce je prisel do konca, zakljuci cikel vdiha
      if (Measured->volume_t > TargetVolume)
      {
        DecError(&Err_Limit_MinTidalVolume);
        if (Measured->pressure < MINpressure) IncError(&Err_Limit_MinPressure);
        else DecError(&Err_Limit_MinPressure);
        DecError(&Err_Limit_PeakPreasure);
        dihanje_state = MODE_STATE_EXP_START;
      }
      //Alternate condition - max volume reached. Should probably issue a warning
      if (timing > SETinsp_time)
      {
        if (Measured->volume_t < MINvolume) IncError(&Err_Limit_MinTidalVolume);
        else DecError(&Err_Limit_MinTidalVolume);
        if (Measured->pressure < MINpressure) IncError(&Err_Limit_MinPressure);
        else DecError(&Err_Limit_MinPressure);
        DecError(&Err_Limit_PeakPreasure);
        dihanje_state=MODE_STATE_EXP_START;
      }
      if (Measured->pressure > MAXpressure)
      {
        if (Measured->volume_t < MINvolume) IncError(&Err_Limit_MinTidalVolume);
        else DecError(&Err_Limit_MinTidalVolume);
        DecError(&Err_Limit_MinPressure);
        IncError(&Err_Limit_PeakPreasure);
        dihanje_state = MODE_STATE_EXP_START;
      }
      //Errors:
      if (Control->cur_position >= CTRL_PAR_MAX_POSITION) //Came too far - wait in this position until insp
      {
        if (Measured->volume_t < MINvolume) IncError(&Err_Limit_MinTidalVolume);
        else DecError(&Err_Limit_MinTidalVolume);
        if (Measured->pressure < MINpressure) IncError(&Err_Limit_MinPressure);
        else DecError(&Err_Limit_MinPressure);
        DecError(&Err_Limit_PeakPreasure);
        dihanje_state = MODE_STATE_EXP_START;
      }
      break;

//Endings//////////////////////////////
/*    case MODE_STATE_INSP_COMPLETE_MAX_VOL: //(Only in case of Error in previous state) motor je prisel do konca, pocakaj, da mine cas vdiha
      timing += TIME_SLICE_MS;
      Control->mode = CTRL_PAR_MODE_STOP;
      if (timing > SETinsp_time)
      {
        dihanje_state=MODE_STATE_EXP_START;
      }
      break;

    case MODE_STATE_INSP_COMPLETE_MAX_PRESSURE: //(Only in case of Error in previous state) motor je prisel do konca, pocakaj, da mine cas vdiha
      timing += TIME_SLICE_MS;
      Control->mode = CTRL_PAR_MODE_STOP;
      if (timing > SETinsp_time)
      {
        dihanje_state=MODE_STATE_EXP_START;
      }
      break;

    case MODE_STATE_INSP_COMPLETE_MAX_POS: //(Only in case of Error in previous state) motor je prisel do konca, pocakaj, da mine cas vdiha
      timing += TIME_SLICE_MS;
      Control->mode = CTRL_PAR_MODE_STOP;
      if (timing > SETinsp_time)
      {
        dihanje_state=MODE_STATE_EXP_START;
      }
      break;
*/
    default:
      ReportError(ModeCMV_UnknownState,FSH("Error: Unknown state in C_VCV state machine"));
      Control->mode=CTRL_PAR_MODE_STOP;
      dihanje_state = 0;
      break;
  }
}

float FIR(float new_x, int reset, float reset_val)
{
  const float b[]={3.747227e-06, 4.645703e-05, 1.789057e-04, 4.545593e-04, 9.267559e-04,
      1.645741e-03, 2.655701e-03, 3.991955e-03, 5.678439e-03, 7.725642e-03, 1.012909e-02,
      1.286849e-02, 1.590759e-02, 1.919477e-02, 2.266440e-02, 2.623885e-02, 2.983118e-02,
      3.334824e-02, 3.669422e-02, 3.977437e-02, 4.249879e-02, 4.478605e-02, 4.656651e-02,
      4.778525e-02, 4.840431e-02};
  #define FILTER_LENGTH (sizeof(b)/sizeof(b[0])*2)
  static float x[FILTER_LENGTH];
  static int index=0;
  int i;
  float y;

  if (reset)
  {
    for (i= 0; i<FILTER_LENGTH; i++) x[i]=reset_val;
  }

  x[index]=new_x;
  y=0;
  for (i=0; i<FILTER_LENGTH/2; i++)
  {
    y+=b[i]*x[(index+FILTER_LENGTH-i)%FILTER_LENGTH] + b[i]*x[(index+1+i)%FILTER_LENGTH];
  }
  index++;
  if (index >= FILTER_LENGTH) index = 0;

  return y;
}
