/*
 * CFile1.c
 *
 * Created: 2. 04. 2020 09:27:40
 *  Author: mjankovec
 */ 
//Opisi mode, da vemo kaj pocnemo

//Pri lovljenju ustreznega volumna ima prednost �AS (Target inspiratory time)
#include <modeCMV.h>
#include "Measure.h"
#include "GPIO.h"

float FIR(float new_x, int reset, float reset_val);


#define MAX_PRAMP_TIME  200

void modeCMV(RespSettings_t* Settings, MeasuredParams_t* Measured, CtrlParams_t* Control)
{

  static  ModeStates_t dihanje_state = MODE_STATE_FIRST_RUN;
  static  int16_t timing;
  static  uint16_t SETinsp_time;
  static  uint16_t SETexp_time;
  static  uint16_t SETpramp_time;
  static  int16_t PreStartBoostTime;
  static  float PREP_CURRENT_START = 25;
  static  float PREP_CURRENT_MAX = 30;
  static  float PREP_T_TOTAL = 50;
  static  float PREP_T_START = 10;
  static  float PREP_T_RAMP_I = 39;//(49-PREP_T_START);

  static  float SET_PEEP;
  static  float MAXpressure;
  static  float PressureSafetyMargin = 5;

  static  float TargetVolume = 0;
  static  float TargetFlow = 0;
  static  float StartFlow = 0;
  static  float Pramp_rate = 0;
  static  float InertiaVolume = 0;

  Control->status = dihanje_state;  // shrani stanje dihanja

  //State machine starts with exhalation.
  //At the end of exhalation all the settings are reloaded into local copies.
  //Local settings are used during the rest of the cycle,
  //so that incompatible settings can not be loaded at the wrong time

  switch (dihanje_state)
  {
    case MODE_STATE_FIRST_RUN:  //First time: init local settings, etc
      SETinsp_time = Settings->target_inspiria_time;
      SETexp_time = Settings->target_expiria_time;
      SETpramp_time = Settings->target_Pramp_time;
      SET_PEEP = Settings->PEEP/10.0;
      MAXpressure = Settings->PeakInspPressure/10.0;
      TargetVolume = Settings->target_tidal_volume;
      dihanje_state=MODE_STATE_EXP_START;
    break;

    case MODE_STATE_EXP_START: // zacetek vdiha, preveri, ce so klesce narazen, sicer jih daj narazen
      Control->mode = CTRL_PAR_MODE_TARGET_POSITION;
      Control->target_position  = 0;
      Control->target_pressure = SET_PEEP;
      FIR(0,1,0);
      timing=0;
      dihanje_state=MODE_STATE_EXP_ZERO_POS_WAIT;
      LED1_Off();
      LED2_Off();
      LED3_Off();
      LED4_Off();
      LED5_Off();
      LED6_Off();
      LED7_Off();
    break;

    case MODE_STATE_EXP_ZERO_POS_WAIT: // cakaj, da so klesce narazen
      timing += TIME_SLICE_MS;
      if (Control->mode == CTRL_PAR_MODE_STOP)
      {
        dihanje_state=MODE_STATE_EXP_WAIT;
      }
    break;

    case MODE_STATE_EXP_WAIT: // cakaj na naslednji vdih
      timing += TIME_SLICE_MS;
      if (timing > SETexp_time)
      {
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

      SETinsp_time = Settings->target_inspiria_time;
      SETexp_time = Settings->target_expiria_time;
      SETpramp_time = Settings->target_Pramp_time;
      SET_PEEP = Settings->PEEP/10.0;
      MAXpressure = Settings->PeakInspPressure/10.0;
      TargetVolume = Settings->target_tidal_volume;     //

      TargetFlow = TargetVolume / SETinsp_time * 60.0;         //  ml/ms * 60 = l/min
      Pramp_rate = (TargetFlow * 0.75 )/ MAX_PRAMP_TIME;
      StartFlow = TargetFlow - SETpramp_time*Pramp_rate;;
      //InertiaVolume = 0;

      //start cycle
      Measured->volume_mode = VOLUME_INTEGRATE;
      Control->BreathCounter++;
      Control->mode=CTRL_PAR_MODE_DUMMY_REGULATE_PRESSURE_PID_RESET;
      Control->target_speed = PREP_CURRENT_START;  //20%
      PreStartBoostTime = PREP_T_TOTAL;
//      PreStartBoostTime = 23 - SETpramp_time/20;
      timing=0;
      LED1_On();
      dihanje_state=MODE_STATE_INSP_PREP_1;
    break;

// Motor pre-ignition  ///////////////////////////////////////////////////
    case MODE_STATE_INSP_PREP_1: //pre-start (get the motor going)
      timing += TIME_SLICE_MS;
      if (timing >= PREP_T_START)
      {
        dihanje_state=MODE_STATE_INSP_PREP_2;
        LED1_Off();
        LED2_On();
      }
      break;

    case MODE_STATE_INSP_PREP_2: //pre-start (get the motor going)
      timing += TIME_SLICE_MS;
      Control->target_speed = PREP_CURRENT_START+(PREP_CURRENT_MAX-PREP_CURRENT_START)*(float)(timing-PREP_T_START)/(float)PREP_T_RAMP_I;
      if (timing >= (PREP_T_START+PREP_T_RAMP_I))
      {
        Control->target_speed = PREP_CURRENT_MAX;
        dihanje_state=MODE_STATE_INSP_PREP_3;
        LED2_Off();
        LED3_On();

      }
      break;

    case MODE_STATE_INSP_PREP_3: //pre-start (get the motor going)
      timing += TIME_SLICE_MS;
      if (timing >= PREP_T_TOTAL)
      {
        Control->mode=CTRL_PAR_MODE_REGULATE_FLOW;
        Control->target_flow = FIR(StartFlow,0,0);
        dihanje_state=MODE_STATE_INSP_PRAMP;
        SETexp_time = SETexp_time - PreStartBoostTime;
        timing=0;
        LED3_Off();
        LED4_On();
      }
      //TODO: FIGURE OUT EXACTLY WHAT OT DO WHEN FLOW CAN NOT BE ACHIEVED
      if (Measured->pressure > (MAXpressure - PressureSafetyMargin))
      {
        Control->mode = CTRL_PAR_MODE_REGULATE_PRESSURE;
        Control->target_pressure = FIR(MAXpressure-PressureSafetyMargin,0,0);
        SETexp_time = SETexp_time - (PreStartBoostTime + timing);
        timing = 0;
        dihanje_state = MODE_STATE_INSP_CONST_P;  //direktno na const. pressure step
        LED3_Off();
        LED4_On();
        LED7_On();
      }
      break;
//P-ramp  ///////////////////////////////////////////////////

    case MODE_STATE_INSP_PRAMP: //P-ramp
    timing += TIME_SLICE_MS;
    Control->target_flow = FIR(StartFlow + Pramp_rate*timing,0,0);
    if (timing >= SETpramp_time)  // gremo v constant pressure
    {
      Control->target_flow = FIR(TargetFlow,0,0);
      dihanje_state=MODE_STATE_INSP_CONST_P;
    }
    break;

//Constant Flow ////////////////////////////////////////
    case MODE_STATE_INSP_CONST_P: //cakaj da mine INHALE_TIME ali da motor pride do konca
      timing += TIME_SLICE_MS;
      Control->target_flow=FIR(TargetFlow,0,0);
      // ce je prisel do konca, zakljuci cikel vdiha
      if (Measured->volume_t > (TargetVolume-InertiaVolume) )
      {
        dihanje_state = MODE_STATE_INSP_COMPLETE_MAX_VOL;
      }
      //Alternate condition - max volume reached. Should probably issue a warning
      if (timing > SETinsp_time)
      {
        dihanje_state=MODE_STATE_EXP_START;
        LED5_Off();
        LED6_On();
      }
      if (Measured->pressure > MAXpressure)
      {
        dihanje_state = MODE_STATE_INSP_COMPLETE_MAX_PRESSURE;
      }
      //Errors:
      if (Control->cur_position >= CTRL_PAR_MAX_POSITION) //Came too far - wait in this position until insp
      {
        dihanje_state = MODE_STATE_INSP_COMPLETE_MAX_POS;
      }
      break;

    case MODE_STATE_INSP_COMPLETE_MAX_VOL: //(Only in case of Error in previous state) motor je prisel do konca, pocakaj, da mine cas vdiha
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
