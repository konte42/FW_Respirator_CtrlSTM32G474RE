/*
 * modeHWtest.c
 *
 * Created: 7. 04. 2020 08:36:55
 *  Author: maticpi
 */ 
#include "modeHWtest.h"
#include "GPIO.h"
#include "Measure.h"

void modeHWtest(RespSettings_t* Settings, MeasuredParams_t* Measured, CtrlParams_t* Control)
{
	static ModeStates_t MODE_STATE = MODE_STATE_FIRST_RUN;
	static int16_t timing;
	
	#define PRAMP_OFFSET	5	//cmH2O
	
	static uint16_t SETinsp_time;
	static uint16_t SETexp_time;
	
	Control->status = MODE_STATE;	// shrani stanje dihanja
	
	//State machine starts with exhalation. 
	//At the end of exhalation all the settings are reloaded into local copies (state 3?).
	//Local settings are used during the rest of the cycle, 
	//so that incompatible settings can not be loaded at the wrong time

	switch (MODE_STATE)
	{
		case MODE_STATE_FIRST_RUN:	//First time: init local settings, etc
			SETinsp_time = Settings->target_inspiria_time;
			SETexp_time = Settings->target_expiria_time;
			MODE_STATE=MODE_STATE_EXP_START;
			break;
			
		case MODE_STATE_EXP_START: // zacetek vdiha, preveri, ce so klesce narazen, sicer jih daj narazen
			Control->mode = CTRL_PAR_MODE_TARGET_POSITION;
			Control->target_position  = 0;
			//LED1_On();
			timing=0;
			MODE_STATE=MODE_STATE_EXP_ZERO_POS_WAIT;
			//LED2_Off();
			//LED3_Off();
			//LED4_Off();
		break;
		
		case MODE_STATE_EXP_ZERO_POS_WAIT: // cakaj, da so klesce narazen
			timing += TIME_SLICE_MS;
			if (Control->mode == CTRL_PAR_MODE_STOP)
			{
				//reload settings, pid parameters, reset stuff,...
				MODE_STATE=MODE_STATE_EXP_WAIT;
			}
		break;

		case MODE_STATE_EXP_WAIT: // cakaj na naslednji vdih
			timing += TIME_SLICE_MS;
			if (timing > SETexp_time)
			{
				MODE_STATE=MODE_STATE_INSP_INIT;
			}
		break;
		
		case MODE_STATE_INSP_INIT: //start inspiratory cycle
			//Reload all settings, if needed change mode, etc.
			if (Settings->new_mode != MODE_HW_TEST)
			{
				Settings->current_mode = Settings->new_mode;
				MODE_STATE = MODE_STATE_FIRST_RUN;
				break;
			}
			SETinsp_time = Settings->target_inspiria_time;
			SETexp_time = Settings->target_expiria_time;
			//PID values reload every time control mode changes
			
			//start cycle
			Measured->volume_mode = VOLUME_RESET;
			Control->BreathCounter++;
			Control->target_speed = 100;
			Control->mode=CTRL_PAR_MODE_TARGET_SPEED;
			MODE_STATE=MODE_STATE_INSP_CONST_P;
			timing = 0;
			
		break;
				
		case MODE_STATE_INSP_CONST_P: //cakaj da mine INHALE_TIME ali da motor pride do konca
			
			timing += TIME_SLICE_MS;
			// ce je prisel do konca, zakljuci cikel vdiha
			if (timing > SETinsp_time)
			{
				Control->mode=CTRL_PAR_MODE_STOP;
				MODE_STATE=MODE_STATE_EXP_START;
			}
			//LED1_Off();
			//Errors:
			if (Control->cur_position >= CTRL_PAR_MAX_POSITION)	//Came too far - wait in this position until insp
			{
				Control->mode=CTRL_PAR_MODE_STOP;
				MODE_STATE = MODE_STATE_INSP_COMPLETE_MAX_POS;
			}
		break;

		case MODE_STATE_INSP_COMPLETE_MAX_POS: //motor je prisel do konca, pocakaj, da mine cas vdiha
			timing += TIME_SLICE_MS;
			if (timing > SETinsp_time)
			{
				Control->mode=CTRL_PAR_MODE_STOP;
				MODE_STATE=MODE_STATE_EXP_START;
			}
		break;
		
		default:
		//ReportError(ModeC_VCV_UnknownState,NULL/*"Error: Unknown state in C_VCV state machine"*/);
		Control->mode=CTRL_PAR_MODE_STOP;
		MODE_STATE = 0;
		break;
	}

}
