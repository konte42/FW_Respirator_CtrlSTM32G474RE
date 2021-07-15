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
//#include "MPC.h"  //PID_SAMPLING_TIME must be defined before including "MPC.h"

#define PROTOTYPE_V2
//#define PROTOTYPE_V1

//app defines
#define MSG_CORE_LENGTH	25+12
#ifdef AVR
#define TIME_SLICE_MS	2	//Timeslice in ms
#else
#define TIME_SLICE_MS	1	//Timeslice in ms
#endif
#define STATUS_REPORTING_PERIOD	10	// ms

#define METRICS_MAX_HIST_LENGTH   12
#define METRICS_MIN_HIST_LENGTH   3
#define METRICS_MAX_HIST_TIME     60000

#define MODE_DEFAULT	0	//Kateri je default?
#define MODE_STOP	    0
#define MODE_CMV	    1
#define MODE_PCV	    2
#define MODE_PS	      3
#define MODE_HW_TEST	4

//default settings
#define SETTINGS_DEFAULT_MODE					        MODE_DEFAULT
#define SETTINGS_DEFAULT_RAMPUP_TIME_MS			  200		// 50ms	???	100ms/TIME_SLICE_MS?
#define SETTINGS_DEFAULT_INHALE_TIME_MS			  1250	// 1.25s
#define SETTINGS_DEFAULT_EXHALE_TIME_MS       3000  // 3s
#define SETTINGS_DEFAULT_APNEA_LIMIT_TIME_MS  5000  // 20s
#define SETTINGS_DEFAULT_TARGET_PRESSURE_MBAR	100		// mmH2O
#define SETTINGS_DEFAULT_MAX_PRESSURE_MBAR		400		// mmH2O
#define SETTINGS_DEFAULT_TARGET_MOTOR_SPEED	50	//	50%
#define SETTINGS_DEFAULT_PEEP					        50		// mmH2O
#define SETTINGS_DEFAULT_TARGET_VOLUME_ML		  1500		// milliliters
#define SETTINGS_DEFAULT_INSP_TRIGGER         10  // -1cmH2O
#define SETTINGS_DEFAULT_LIMIT_TIDAL_VOLUME_MIN   200
#define SETTINGS_DEFAULT_LIMIT_TIDAL_VOLUME_MAX   1500
#define SETTINGS_DEFAULT_MINUTE_VOLUME_MIN    2000
#define SETTINGS_DEFAULT_MINUTE_VOLUME_MAX    30000
#define SETTINGS_DEFAULT_BREATH_RATE_MIN      4
#define SETTINGS_DEFAULT_BREATH_RATE_MAX      50
#define SETTINGS_DEFAULT_MIN_INSP_PRESSURE    10
#define SETTINGS_DEFAULT_ETS                  25



#define SETTINGS_DEFAULT_PRESSURE_PID_P	        	5
#define SETTINGS_DEFAULT_PRESSURE_PID_I       		20
#define SETTINGS_DEFAULT_PRESSURE_PID_D           0.05
#define SETTINGS_DEFAULT_PRESSURE_PID_MAXERR      500
#define SETTINGS_DEFAULT_PRESSURE_PID_MAXSUMERR   20000
#define SETTINGS_DEFAULT_PRESSURE_PID_MAXOUT      100
#define SETTINGS_DEFAULT_PRESSURE_PID_MINOUT      -100

#define SETTINGS_DEFAULT_FLOW_PID_P           2 //64  // = P/SCALING_FACTOR = 0.5
#define SETTINGS_DEFAULT_FLOW_PID_I           30 //1  // = I/SCALING_FACTOR
#define SETTINGS_DEFAULT_FLOW_PID_D           0.05 // = D/SCALING_FACTOR
#define SETTINGS_DEFAULT_FLOW_PID_MAXERR      500
#define SETTINGS_DEFAULT_FLOW_PID_MAXSUMERR   20000
#define SETTINGS_DEFAULT_FLOW_PID_MAXOUT      100
#define SETTINGS_DEFAULT_FLOW_PID_MINOUT      -100

#define SETTINGS_DEFAULT_VOLUME_PID_P           0.2 //64  // = P/SCALING_FACTOR = 0.5
#define SETTINGS_DEFAULT_VOLUME_PID_I           5.0 //1  // = I/SCALING_FACTOR
#define SETTINGS_DEFAULT_VOLUME_PID_D           0.000 // = D/SCALING_FACTOR
#define SETTINGS_DEFAULT_VOLUME_PID_MAXERR      500
#define SETTINGS_DEFAULT_VOLUME_PID_MAXSUMERR   20000
#define SETTINGS_DEFAULT_VOLUME_PID_MAXOUT      100
#define SETTINGS_DEFAULT_VOLUME_PID_MINOUT      -100

