/*
 * ActuatorControl.c
 *
 * Created: 4. 04. 2020 10:14:58
 *  Author: maticpi
 */ 
#include "ActuatorControl.h"

  float motorSpeed;
  //float motorCurrent;
//TODO: every time control mode is changed, PID could be reset and loaded with appropriate parameters
void ActuatorControl(RespSettings_t *Settings, MeasuredParams_t* Measured, CtrlParams_t* Control, ControlData_t *ControlData)
{
//  static uint8_t last_mode = CTRL_PAR_MODE_STOP;
//  uint8_t cur_mode = Control->mode;

	//motorCurrent=motor_GetCurrent();
	Control->cur_position = motor_GetPosition();
	Control->cur_speed = (Control->cur_position - Control->last_position) / TIME_SLICE_MS;
	
	switch(Control->mode)
	{
// STOP MODES ///////////////////////////////////////////////////////////////////////
		case CTRL_PAR_MODE_STOP:
			motor_SetPower(0);
			PID_fReset(ControlData->PIDdata);
			break;

		case CTRL_PAR_MODE_HOLD_MAX_CLOSED_POSITION:
			motor_SetPower(0.01);  //turn motor direction to close (inhale) and
			break;                 //maintain minimum duty cycle to hold clamps together

// POWER CONTROL MODE ///////////////////////////////////////////////////////////
		case CTRL_PAR_MODE_TARGET_POWER:
			if (Control->target_power > 0)
			{
				if (Control->cur_position < CTRL_PAR_MAX_POSITION)	//Obey if within limits
				  { motor_SetPower(Control->target_power);}
				else {  motor_SetPower(0); }	//Stop if too far
			}
			else //else if (Control->target_power < 0)
			{
				if (Control->cur_position > CTRL_PAR_MIN_POSITION)	//Obey if within limits
				  { motor_SetPower(Control->target_power);}
				else
				  { motor_SetPower(0); }  //Stop if too far
			}
			break;

// POSITION REGULATION MODES ///////////////////////////////////////////////////////////
		//not used
		case CTRL_PAR_MODE_TARGET_POSITION_INHALE:
			if (Control->target_position - Control->cur_position >= 0)
			{
				if (Control->target_position - Control->cur_position > 2) { motorSpeed=50; }
				else { motorSpeed = 20; }
			}
			if (motorSpeed > 0)
			{
				motor_SetPower(motorSpeed);
			}
			
		break;
		
		case CTRL_PAR_MODE_TARGET_POSITION: //Replace with PID regulator
		  motorSpeed=0;
			if (Control->target_position - Control->cur_position >= 0)
			{
				if (Control->target_position - Control->cur_position > 2) motorSpeed=25;
				else
				{
					Control->mode=CTRL_PAR_MODE_STOP;
					motor_SetPower(0);
				}
			}
			if (Control->target_position - Control->cur_position < 0)
			{
				if (Control->target_position - Control->cur_position < -2) motorSpeed=-15;
				else
				{
					Control->mode=CTRL_PAR_MODE_STOP;
					motor_SetPower(0);
				}
			}
			motor_SetPower(motorSpeed);
		break;

// PRESSURE REGULATION MODES ///////////////////////////////////////////////////////////
// dummy regulation for motor start-up - should probably be solved differently
		case CTRL_PAR_MODE_DUMMY_REGULATE_PRESSURE_PID_RESET:
			PID_fInit(Settings->PID_Pressure.P_Factor,
			          Settings->PID_Pressure.I_Factor,
			          Settings->PID_Pressure.D_Factor,
			          Settings->PID_Pressure.maxError,
			          Settings->PID_Pressure.maxSumError,
			          ControlData->PIDdata);
			Control->mode=CTRL_PAR_MODE_DUMMY_REGULATE_PRESSURE;
			//DO NOT PUT BREAK HERE!

		case CTRL_PAR_MODE_DUMMY_REGULATE_PRESSURE:
			//can only regulate inspiration		//pressure span 50mmH2O --> cca 14500 (14500/16 = cca 900)
			motorSpeed = PID_fCalculate(1,Control->target_pressure, Measured->pressure, ControlData->PIDdata);
			if (Control->target_power > 0)
			{
				if (Control->cur_position < CTRL_PAR_MAX_POSITION)	//Obey if within limits
				{ motor_SetPower(Control->target_power); }
				else { motor_SetPower(0); } //Stop if too far
			}
			else // if (Control->target_speed < 0)
			{
				if (Control->cur_position > CTRL_PAR_MIN_POSITION)	//Obey if within limits
        { motor_SetPower(Control->target_power); }
        else { motor_SetPower(0); } //Stop if too far
			}
			break;

////////////////// normal pressure regulation mode /////////////////////////
		case CTRL_PAR_MODE_REGULATE_PRESSURE_PID_RESET:
      PID_fInit(Settings->PID_Pressure.P_Factor,
                Settings->PID_Pressure.I_Factor,
                Settings->PID_Pressure.D_Factor,
                Settings->PID_Pressure.maxError,
                Settings->PID_Pressure.maxSumError,
                ControlData->PIDdata);
			Control->mode=CTRL_PAR_MODE_REGULATE_PRESSURE;
			//DO NOT PUT BREAK HERE!

		case CTRL_PAR_MODE_REGULATE_PRESSURE:
			//can only regulate inspiration		//pressure span 50mmH2O
		  motorSpeed = PID_fCalculate(1,Control->target_pressure, Measured->pressure, ControlData->PIDdata);
			if (Control->cur_position >= CTRL_PAR_MAX_POSITION)
			{
				motorSpeed = 0.01;  //minimum speed setting to maintain inhale direction and minimum clamp pressure
			}
			if (motorSpeed < 0) motorSpeed = 0.01;  //minimum speed setting to maintain inhale direction and minimum clamp pressure
			motor_SetPower(motorSpeed);
		break;

// VOLUME REGULATION MODES ///////////////////////////////////////////////////////////
// dummy regulation for motor start-up - should probably be solved differently
    case CTRL_PAR_MODE_DUMMY_REGULATE_VOLUME_PID_RESET:
      PID_fInit(Settings->PID_Volume.P_Factor,
                Settings->PID_Volume.I_Factor,
                Settings->PID_Volume.D_Factor,
                Settings->PID_Volume.maxError,
                Settings->PID_Volume.maxSumError,
                ControlData->PIDdata);
      Control->mode=CTRL_PAR_MODE_DUMMY_REGULATE_VOLUME;
      //DO NOT PUT BREAK HERE!

    case CTRL_PAR_MODE_DUMMY_REGULATE_VOLUME:
      //can only regulate inspiration   //pressure span 50mmH2O --> cca 14500 (14500/16 = cca 900)
      motorSpeed = PID_fCalculate(1,Control->target_volume, Measured->volume_t, ControlData->PIDdata);
      if (Control->target_power > 0)
      {
        if (Control->cur_position < CTRL_PAR_MAX_POSITION)  //Obey if within limits
        { motor_SetPower(Control->target_power); }
        else { motor_SetPower(0); } //Stop if too far
      }
      else // if (Control->target_speed < 0)
      {
        if (Control->cur_position > CTRL_PAR_MIN_POSITION)  //Obey if within limits
        { motor_SetPower(Control->target_power); }
        else { motor_SetPower(0); } //Stop if too far
      }
      break;

////////////////// normal volume regulation mode /////////////////////////
    case CTRL_PAR_MODE_REGULATE_VOLUME_PID_RESET:
      PID_fInit(Settings->PID_Volume.P_Factor,
                Settings->PID_Volume.I_Factor,
                Settings->PID_Volume.D_Factor,
                Settings->PID_Volume.maxError,
                Settings->PID_Volume.maxSumError,
                ControlData->PIDdata);
      Control->mode=CTRL_PAR_MODE_REGULATE_VOLUME_PID;
      //DO NOT PUT BREAK HERE!

    case CTRL_PAR_MODE_REGULATE_VOLUME_PID:
      //can only regulate inspiration   //pressure span 50mmH2O
      motorSpeed = PID_fCalculate(1,Control->target_volume, Measured->volume_t, ControlData->PIDdata);
      if (Control->cur_position >= CTRL_PAR_MAX_POSITION)
      {
        motorSpeed = 0.01;  //minimum speed setting to maintain inhale direction and minimum clamp pressure
      }
      if (motorSpeed < 0) motorSpeed = 0.01;  //minimum speed setting to maintain inhale direction and minimum clamp pressure
      motor_SetPower(motorSpeed);
    break;

#warning "MPC regulator zacasno zbrisan"
    ////////////////// MPC volume regulation mode /////////////////////////
        case CTRL_PAR_MODE_REGULATE_VOLUME_MPC_RESET:
/*        	MPC_Init(Settings->MPC_Volume.modelT,
        	         Settings->MPC_Volume.Amat,
        	         Settings->MPC_Volume.Bmat,
        	         Settings->MPC_Volume.Cmat,
        	         Settings->MPC_Volume.modelTm,
        	         Settings->MPC_Volume.Hmpc,
        	         Settings->MPC_Volume.maxError,
					 ControlData->MPCdata);
          Control->mode=CTRL_PAR_MODE_REGULATE_VOLUME_MPC;
          //DO NOT PUT BREAK HERE!
*/
        case CTRL_PAR_MODE_REGULATE_VOLUME_MPC:
/*          //can only regulate inspiration   //pressure span 50mmH2O
        	motorSpeed = MPC_Calculate(Control->target_volume, Measured->volume_t, ControlData->MPCdata);
          if (Control->cur_position >= CTRL_PAR_MAX_POSITION)
          {
            motorSpeed = 0.01;  //minimum speed setting to maintain inhale direction and minimum clamp pressure
          }
          if (motorSpeed < 0) motorSpeed = 0.01;  //minimum speed setting to maintain inhale direction and minimum clamp pressure
          motor_SetPower(motorSpeed);*/
        break;


// FLOW REGULATION MODES ///////////////////////////////////////////////////////////
// dummy regulation for motor start-up - should probably be solved differently
    case CTRL_PAR_MODE_DUMMY_REGULATE_FLOW_PID_RESET:
      PID_fInit(Settings->PID_Flow.P_Factor,
                Settings->PID_Flow.I_Factor,
                Settings->PID_Flow.D_Factor,
                Settings->PID_Flow.maxError,
                Settings->PID_Flow.maxSumError,
                ControlData->PIDdata);
      Control->mode=CTRL_PAR_MODE_DUMMY_REGULATE_FLOW;
      //DO NOT PUT BREAK HERE!

    case CTRL_PAR_MODE_DUMMY_REGULATE_FLOW:
      //can only regulate inspiration
      motorSpeed = PID_fCalculate(1,Control->target_flow, Measured->flow, ControlData->PIDdata);
      if (Control->target_power > 0)
      {
        if (Control->cur_position < CTRL_PAR_MAX_POSITION)  //Obey if within limits
        { motor_SetPower(Control->target_power); }
        else { motor_SetPower(0); } //Stop if too far
      }
      else // if (Control->target_speed < 0)
      {
        if (Control->cur_position > CTRL_PAR_MIN_POSITION)  //Obey if within limits
        { motor_SetPower(Control->target_power); }
        else { motor_SetPower(0); } //Stop if too far
      }
      break;

////////////////// normal flow regulation mode /////////////////////////
	case CTRL_PAR_MODE_REGULATE_FLOW_PID_RESET:
      PID_fInit(Settings->PID_Flow.P_Factor,
                Settings->PID_Flow.I_Factor,
                Settings->PID_Flow.D_Factor,
                Settings->PID_Flow.maxError,
                Settings->PID_Flow.maxSumError,
                ControlData->PIDdata);
			Control->mode=CTRL_PAR_MODE_REGULATE_FLOW_PID;
      //DO NOT PUT BREAK HERE!

		case CTRL_PAR_MODE_REGULATE_FLOW_PID:
			//can only regulate inspiratory part of the cycle
			motorSpeed = PID_fCalculate(1,Control->target_flow, Measured->flow, ControlData->PIDdata);
			if (Control->cur_position >= CTRL_PAR_MAX_POSITION)
			{
        motorSpeed = 0.01;  //minimum speed setting to maintain inhale direction and minimum clamp pressure
			}
      if (motorSpeed<0) motorSpeed = 0.01;
      motor_SetPower(motorSpeed);
		break;

///////////////////////////////////////////////////////////////////////////////////
// ERROR - UNKNOWN MODE ///////////////////////////////////////////////////////////
		default: //Error: Stop immediately
		ReportError(ActuatorCtrlUnknownMode,FSH("Unknown actuator control mode"));
		Control->mode=CTRL_PAR_MODE_TARGET_POSITION;
		motor_SetPower(0);
		break;
	}

//	last_mode = cur_mode;
	Control->last_position = Control->cur_position;
}
