/*
 * CommProtocol.c
 *
 * Created: 3. 04. 2020 17:50:54
 *  Author: maticpi
 */ 
#include "CommProtocol.h"
#include <stdio.h>

#define ValidateAndApplyReceivedValue(typ, value, setting, min, max, err) \
case typ: if ((value >= min ) && (value <= max ) ) {setting = value;} else {ReportError(err, FSH(""));} break
/*
typedef enum
{
	eT_INT8,
	eT_UINT8,
	eT_INT16,
	eT_UINT16,
	eT_INT32,
	eT_UINT32
} eParamType_t;

void ValidateAndApplyReceivedValue(int32_t value, void* setting, eParamType_t type, int32_t min, int32_t max, ErrCodes_t, err)
{
	if ( (value >= min) && (value <=  max) )
	{
		switch (type)
		{
			case eT_INT8:   *(int8_t*)  setting = value;
			case eT_UINT8:  *(uint8_t*) setting = value;
			case eT_INT16:  *(int16_t*) setting = value;
			case eT_UINT16: *(uint16_t*)setting = value;
			case eT_INT32:  *(int32_t*) setting = value;
			case eT_UINT32: *(uint32_t*)setting = value;
		}
	}
	else 
	{
		ReportError(Err, NULL);
	}
}*/

int PrepareStatusMessage(uint32_t timestamp, int16_t Flow, int16_t Pressure, int16_t Volume, int16_t MotorPosition, \
int16_t MotorCurrent, int16_t MotorDutyCycle, uint16_t BreathCounter, uint8_t Status, ErrCodes_t Error, float target_value, char *p_msg)
{
	//STX+N+TIMESTAMP+4xADC+ETX
	*p_msg = 0x55;
	p_msg++;
				
	*p_msg = MSG_CORE_LENGTH;
	p_msg++;
				
	*(uint32_t *)p_msg = timestamp;
	p_msg +=4;
				
	*(int16_t *)p_msg = Flow;
	p_msg +=2;
				
	*(int16_t *)p_msg = Pressure;
	p_msg +=2;
				
	*(int16_t *)p_msg = Volume;
	p_msg +=2;

	*(int16_t *)p_msg = MotorPosition;
	p_msg +=2;

	*(int16_t *)p_msg = MotorCurrent;
	p_msg +=2;

	*(int16_t *)p_msg = MotorDutyCycle;
	p_msg +=2;
	
	*(uint16_t *)p_msg = BreathCounter;
	p_msg +=2;
	
	*(uint8_t *)p_msg = Status;
	p_msg +=1;					

	*(uint16_t *)p_msg = Error;
	p_msg +=2;
	
	*(float *)p_msg = target_value;
  p_msg +=4;

	*(p_msg) = 0xAA;

	return (1+1+MSG_CORE_LENGTH+1);
}

//insp time, MAP, Vt
int PrepareMetricsMessage(char *p_msg, int MAX_LENGTH, BreathCycleMetrics_t* p_stat)
{
  //STX+N+data+ETX
  int length = sizeof(BreathCycleMetrics_t);
  if (MAX_LENGTH >= length+3)
  {
    *p_msg = 0x57;
    p_msg++;

    *p_msg = length;
    p_msg++;

    memcpy(p_msg,p_stat,length);

    p_msg+=length;
    *p_msg = 0xAA;
  }
  else
  {
    length = -3;
  }

  return length+3;
}


int PrepareStatisticsMessage(char *p_msg, int MAX_LENGTH, MetricsStatistics_t* p_stat)
{
  //STX+N+data+ETX
  int length = sizeof(MetricsStatistics_t);
  if (MAX_LENGTH >= length+3)
  {
    *p_msg = 0x58;
    p_msg++;

    *p_msg = length;
    p_msg++;

    memcpy(p_msg,p_stat,length);

    p_msg+=length;
    *p_msg = 0xAA;
  }
  else
  {
    length = -3;
  }

  return length+3;
}

int ReportAllCurrentSettings(char *p_msg, int MAX_LENGTH, RespSettings_t *Settings)
{
//  int length = sizeof(RespSettings_t);
  int length = 36;
  if (MAX_LENGTH >= length)
	{
		*p_msg = 0x56;
		p_msg++;
	
		*p_msg = length;
		p_msg++;

		memcpy(p_msg,Settings,length);
		
		p_msg+=length;
		*p_msg = 0xAA;
	}
	else
	{
		length = -3;
	}

	return length+3;
}