#define SETTINGS_DEFAULT_VOLUME_MPC_modelT		0.5747 //time constant of first order model in seconds
#define SETTINGS_DEFAULT_VOLUME_MPC_Amat		0.99826	//y = Ax+Bu, y = Cx
#define SETTINGS_DEFAULT_VOLUME_MPC_Bmat		0.000999
#define SETTINGS_DEFAULT_VOLUME_MPC_Cmat		49.591
#define SETTINGS_DEFAULT_VOLUME_MPC_modelTm		0.160  //dead time in seconds - Smith predictor (nekaj kar smo pogruntali sami, pa nismo vedeli, kaj je to)
#define SETTINGS_DEFAULT_VOLUME_MPC_Hmpc		60	//horizont predikcije (koliko samplov naprej računaš predikcijo in gledaš pogrešek
#define SETTINGS_DEFAULT_VOLUME_MPC_maxError	500	//omejitev napake, enako kot pri PID

//settings limits
//#define SETTINGS_RAMPUP_MIN			  0
#define SETTINGS_RAMPUP_MIN			    50
#define SETTINGS_RAMPUP_MAX			    200
#define SETTINGS_INSPIRIA_TIME_MIN	100
#define SETTINGS_INSPIRIA_TIME_MAX	3000
#define SETTINGS_EXPIRIA_TIME_MIN	  100
#define SETTINGS_EXPIRIA_TIME_MAX	  10000
#define SETTINGS_PEEP_MIN                 0
#define SETTINGS_PEEP_MAX                 200

#define SETTINGS_PEAK_PRESSURE_MIN		  10
#define SETTINGS_PEAK_PRESSURE_MAX		  1000
#define SETTINGS_MIN_PRESSURE_MIN       10
#define SETTINGS_MIN_PRESSURE_MAX       1000
#define SETTINGS_TARGET_PRESSURE_MIN    10
#define SETTINGS_TARGET_PRESSURE_MAX    1000
#define SETTINGS_TIDAL_VOLUME_MIN       100
//#define SETTINGS_TIDAL_VOLUME_MAX       1500
#define SETTINGS_TIDAL_VOLUME_MAX       2000

#define SETTINGS_MOTOR_POWER_MIN		-100
#define SETTINGS_MOTOR_POWER_MAX		100

#define SETTINGS_MOTOR_SPEED_MIN		-100
#define SETTINGS_MOTOR_SPEED_MAX		100

#define SETTINGS_TRIG_PRESSURE_MIN        5
#define SETTINGS_TRIG_PRESSURE_MAX        20
#define SETTINGS_ETS_MIN                  25
#define SETTINGS_ETS_MAX                  75

#define SETTINGS_APNEA_TIME_LIMIT_MIN     1000
#define SETTINGS_APNEA_TIME_LIMIT_MAX     20000

#define SETTINGS_MIN_VT_LIMIT_MIN         100
#define SETTINGS_MIN_VT_LIMIT_MAX         1500
#define SETTINGS_MAX_VT_LIMIT_MIN         100
#define SETTINGS_MAX_VT_LIMIT_MAX         1500

#define SETTINGS_MIN_MINUTE_VOLUME_LIMIT_MIN 1000
#define SETTINGS_MIN_MINUTE_VOLUME_LIMIT_MAX 30000
#define SETTINGS_MAX_MINUTE_VOLUME_LIMIT_MIN 1000
#define SETTINGS_MAX_MINUTE_VOLUME_LIMIT_MAX 30000

#define SETTINGS_MIN_BREATH_RATE_LIMIT_MIN   1
#define SETTINGS_MIN_BREATH_RATE_LIMIT_MAX   50
#define SETTINGS_MAX_BREATH_RATE_LIMIT_MIN   1
#define SETTINGS_MAX_BREATH_RATE_LIMIT_MAX   50

#define SETTINGS_MUTE_TIME_MIN   1000
#define SETTINGS_MUTE_TIME_MAX   300000

#define MAX_PS_INSP_TIME  5000

#define SETTINGS_PID_P_MIN			  0
#define SETTINGS_PID_P_MAX  			UINT16_MAX
#define SETTINGS_PID_I_MIN	  		0
#define SETTINGS_PID_I_MAX		  	UINT16_MAX
#define SETTINGS_PID_D_MIN			  0
#define SETTINGS_PID_D_MAX		  	UINT16_MAX

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


//MPC regulator settings
typedef struct MPC_SETTINGS{
	float modelT;
	float Amat;
	float Bmat;
	float Cmat;
	float modelTm;
	float Hmpc;
	float maxError;
}MPCSettings_t;

