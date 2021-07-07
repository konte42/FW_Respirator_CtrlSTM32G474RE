/*
 * CFile1.c
 *
 * Created: 2. 04. 2020 09:27:40
 *  Author: mjankovec
 */ 
//Opisi mode, da vemo kaj pocnemo

#include <modePCV.h>
#include "Measure.h"
#include "GPIO.h"

float fFIR50(float new_x, int reset, float reset_val);

#define MAX_PRAMP_TIME  200

void modePCV(RespSettings_t* Settings, MeasuredParams_t* Measured, CtrlParams_t* Control)
{

  static  ModeStates_t dihanje_state = MODE_STATE_FIRST_RUN;
  static  int16_t timing;
  static  uint16_t SETinsp_time;
  static  uint16_t SETexp_time;
  static  float SETpressure;
  static  float SETPrampPressure;
  static  float SETPrampStartPressure;
  static  float SET_PEEP;
  static  float MINpressure;
  static  float MAXpressure;
  static  float MINvolume;
  static  float MAXvolume;
  static  uint16_t SETpramp_time;
  static  int16_t PreStartBoostTime;
  static  float PREP_CURRENT_START =25; //25;
  static  float PREP_CURRENT_MAX =30; //30;

  static  float PREP_T_TOTAL = 50;
  static  float PREP_T_START = 10;
  static  float PREP_T_RAMP_I = 39;//(49-PREP_T_START);
  static  float PRESSURE_INCREMENT = 0.001; // cmH2O
  static  float Pramp_rate = 0;
  static uint16_t Measured_ExpTime=0;
  static uint16_t Measured_InspTime=0;
  static uint16_t Measured_PrampTime=0;
  static uint16_t Measured_PreStartTime=0;
  static float MaxMeasuredVolume;
  static float sumAirwayPressure;
  static float peakExpFlow;

	Control->status = dihanje_state;	// shrani stanje dihanja
	
	//State machine starts with exhalation.
	//At the end of exhalation all the settings are reloaded into local copies (state 3?).
	//Local settings are used during the rest of the cycle,
	//so that incompatible settings can not be loaded at the wrong time

	switch (dihanje_state)
	{
		case MODE_STATE_FIRST_RUN:	//First time: init local settings, etc
      ClrError(&Err_Limit_PeakPreasure);
      ClrError(&Err_Limit_MaxTidalVolume);
      ClrError(&Err_Limit_MinPressure);
      ClrError(&Err_Limit_MinTidalVolume);
      SET_PEEP = Settings->PEEP/10.0;
      SETexp_time = 0;  //First time skip to insp_init
      timing=0;
      dihanje_state=MODE_STATE_EXP_START;
		//break;  //BRAKE is not needed here.

		case MODE_STATE_EXP_START: // zacetek vdiha, preveri, ce so klesce narazen, sicer jih daj narazen
      Control->mode = CTRL_PAR_MODE_TARGET_POSITION;
      Control->target_position  = 0;
      Control->target_pressure = SET_PEEP;
      fFIR50(SET_PEEP,1,SET_PEEP);
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
      if (Settings->new_mode != MODE_PCV)
      {
        Settings->current_mode = Settings->new_mode;
        dihanje_state = MODE_STATE_FIRST_RUN;
        break;
      }
      MINpressure =  Settings->limit_InspPressure_min/10.0;
      MAXpressure = Settings->limit_PeakInspPressure/10.0;
      MINvolume = Settings->limit_tidal_volume_min;
      MAXvolume = Settings->limit_tidal_volume_max;
      SETinsp_time = Settings->target_inspiria_time;
      SETexp_time = Settings->target_expiria_time;
      SETpramp_time = Settings->target_Pramp_time;
      SET_PEEP = Settings->PEEP/10.0;
      SETpressure = Settings->target_pressure/10.0;
      SETPrampPressure = SETpressure - SETinsp_time*PRESSURE_INCREMENT;
      //SETPrampStartPressure = SET_PEEP+(SETPrampPressure-SET_PEEP)/4;
      Pramp_rate = (SETPrampPressure - SET_PEEP - (SETPrampPressure-SET_PEEP)/4)/ MAX_PRAMP_TIME;
      SETPrampStartPressure = SETPrampPressure - SETpramp_time*Pramp_rate;

      Measured_PreStartTime=0;
      Measured_PrampTime=0;
      Measured_InspTime=0;
      Measured_ExpTime=0;
      MaxMeasuredVolume=0;
      sumAirwayPressure=0;

      //start cycle
      Measured->volume_mode = VOLUME_INTEGRATE;
      Control->BreathCounter++;
      Control->mode=CTRL_PAR_MODE_DUMMY_REGULATE_PRESSURE_PID_RESET;
      Control->target_power = PREP_CURRENT_START;  //20%
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
        Control->mode=CTRL_PAR_MODE_REGULATE_PRESSURE;
        Control->target_pressure = fFIR50(SETPrampStartPressure,0,0);
        SETexp_time = SETexp_time - PreStartBoostTime;
        Measured_PreStartTime=timing;
        timing=0;
        dihanje_state=MODE_STATE_INSP_PRAMP;
      }
      if (Measured->pressure > SETpressure)
      {
        Control->mode = CTRL_PAR_MODE_REGULATE_PRESSURE;
        Control->target_pressure = fFIR50(SETPrampPressure,0,0);
        SETexp_time = SETexp_time - (PreStartBoostTime + timing);
        Measured_PrampTime=0;
        Measured_PreStartTime=timing;
        timing = 0;
        dihanje_state = MODE_STATE_INSP_CONST_P;	//direktno na const. pressure step
      }
      break;
//P-ramp  ///////////////////////////////////////////////////

		case MODE_STATE_INSP_PRAMP: //P-ramp
      timing += TIME_SLICE_MS;
      Control->target_pressure = fFIR50(SETPrampStartPressure + Pramp_rate*timing,0,0);
      sumAirwayPressure += Measured->pressure;
      if (timing >= SETpramp_time)  // gremo v constant pressure
      {
        Control->target_pressure = fFIR50(SETPrampPressure,0,0);
        Measured_PrampTime=timing;
        timing = 0;
        dihanje_state=MODE_STATE_INSP_CONST_P;
      }
      break;

//Constant pressure ////////////////////////////////////////
		case MODE_STATE_INSP_CONST_P: //cakaj da mine INHALE_TIME ali da motor pride do konca
      timing += TIME_SLICE_MS;
      Control->target_pressure=fFIR50(SETPrampPressure+ (timing-SETpramp_time)*PRESSURE_INCREMENT,0,0);
      if (Measured->volume_t > MaxMeasuredVolume) MaxMeasuredVolume = Measured->volume_t;
      sumAirwayPressure += Measured->pressure;
      //Cycle END condition
      if (timing > (SETinsp_time-Measured_PrampTime))
      {
        ReportError(InfoCycleEND_InspTime,FSH("Max TIME reached. "));
        DecError(&Err_Limit_PeakPreasure);
        DecError(&Err_Limit_MaxTidalVolume);
        if (Measured->pressure < MINpressure) IncError(&Err_Limit_MinPressure);
        else DecError(&Err_Limit_MinPressure);
        if (Measured->volume_t < MINvolume) IncError(&Err_Limit_MinTidalVolume);
        else DecError(&Err_Limit_MinTidalVolume);
        dihanje_state=MODE_STATE_EXP_START;
      }
      //Alternate Cycle END conditions
      if (Measured->volume_t > MAXvolume)
      {
        IncError(&Err_Limit_MaxTidalVolume);
        DecError(&Err_Limit_MinTidalVolume);
        DecError(&Err_Limit_PeakPreasure);
        if (Measured->pressure < MINpressure) IncError(&Err_Limit_MinPressure);
        else DecError(&Err_Limit_MinPressure);
        dihanje_state=MODE_STATE_EXP_START;
      }
      if (Measured->pressure > MAXpressure)
      {
        DecError(&Err_Limit_MaxTidalVolume);
        IncError(&Err_Limit_PeakPreasure);
        DecError(&Err_Limit_MinPressure);
        if (Measured->volume_t < MINvolume) IncError(&Err_Limit_MinTidalVolume);
        else DecError(&Err_Limit_MinTidalVolume);
        dihanje_state=MODE_STATE_EXP_START;
      }
      if (Control->cur_position >= CTRL_PAR_MAX_POSITION)	//Came too far - wait in this position until insp
      {
        ReportError(Info_Limits_MaxPosition,FSH("Max Motor Position"));
        DecError(&Err_Limit_MaxTidalVolume);
        DecError(&Err_Limit_PeakPreasure);
        if (Measured->pressure < MINpressure) IncError(&Err_Limit_MinPressure);
        else DecError(&Err_Limit_MinPressure);
        if (Measured->volume_t < MINvolume) IncError(&Err_Limit_MinTidalVolume);
        else DecError(&Err_Limit_MinTidalVolume);
        dihanje_state = MODE_STATE_EXP_START;
      }
      break;
		
		default:
      ReportError(ModePCV_UnknownState,FSH("Error: Unknown state in PCV state machine"));
      Control->mode=CTRL_PAR_MODE_STOP;
      dihanje_state = 0;
      break;
	}

}

float fFIR50(float new_x, int reset, float reset_val)
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
