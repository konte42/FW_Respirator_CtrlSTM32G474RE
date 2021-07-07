/*
 * modeHWtest.c
 *
 * Created: 7. 04. 2020 08:36:55
 *  Author: maticpi
 */ 
#include "modeHWtest.h"
#include "GPIO.h"
#include "Measure.h"

//for PID HW motor testing. Makes one inspiria cycle with maximal motor speed, then goes into stop mode
void modeHWtest(RespSettings_t* Settings, MeasuredParams_t* Measured, CtrlParams_t* Control)
{
	static ModeStates_t MODE_STATE = MODE_STATE_FIRST_RUN;
	static int16_t timing;
	
	static uint16_t SETinsp_time;
	static uint8_t stop;	//indicates stop, to do inspiria only once
	
	Control->status = MODE_STATE;	// shrani stanje dihanja
	
	//State machine starts with exhalation. 
	//At the end of exhalation all the settings are reloaded into local copies (state 3?).
	//Local settings are used during the rest of the cycle, 
	//so that incompatible settings can not be loaded at the wrong time

	switch (MODE_STATE)
	{
		case MODE_STATE_FIRST_RUN:	//First time: init local settings, etc
			SETinsp_time = Settings->target_inspiria_time;
			MODE_STATE=MODE_STATE_EXP_START;
			stop = 0;	// da naredi en dih
			break;
			
		case MODE_STATE_EXP_START: // zacetek vdiha, preveri, ce so klesce narazen, sicer jih daj narazen
			if (Control->cur_position > 5)	// ce je izven zacetne pozicije, pojdi na zacetno pozicijo
			{
				Control->mode = CTRL_PAR_MODE_TARGET_POSITION;
				Control->target_position  = 0;
			}
			MODE_STATE=MODE_STATE_EXP_ZERO_POS_WAIT;
			timing=0;
		break;
		
		case MODE_STATE_EXP_ZERO_POS_WAIT: // cakaj, da so klesce narazen
			timing += TIME_SLICE_MS;

			//Reload all settings, if needed change mode, etc.
			if (Settings->new_mode != MODE_HW_TEST)
			{
				Settings->current_mode = Settings->new_mode;
				MODE_STATE = MODE_STATE_FIRST_RUN;
				break;
			}

			if (Control->mode == CTRL_PAR_MODE_STOP)
			{
				//reload settings, pid parameters, reset stuff,...
				if (stop)
				{
					Control->mode=CTRL_PAR_MODE_STOP;
					Settings->new_mode = MODE_STOP;
				}
				else
				{
					MODE_STATE=MODE_STATE_INSP_INIT;
					Measured->volume_mode = VOLUME_RESET;
				}
			}
		break;
		
		case MODE_STATE_INSP_INIT: //start inspiratory test cycle

			//Reload all settings, if needed change mode, etc.
			if (Settings->new_mode != MODE_HW_TEST)
			{
				Settings->current_mode = Settings->new_mode;
				MODE_STATE = MODE_STATE_FIRST_RUN;
				break;
			}
			SETinsp_time = Settings->target_inspiria_time;
			
			//start cycle
			Measured->volume_mode = VOLUME_INTEGRATE;
			Control->BreathCounter++;
			Control->target_power = Settings->target_motor_power;
			Control->mode=CTRL_PAR_MODE_TARGET_POWER;
			MODE_STATE=MODE_STATE_INSP_CONST_P;
			timing = 0;
		break;
				
		case MODE_STATE_INSP_CONST_P: //cakaj da mine INHALE_TIME ali da motor pride do konca
			
			timing += TIME_SLICE_MS;

			// ce je prisel do konca, zakljuci cikel vdiha in pojdi v stop mode
			if (timing > SETinsp_time || Control->cur_position >= CTRL_PAR_MAX_POSITION)
			{
				Control->mode=CTRL_PAR_MODE_STOP;
				MODE_STATE=MODE_STATE_EXP_START;
				stop = 1;
			}
		break;

		default:
		//ReportError(ModeC_VCV_UnknownState,NULL/*"Error: Unknown state in C_VCV state machine"*/);
		Control->mode=CTRL_PAR_MODE_STOP;
		MODE_STATE = 0;
		break;
	}

}