//Settings
typedef struct RESPIRATOR_SETTINGS{
	uint8_t current_mode;
	uint8_t	new_mode;		//If new_mode is different than current mode, do transition
	uint16_t target_Pramp_time;
	uint16_t target_inspiria_time;
	uint16_t target_expiria_time;
	uint16_t limit_apnea_time_max;
	uint16_t target_tidal_volume;
	uint16_t limit_tidal_volume_min;
	uint16_t limit_tidal_volume_max;
	uint16_t limit_minute_volume_min;
	uint16_t limit_minute_volume_max;
	uint16_t limit_breath_rate_min;
	uint16_t limit_breath_rate_max;
	uint16_t PEEP;
	uint16_t limit_InspPressure_min;
	uint16_t limit_PeakInspPressure;
	uint16_t target_pressure;
	uint16_t trigger_pressure; // PCAP-PS inhale trigger
	uint16_t ETS;              // Expiria Stop trigger
	int16_t target_motor_power;	//debug HW test
	int16_t target_motor_speed;	//debug HW test
	fpidSettings_t PID_Pressure;
	fpidSettings_t PID_Flow;
	fpidSettings_t PID_Volume;
	MPCSettings_t MPC_Volume;
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
typedef enum
{
	CTRL_PAR_MODE_STOP,
	CTRL_PAR_MODE_HOLD_MAX_CLOSED_POSITION,
	CTRL_PAR_MODE_TARGET_POWER,
	CTRL_PAR_MODE_TARGET_POSITION_INHALE,
	CTRL_PAR_MODE_TARGET_POSITION,
	CTRL_PAR_MODE_DUMMY_REGULATE_PRESSURE_PID_RESET,
	CTRL_PAR_MODE_DUMMY_REGULATE_PRESSURE,
	CTRL_PAR_MODE_REGULATE_PRESSURE_PID_RESET,
	CTRL_PAR_MODE_REGULATE_PRESSURE,
	CTRL_PAR_MODE_DUMMY_REGULATE_VOLUME_PID_RESET,
	CTRL_PAR_MODE_DUMMY_REGULATE_VOLUME,
	CTRL_PAR_MODE_REGULATE_VOLUME_PID_RESET,
	CTRL_PAR_MODE_REGULATE_VOLUME_PID,
	CTRL_PAR_MODE_DUMMY_REGULATE_FLOW_PID_RESET,
	CTRL_PAR_MODE_DUMMY_REGULATE_FLOW,
	CTRL_PAR_MODE_REGULATE_FLOW_PID_RESET,
	CTRL_PAR_MODE_REGULATE_FLOW_PID,
	CTRL_PAR_MODE_REGULATE_VOLUME_MPC_RESET,
	CTRL_PAR_MODE_REGULATE_VOLUME_MPC,
} Ctrl_Par_Modes_t;


#define CTRL_PAR_MAX_POSITION	100
#define CTRL_PAR_MIN_POSITION	0

#define CTR_REPRDY_INSP   0
#define CTR_REPRDY_EXP   1

typedef struct CONTROL_PARAMS{
	Ctrl_Par_Modes_t mode;		//regulate power/position/...
	float target_power;	// max: +-100%
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
  uint8_t Error;  //napake (bitwise)
  uint8_t ReportReady;  //bit0 - insp done, bit1 exp done
} CtrlParams_t;


typedef struct CONTROL_DATA{
	 fpidData_t *PIDdata;
	// mpcData_t *MPCdata;
} ControlData_t;

typedef enum
{
  MODE_STATE_FIRST_RUN = 0x00,
  MODE_STATE_EXP_START = 0x10,    //16
  MODE_STATE_EXP_ZERO_POS_WAIT,
  MODE_STATE_EXP_WAIT,
  MODE_STATE_LOOK_FOR_INSP_TRIGGER,
  MODE_STATE_INSP_INIT = 0x20,    //32
  MODE_STATE_INSP_PREP_1,
  MODE_STATE_INSP_PREP_2,
  MODE_STATE_INSP_PREP_3,
  MODE_STATE_INSP_PRAMP = 0x30,   //48
  MODE_STATE_INSP_CONST_P,
  MODE_STATE_INSP_CONST_S,
  MODE_STATE_INSP_CONST_F,
  MODE_STATE_INSP_RAMP_V,
  MODE_STATE_INSP_COMPLETE_ETS_TRIG = 0x40,  //64
  MODE_STATE_INSP_COMPLETE_MAX_VOL,
  MODE_STATE_INSP_COMPLETE_MAX_POS,
  MODE_STATE_INSP_COMPLETE_MAX_PRESSURE,
} ModeStates_t;



#endif /* COMMON_DEFINITIONS_H_ */