//Protocol definition
//STX PARAM space VALUE ETX
//STX = '>'
//ETX = '\n'
//PARAM:
//	'M' = mode
//	'R' = rampup
//  'I' = inspiria time (ms)
//	'E' = expiria time (ms)
//  'P' = PEEP
//  'T' = PeakInspiratory Pressure
//  't' = Min Pressure ????
//  'S' = Target Pressure
//	'V' = volume (ml)
//  'G' = Inhale Trigger pressure (mmH2O)
//  'H' = ETS (%)
//  'A' = Apnea time limit
//  'd' = Min Vt
//  'D' = Max Vt
//  'c' = Min minute Volume
//  'C' = Max minute Volume
//  'b' = Min breath rate
//  'B' = Max breath rate
//  '' =
//Example:
//>M V\n

//TODO: target pressure, 
//	inhale trigger 
//		-flow
//		-negative pressure

void ProcessMessages(char data, RespSettings_t* Settings, ProcMsgState_t* pstate, uint8_t* newdata)
{
	switch (pstate->state)
	{
		case 0:	{//Waiting for STX
			if (data == '>') pstate->state++;
			break;
		}
		case 1: {//Waiting for PARAM
		  pstate->value=0;
			switch (data){
        case 's': //get status
        case 'q': //quiet! - mute buzzer for x seconds
        case 'M':
				case 'R':
				case 'I':
				case 'E':
				case 'P':
        case 'T':
        case 't':
				case 'S':
        case 'V':
        case 'G':
				case 'H':
        case 'A':
        case 'd':
        case 'D':
        case 'c':
        case 'C':
        case 'b':
        case 'B':
				case '1':
				case '2':
				case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
				{
				  pstate->param = data;
				  pstate->state++;
					break;
				}
				default:{
					ReportError(ComRxUnknownParameter,FSH("Received unknown parameter"));
					pstate->param=0;
					pstate->state=0;
					break;
				}
			}
			break;
		}
		case 2: {//Waiting for space
			if (data == ' ') pstate->state++;
			else{
				ReportError(ComRxNoSpaceAfterParam,FSH("Space missing after parameter"));
				pstate->state=0;
			}
			break;
		}
		case 3: //waiting for value and ETX
		////////////////////////////////////////////////
		// Parameter value parsing /////////////////////
		{
			if (pstate->param == 'M')
			{
				switch (data)
				{
					case '0':
					case '1': 
					case '2': 
					case '3':
					case '4':
					  pstate->value = data-'0';
					  pstate->state++;
						break;
					default:
					ReportError(ComRxUnknownMode,FSH("Unknown mode received"));
					pstate->state = 0;
					break;
				}
				break;
			}
			else	//Parameters with ASCII numerical value:
			{		// R, I, E, V, P, T, G, H, 1, 2, 3, 4
				if (data >= '0' && data <= '9'){ pstate->value=pstate->value*10+data-'0'; break;}
				else if (data == '\n') {pstate->state++;}	// !DO NOT BREAK HERE AS THE LAST CHAR IS ETX!
				else {
					ReportError(ComRxExpectingNumber,FSH("Expecting numerical value, received something else"));
					pstate->state = 0;
					break;
				}
			}
		}	// !DO NOT BREAK HERE AS THE LAST CHAR may be ETX! Every case already called brake if it was needed.
		// End of Parameter value parsing //////////////
		////////////////////////////////////////////////
		case 4:	{//Wait for ETX
			if (data == '\n')	//Yes! ETX received, communication finished, validate param value range
			{
				switch (pstate->param){
					case 'M': Settings->new_mode = pstate->value; break;	//Already validated
					//case statements are included in the below macros
ValidateAndApplyReceivedValue('q', pstate->value*1000, buzzerMute,    SETTINGS_MUTE_TIME_MIN, SETTINGS_MUTE_TIME_MAX, ComRxMuteTimeOutsideLimits);
ValidateAndApplyReceivedValue('R', pstate->value, Settings->target_Pramp_time,    SETTINGS_RAMPUP_MIN, SETTINGS_RAMPUP_MAX, ComRxRampOutsideLimits);
ValidateAndApplyReceivedValue('I', pstate->value, Settings->target_inspiria_time, SETTINGS_INSPIRIA_TIME_MIN, SETTINGS_INSPIRIA_TIME_MAX, ComRxInspTmOutsideLimits);
ValidateAndApplyReceivedValue('E', pstate->value, Settings->target_expiria_time,  SETTINGS_EXPIRIA_TIME_MIN, SETTINGS_EXPIRIA_TIME_MAX, ComRxExpTmOutsideLimits);
ValidateAndApplyReceivedValue('P', pstate->value, Settings->PEEP,					        SETTINGS_PEEP_MIN, SETTINGS_PEEP_MAX, ComRxPEEPOutsideLimits);

ValidateAndApplyReceivedValue('T', pstate->value, Settings->limit_PeakInspPressure,     SETTINGS_PEAK_PRESSURE_MIN, SETTINGS_PEAK_PRESSURE_MAX, ComRxPeakInspPressureOutsideLimits);
ValidateAndApplyReceivedValue('t', pstate->value, Settings->limit_InspPressure_min,      SETTINGS_MIN_PRESSURE_MIN, SETTINGS_MIN_PRESSURE_MAX, ComRxMinInspPressureOutsideLimits);
ValidateAndApplyReceivedValue('S', pstate->value, Settings->target_pressure,      SETTINGS_TARGET_PRESSURE_MIN, SETTINGS_TARGET_PRESSURE_MAX, ComRxTargetPressureOutsideLimits);
ValidateAndApplyReceivedValue('V', pstate->value, Settings->target_tidal_volume,  SETTINGS_TIDAL_VOLUME_MIN, SETTINGS_TIDAL_VOLUME_MAX, ComRxVolumeOutsideLimits);

ValidateAndApplyReceivedValue('G', pstate->value, Settings->trigger_pressure,     SETTINGS_TRIG_PRESSURE_MIN, SETTINGS_TRIG_PRESSURE_MAX, ComRxInspiriaTriggerPressureOutsideLimits);
ValidateAndApplyReceivedValue('H', pstate->value, Settings->ETS,                  SETTINGS_ETS_MIN, SETTINGS_ETS_MAX, ComRxETS_OutsideLimits);
ValidateAndApplyReceivedValue('A', pstate->value, Settings->limit_apnea_time_max,   SETTINGS_APNEA_TIME_LIMIT_MIN, SETTINGS_APNEA_TIME_LIMIT_MAX, ComRxETS_OutsideLimits);
ValidateAndApplyReceivedValue('d', pstate->value, Settings->limit_tidal_volume_min,  SETTINGS_MIN_VT_LIMIT_MIN, SETTINGS_MIN_VT_LIMIT_MAX, ComRxInspiriaTriggerPressureOutsideLimits);
ValidateAndApplyReceivedValue('D', pstate->value, Settings->limit_tidal_volume_max,  SETTINGS_MAX_VT_LIMIT_MIN, SETTINGS_MAX_VT_LIMIT_MAX, ComRxETS_OutsideLimits);

ValidateAndApplyReceivedValue('c', pstate->value, Settings->limit_minute_volume_min, SETTINGS_MIN_MINUTE_VOLUME_LIMIT_MIN, SETTINGS_MIN_MINUTE_VOLUME_LIMIT_MAX, ComRxInspiriaTriggerPressureOutsideLimits);
ValidateAndApplyReceivedValue('C', pstate->value, Settings->limit_minute_volume_max, SETTINGS_MAX_MINUTE_VOLUME_LIMIT_MIN, SETTINGS_MAX_MINUTE_VOLUME_LIMIT_MAX, ComRxETS_OutsideLimits);
ValidateAndApplyReceivedValue('b', pstate->value, Settings->limit_breath_rate_min,   SETTINGS_MIN_BREATH_RATE_LIMIT_MIN, SETTINGS_MIN_BREATH_RATE_LIMIT_MAX, ComRxInspiriaTriggerPressureOutsideLimits);
ValidateAndApplyReceivedValue('B', pstate->value, Settings->limit_breath_rate_max,   SETTINGS_MAX_BREATH_RATE_LIMIT_MIN, SETTINGS_MAX_BREATH_RATE_LIMIT_MAX, ComRxETS_OutsideLimits);

ValidateAndApplyReceivedValue('1', pstate->value/100.0, Settings->PID_Pressure.P_Factor, SETTINGS_PID_P_MIN, SETTINGS_PID_P_MAX, ComRxPIDPOutsideLimits);
ValidateAndApplyReceivedValue('2', pstate->value/100.0, Settings->PID_Pressure.I_Factor, SETTINGS_PID_I_MIN, SETTINGS_PID_I_MAX, ComRxPIDIOutsideLimits);
ValidateAndApplyReceivedValue('3', pstate->value/100.0, Settings->PID_Pressure.D_Factor, SETTINGS_PID_D_MIN, SETTINGS_PID_D_MAX, ComRxPIDDOutsideLimits);
//ValidateAndApplyReceivedValue('4', value, Settings->PID_Pressure.maxError,    SETTINGS_PID_MAX_ERR_MIN, SETTINGS_PID_MAX_ERR_MAX, ComRxPIDmaxErrOutsideLimits);
//ValidateAndApplyReceivedValue('5', value, Settings->PID_Pressure.maxSumError, SETTINGS_PID_MAX_SUM_ERR_MIN, SETTINGS_PID_MAX_SUM_ERR_MAX, ComRxPIDmaxSumErrOutsideLimits);
//ValidateAndApplyReceivedValue('6', value, Settings->PID_Pressure.maxOut,      SETTINGS_PID_MAX_OUT_MIN, SETTINGS_PID_MAX_OUT_MAX, ComRxPIDmaxOutOutsideLimits);
//ValidateAndApplyReceivedValue('7', value, Settings->PID_Pressure.minOut,      SETTINGS_PID_MIN_OUT_MIN, SETTINGS_PID_MIN_OUT_MAX, ComRxPIDminOutOutsideLimits);
ValidateAndApplyReceivedValue('4', pstate->value/100.0, Settings->PID_Flow.P_Factor, SETTINGS_PID_P_MIN, SETTINGS_PID_P_MAX, ComRxPIDPOutsideLimits);
ValidateAndApplyReceivedValue('5', pstate->value/100.0, Settings->PID_Flow.I_Factor, SETTINGS_PID_I_MIN, SETTINGS_PID_I_MAX, ComRxPIDIOutsideLimits);
ValidateAndApplyReceivedValue('6', pstate->value/100.0, Settings->PID_Flow.D_Factor, SETTINGS_PID_D_MIN, SETTINGS_PID_D_MAX, ComRxPIDDOutsideLimits);
//ValidateAndApplyReceivedValue('4', value, Settings->PID_Pressure.maxError,    SETTINGS_PID_MAX_ERR_MIN, SETTINGS_PID_MAX_ERR_MAX, ComRxPIDmaxErrOutsideLimits);
//ValidateAndApplyReceivedValue('5', value, Settings->PID_Pressure.maxSumError, SETTINGS_PID_MAX_SUM_ERR_MIN, SETTINGS_PID_MAX_SUM_ERR_MAX, ComRxPIDmaxSumErrOutsideLimits);
//ValidateAndApplyReceivedValue('6', value, Settings->PID_Pressure.maxOut,      SETTINGS_PID_MAX_OUT_MIN, SETTINGS_PID_MAX_OUT_MAX, ComRxPIDmaxOutOutsideLimits);
//ValidateAndApplyReceivedValue('7', value, Settings->PID_Pressure.minOut,      SETTINGS_PID_MIN_OUT_MIN, SETTINGS_PID_MIN_OUT_MAX, ComRxPIDminOutOutsideLimits);
ValidateAndApplyReceivedValue('7', pstate->value/100.0, Settings->PID_Volume.P_Factor, SETTINGS_PID_P_MIN, SETTINGS_PID_P_MAX, ComRxPIDPOutsideLimits);
ValidateAndApplyReceivedValue('8', pstate->value/100.0, Settings->PID_Volume.I_Factor, SETTINGS_PID_I_MIN, SETTINGS_PID_I_MAX, ComRxPIDIOutsideLimits);
ValidateAndApplyReceivedValue('9', pstate->value/100.0, Settings->PID_Volume.D_Factor, SETTINGS_PID_D_MIN, SETTINGS_PID_D_MAX, ComRxPIDDOutsideLimits);
				}
				pstate->state = 0;
				*newdata = 1;
				char txt[50];
				sprintf(txt,"R:>%c %ld ",pstate->param,pstate->value);
        ReportError(DbgMsg,txt);
			}
			else
			{
				ReportError(ComRxNoEtx,FSH("The last character was not ETX"));
			}
			pstate->state = 0;
			break;
		}
		default:
			ReportError(ComRxUnknownState,FSH("Receiver state machine: unknown state"));
			pstate->state=0;
			break;
	}
	
	if (data == '>' && pstate->state != 1)
	{
		//if STX character received in the middle of a packet, immediately jump to the appropriate state
	  pstate->state=1;
		ReportError(ComRxUnexpectedStx,FSH("An STX character was received in the middle of a message - restarting state machine"));
	}
}
