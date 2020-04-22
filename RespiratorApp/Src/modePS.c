/*
 * modeCPAP_PS.c
 *
 * Created: 20. 04. 2020 19:04:40
 *  Author: mpirc
 */ 
//Opisi mode, da vemo kaj pocnemo

#include <modePS.h>
#include "Measure.h"
#include "GPIO.h"
#include <stdio.h>

#define MIN_FLOW_TO_START_MONITORING  -10
#define AVERAGING_LENGTH  4

float FIRtargetP(float new_x, int reset, float reset_val);
float FIRflow(float new_x, int reset, float reset_val);
float AVG(float new_x, int reset, float reset_val);

#define MAX_PRAMP_TIME  200

void modePS(RespSettings_t* Settings, MeasuredParams_t* Measured, CtrlParams_t* Control)
{
static  ModeStates_t dihanje_state = MODE_STATE_FIRST_RUN;
static  int16_t timing;
static  uint16_t SETinsp_time;
static  uint16_t SETapnea_time;
static  float SETInspTrig= 1.0;
static  float SETpressure;
static  float SETPrampPressure;
static  float SETPrampStartPressure;
static  float SET_PEEP;
static  float MAXpressure;
static  float MAXvolume;
static  float SET_ETSfactor;
static  uint16_t SETpramp_time;
static  int16_t PreStartBoostTime;
static  float PREP_CURRENT_START = 10;  //25;
static  float PREP_CURRENT_MAX = 20;    //30;

static  float PREP_T_TOTAL = 50;
static  float PREP_T_START = 10;
static  float PREP_T_RAMP_I = 39;//(49-PREP_T_START);
static  float Pramp_rate = 0;
static  float FilteredPressure = 0;

//static  float MAXexpiriaFlow = 0;
static  float MAXinspiriaFlow = 0;
static  float FilteredFlow=0;


	Control->status = dihanje_state;	// shrani stanje dihanja
	
	//State machine starts with exhalation.
	//At the end of exhalation all the settings are reloaded into local copies (state 3?).
	//Local settings are used during the rest of the cycle,
	//so that incompatible settings can not be loaded at the wrong time
	FilteredPressure=AVG(Measured->pressure,0,0);
  FilteredFlow=FIRflow(Measured->flow,0,0);
	if (dihanje_state >= MODE_STATE_INSP_CONST_P)
  {
    if (FilteredFlow > MAXinspiriaFlow) MAXinspiriaFlow = FilteredFlow;
//    printf("%.1f %.1f \r\n",MAXinspiriaFlow, FilteredFlow);
  }

	switch (dihanje_state)
	{
		case MODE_STATE_FIRST_RUN:	//First time: init local settings, etc
      SETInspTrig = Settings->trigger_pressure/10.0;
      SETinsp_time = MAX_PS_INSP_TIME;
      SETapnea_time = Settings->limit_apnea_time_max;
      SETpramp_time = Settings->target_Pramp_time;
      SET_PEEP = Settings->PEEP/10.0;
      SETpressure = Settings->target_pressure/10.0;
      MAXpressure = Settings->limit_PeakInspPressure/10.0;
      MAXvolume = Settings->target_tidal_volume;
      dihanje_state=MODE_STATE_EXP_START;
		break;

		case MODE_STATE_EXP_START: // zacetek vdiha, preveri, ce so klesce narazen, sicer jih daj narazen
      Control->mode = CTRL_PAR_MODE_TARGET_POSITION;
      Control->target_position  = 0;
      Control->target_pressure = SET_PEEP;
      FIRtargetP(SET_PEEP,1,SET_PEEP);
      AVG(SET_PEEP,1,SET_PEEP);
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
      if (Measured->flow > MIN_FLOW_TO_START_MONITORING)
      {
        dihanje_state=MODE_STATE_LOOK_FOR_INSP_TRIGGER;
      }
      //Backup trigger
      if (timing > SETapnea_time)     //backup trigger  reda 20 s
      {
        Measured->volume_mode = VOLUME_RESET;
        dihanje_state=MODE_STATE_INSP_INIT;
      }
		break;
		
		case MODE_STATE_LOOK_FOR_INSP_TRIGGER:
      timing += TIME_SLICE_MS;

      if ( (FilteredPressure - Measured->pressure) > SETInspTrig )
      {
        Measured->volume_mode = VOLUME_RESET;
        dihanje_state=MODE_STATE_INSP_INIT;
      }

      //Backup trigger
      if (timing > SETapnea_time)     //backup trigger  reda 20 s
      {
        Measured->volume_mode = VOLUME_RESET;
        dihanje_state=MODE_STATE_INSP_INIT;
      }
		  break;


		case MODE_STATE_INSP_INIT: //start inspiratory cycle
      //Reload all settings, if needed change mode, etc.
      if (Settings->new_mode != MODE_PS)
      {
        Settings->current_mode = Settings->new_mode;
        dihanje_state = MODE_STATE_FIRST_RUN;
        break;
      }
      SETInspTrig = Settings->trigger_pressure/10.0;
      MAXpressure = Settings->limit_PeakInspPressure/10.0;
      MAXvolume = Settings->limit_tidal_volume_max;
      SETinsp_time = MAX_PS_INSP_TIME;
      SETapnea_time = Settings->limit_apnea_time_max;
      SET_ETSfactor = Settings->ETS/100.0;
      SETpramp_time = Settings->target_Pramp_time;
      SET_PEEP = Settings->PEEP/10.0;
      SETpressure = Settings->target_pressure/10.0;
      SETPrampPressure = SETpressure;
      //SETPrampStartPressure = SET_PEEP+(SETPrampPressure-SET_PEEP)/4;
      Pramp_rate = (SETPrampPressure - SET_PEEP - (SETPrampPressure-SET_PEEP)/4)/ MAX_PRAMP_TIME;
      SETPrampStartPressure = SETPrampPressure - SETpramp_time*Pramp_rate;

      //start cycle
      MAXinspiriaFlow = 0;
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
        Control->mode=CTRL_PAR_MODE_REGULATE_PRESSURE;
        Control->target_pressure = FIRtargetP(SETPrampStartPressure,0,0);
        dihanje_state=MODE_STATE_INSP_PRAMP;
        SETapnea_time = SETapnea_time - PreStartBoostTime;
        timing=0;
        LED3_Off();
        LED4_On();
      }
      if (Measured->pressure > SETpressure)
      {
        Control->mode = CTRL_PAR_MODE_REGULATE_PRESSURE;
        Control->target_pressure = FIRtargetP(SETPrampPressure,0,0);
        //LED2_On();
        SETapnea_time = SETapnea_time - (PreStartBoostTime + timing);
        timing = 0;
        dihanje_state = MODE_STATE_INSP_CONST_P;	//direktno na const. pressure step
        LED3_Off();
        LED4_On();
        LED7_On();
      }
      break;

//P-ramp  ///////////////////////////////////////////////////
		case MODE_STATE_INSP_PRAMP: //P-ramp
      timing += TIME_SLICE_MS;
      Control->target_pressure = FIRtargetP(SETPrampStartPressure + Pramp_rate*timing,0,0);
      if (timing >= SETpramp_time)  // gremo v constant pressure
      {
        Control->target_pressure = FIRtargetP(SETPrampPressure,0,0);
        dihanje_state=MODE_STATE_INSP_CONST_P;
        ReportError(DbgMsg,FSH("PS-ConstPressure..."));
        LED4_Off();
        LED5_On();
      }
      break;

//Constant pressure ////////////////////////////////////////
		case MODE_STATE_INSP_CONST_P: //cakaj da mine INHALE_TIME ali da motor pride do konca
      timing += TIME_SLICE_MS;
      Control->target_pressure=FIRtargetP(SETPrampPressure,0,0);
      // Finish inspiria cycle when ETS threshold is reached

      if (FilteredFlow < MAXinspiriaFlow*SET_ETSfactor)
      {
          ReportError(DbgMsg,FSH("ETS reached"));
          dihanje_state=MODE_STATE_EXP_START;
          Control->mode = CTRL_PAR_MODE_STOP;
      }

      //Alternate condition - max volume reached. Should probably issue a warning
      if (timing > SETinsp_time)
      {
          ReportError(DbgMsg,FSH("Max time reached."));
          dihanje_state=MODE_STATE_EXP_START;
          Control->mode = CTRL_PAR_MODE_STOP;
      }
      if (Measured->volume_t > MAXvolume)
      {
          ReportError(DbgMsg,FSH("Max Volume reached."));
        dihanje_state = MODE_STATE_INSP_COMPLETE_MAX_VOL;
        Control->mode = CTRL_PAR_MODE_STOP;
      }
      if (Measured->pressure > MAXpressure)
      {
          ReportError(DbgMsg,FSH("Max Pressure reached"));
        dihanje_state = MODE_STATE_INSP_COMPLETE_MAX_PRESSURE;
        Control->mode = CTRL_PAR_MODE_STOP;
      }
      //Errors:
      if (Control->cur_position >= CTRL_PAR_MAX_POSITION)	//Came too far - wait in this position until insp
      {
          ReportError(DbgMsg,FSH("Max Position reached"));
        dihanje_state = MODE_STATE_INSP_COMPLETE_MAX_POS;
        Control->mode = CTRL_PAR_MODE_STOP;
      }
      break;

    case MODE_STATE_INSP_COMPLETE_ETS_TRIG: //(Only in case of Error in previous state) motor je prisel do konca, pocakaj, da mine cas vdiha
      timing += TIME_SLICE_MS;
      dihanje_state=MODE_STATE_EXP_START;
      break;

    case MODE_STATE_INSP_COMPLETE_MAX_VOL: //(Only in case of Error in previous state) motor je prisel do konca, pocakaj, da mine cas vdiha
      timing += TIME_SLICE_MS;
      dihanje_state=MODE_STATE_EXP_START;
      break;

		case MODE_STATE_INSP_COMPLETE_MAX_PRESSURE: //(Only in case of Error in previous state) motor je prisel do konca, pocakaj, da mine cas vdiha
      timing += TIME_SLICE_MS;
      dihanje_state=MODE_STATE_EXP_START;
      break;

		case MODE_STATE_INSP_COMPLETE_MAX_POS: //(Only in case of Error in previous state) motor je prisel do konca, pocakaj, da mine cas vdiha
      timing += TIME_SLICE_MS;
      dihanje_state=MODE_STATE_EXP_START;
      break;
		
		default:
      ReportError(ModePS_UnknownState,FSH("Error: Unknown state in C_VCV state machine"));
      Control->mode=CTRL_PAR_MODE_STOP;
      dihanje_state = 0;
      break;
	}
}


