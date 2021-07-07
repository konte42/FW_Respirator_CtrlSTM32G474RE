/*
 * ActuatorControl.h
 *
 * Created: 4. 04. 2020 10:15:43
 *  Author: maticpi
 */ 


#ifndef ACTUATORCONTROL_H_
#define ACTUATORCONTROL_H_

#include <inttypes.h>
#include "CommonDefinitions.h"
#include "ErrorHandling.h"
#include "motor.h"
#include "PID.h"
//#include "MPC.h"
#include "Measure.h"

//int32_t FIR(int16_t new_x);
void ActuatorControl(RespSettings_t *Settings, MeasuredParams_t* Measured, CtrlParams_t* Control, ControlData_t *ControlData);

#endif /* ACTUATORCONTROL_H_ */
