/*
 * Common.h
 *
 * Created: 3. 04. 2020 17:25:53
 *  Author: maticpi
 */ 


#ifndef COMMON_DEFINITIONS_H_
#define COMMON_DEFINITIONS_H_

#include <inttypes.h>
#include <stddef.h>
#define PID_SAMPLING_TIME   TIME_SLICE_MS   //only applies to the INT version of PID.
#include "PID.h"  //PID_SAMPLING_TIME must be defined before including "PID.h"

//app defines
#define MSG_CORE_LENGTH	20
#ifdef AVR
#define TIME_SLICE_MS	2	//Timeslice in ms
#else
#define TIME_SLICE_MS	1	//Timeslice in ms
#endif
#define STATUS_REPORTING_PERIOD	10	// ms

#define MODE_DEFAULT	0	//Kateri je default?
#define MODE_STOP	0
#define MODE_C_VCV	1		
#define MODE_C_PCV	2
#define MODE_AC_VCV	3
#define MODE_AC_PCV	4
#define MODE_CPAP	5
#define MODE_HW_TEST	6

//default settings
#define SETTINGS_DEFAULT_MODE					MODE_DEFAULT
#define SETTINGS_DEFAULT_RAMPUP_TIME_MS			100		// 50ms	???	100ms/TIME_SLICE_MS?
#define SETTINGS_DEFAULT_INHALE_TIME_MS			1250	// 1.25s
#define SETTINGS_DEFAULT_EXHALE_TIME_MS			3000	// 3s
#define SETTINGS_DEFAULT_TARGET_PRESSURE_MBAR	300		// mmH2O
#define SETTINGS_DEFAULT_MAX_PRESSURE_MBAR		400		// mmH2O
#define SETTINGS_DEFAULT_PEEP					50		// mmH2O
#define SETTINGS_DEFAULT_TARGET_VOLUME_ML		400		// milliliters

#define SETTINGS_DEFAULT_PRESSURE_PID_P	        	1 //64	// = P/SCALING_FACTOR = 0.5
#define SETTINGS_DEFAULT_PRESSURE_PID_I       		50 //1	// = I/SCALING_FACTOR
#define SETTINGS_DEFAULT_PRESSURE_PID_D           1 // = D/SCALING_FACTOR
#define SETTINGS_DEFAULT_PRESSURE_PID_MAXERR      500
#define SETTINGS_DEFAULT_PRESSURE_PID_MAXSUMERR   500
#define SETTINGS_DEFAULT_PRESSURE_PID_MAXOUT      100
#define SETTINGS_DEFAULT_PRESSURE_PID_MINOUT      -100

//settings limits
#define SETTINGS_RAMPUP_MIN			  0
//#define SETTINGS_RAMPUP_MIN			50
#define SETTINGS_RAMPUP_MAX			  200
#define SETTINGS_INHALE_TIME_MIN	100
#define SETTINGS_INHALE_TIME_MAX	2000
#define SETTINGS_EXHALE_TIME_MIN	100
#define SETTINGS_EXHALE_TIME_MAX	10000
#define SETTINGS_VOLUME_MIN			  100
#define SETTINGS_VOLUME_MAX			  1000
#define SETTINGS_PEEP_MAX			    200
#define SETTINGS_PEEP_MIN			    0
#define SETTINGS_PRESSURE_MIN		  10
#define SETTINGS_PRESSURE_MAX		  1000
#define SETTINGS_PID_P_MIN			  INT16_MIN
#define SETTINGS_PID_P_MAX  			INT16_MAX
#define SETTINGS_PID_I_MIN	  		INT16_MIN
#define SETTINGS_PID_I_MAX		  	INT16_MAX
#define SETTINGS_PID_D_MIN			  INT16_MIN
#define SETTINGS_PID_D_MAX		  	INT16_MAX