float FIRflow(float new_x, int reset, float reset_val)
{
  const float b[]={-3.798186e-07, -2.301454e-06, -6.569785e-06, -1.434567e-05, -2.700543e-05,
       -4.609798e-05, -7.327336e-05, -1.101802e-04, -1.583314e-04, -2.189389e-04, -2.927214e-04,
       -3.796899e-04, -4.789205e-04, -5.883232e-04, -7.044202e-04, -8.221479e-04, -9.346964e-04,
       -1.033401e-03, -1.107706e-03, -1.145198e-03, -1.131748e-03, -1.051730e-03, -8.883608e-04,
       -6.241288e-04, -2.413173e-04, 2.773873e-04, 9.482246e-04, 1.785647e-03, 2.801586e-03,
       4.004740e-03, 5.399891e-03, 6.987314e-03, 8.762279e-03, 1.071470e-02, 1.282893e-02,
       1.508377e-02, 1.745262e-02, 1.990387e-02, 2.240149e-02, 2.490575e-02, 2.737423e-02,
       2.976279e-02, 3.202681e-02, 3.412242e-02, 3.600775e-02, 3.764417e-02, 3.899745e-02,
       4.003883e-02, 4.074592e-02, 4.110336e-02}; //100 taps (N=99)

#define FILTER_LENGTH1 (sizeof(b)/sizeof(b[0])*2)

  static float x[FILTER_LENGTH1];
  static int index=0;
  int i;
  float y;

  if (reset)
  {
    for (i= 0; i<FILTER_LENGTH1; i++) x[i]=reset_val;
  }

  x[index]=new_x;
  y=0;
  for (i=0; i<FILTER_LENGTH1/2; i++)
  {
    y+=b[i]*x[(index+FILTER_LENGTH1-i)%FILTER_LENGTH1] + b[i]*x[(index+1+i)%FILTER_LENGTH1];
  }
  index++;
  if (index >= FILTER_LENGTH1) index = 0;

  return y;
}

float FIRtargetP(float new_x, int reset, float reset_val) //N = 49 (50 taps)
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

float AVG(float new_x, int reset, float reset_val)
{
  static float x[AVERAGING_LENGTH];
  static int index=0;
  static int suma;
  int i;

  if (reset)
  {
    for (i= 0; i<AVERAGING_LENGTH; i++) x[i]=reset_val;
  }

  suma+=new_x;
  suma-=x[index];
  x[index]=new_x;

  index++;
  if (index >= AVERAGING_LENGTH) index = 0;

  return suma/AVERAGING_LENGTH;
}