#define SETTINGS_PID_MAX_ERR_MIN        INT16_MIN
#define SETTINGS_PID_MAX_ERR_MAX        INT16_MAX
#define SETTINGS_PID_MAX_SUM_ERR_MIN    INT16_MIN
#define SETTINGS_PID_MAX_SUM_ERR_MAX    INT16_MAX
#define SETTINGS_PID_MAX_OUT_MIN        INT16_MIN
#define SETTINGS_PID_MAX_OUT_MAX        INT16_MAX
#define SETTINGS_PID_MIN_OUT_MIN        INT16_MIN
#define SETTINGS_PID_MIN_OUT_MAX        INT16_MAX


//PID Settings
typedef struct F_PID_SETTINGS{
  //! The Proportional tuning constant
  float P_Factor;
  //! The Integral tuning constant
  float I_Factor;
  //! The Derivative tuning constant
  float D_Factor;
  //! Maximum allowed error
  float maxError;
  //! Maximum allowed sum error
  float maxSumError;
  //! Maximum output
  float maxOut;
  //! Minimum output
  float minOut;
} fpidSettings_t;

//Settings
typedef struct RESPIRATOR_SETTINGS{
	uint8_t current_mode;
	uint8_t	new_mode;		//If new_mode is different than current mode, do transition
	uint16_t target_Pramp_time;
	uint16_t target_inspiratory_time;
	uint16_t target_expiratory_time;
	uint16_t target_volume;
//	uint8_t breathing_rate;
	uint16_t PEEP;
	uint16_t PeakInspPressure;
	uint16_t target_pressure;
	fpidSettings_t PID_Position;
	fpidSettings_t PID_Pressure;
  fpidSettings_t PID_Volume;
  fpidSettings_t PID_Flow;
  fpidSettings_t PID_Speed;
} RespSettings_t;

//Measured Parameters

typedef enum
{
	VOLUME_STOP,
	VOLUME_INTEGRATE,
	VOLUME_RESET
} MeasureVolumeMode_t;

typedef struct MEASURED_PARAMS{
	float flow;							// l/min
	float pressure;						// cmH2O (=mbar)
	float volume_t;						// ml/b  (=ml/vdih)
	MeasureVolumeMode_t volume_mode;
} MeasuredParams_t;


//Control Parameters
#define CTRL_PAR_MODE_STOP									0
#define CTRL_PAR_MODE_HOLD_MAX_CLOSED_POSITION				1
#define CTRL_PAR_MODE_TARGET_SPEED							2
#define CTRL_PAR_MODE_TARGET_POSITION_INHALE				3
#define CTRL_PAR_MODE_TARGET_POSITION						4
#define CTRL_PAR_MODE_DUMMY_REGULATE_PRESSURE_PID_RESET		5
#define CTRL_PAR_MODE_DUMMY_REGULATE_PRESSURE				6
#define CTRL_PAR_MODE_REGULATE_PRESSURE_PID_RESET			7
#define CTRL_PAR_MODE_REGULATE_PRESSURE						8
#define CTRL_PAR_MODE_REGULATE_VOLUME_PID_RESET				9
#define CTRL_PAR_MODE_REGULATE_VOLUME						10
#define CTRL_PAR_MODE_REGULATE_FLOW_PID_RESET				11
#define CTRL_PAR_MODE_REGULATE_FLOW							12

#define CTRL_PAR_MAX_POSITION	100
#define CTRL_PAR_MIN_POSITION	0

typedef struct CONTROL_PARAMS{
	uint8_t mode;		//regulate speed/position
	float target_speed;	// max: +-100%
	float target_position;	// max: +100%, 0 = completely exhaled, theoretically should not go below 0
	float cur_speed;	// %/ms (calculated from the current an last position)
	float cur_position;	// 0-100
	float last_position;	// 0-100
	float max_speed;	// max: +-100%
	float target_pressure;	// cmH2O
	float target_volume;	// ml
	float target_flow;	// l/min
	int16_t BreathCounter;	//steje vdihe
	uint8_t status;	//stanje state machina za dihanje
	uint8_t Error;	//napake (bitwise)
} CtrlParams_t;



#endif /* COMMON_DEFINITIONS_H_ */
