/*
 * PID.h
 *
 * Created: 27.3.2020 13:42:36
 *  Author: Kristjan
 */ 


#ifndef PID_H_
#define PID_H_

#include <inttypes.h>
#include "CommonDefinitions.h"
#define SCALING_FACTOR  128
#ifndef PID_SAMPLING_TIME
#warning "PID Sampling time not set - using default of 1 ms"
#define PID_SAMPLING_TIME	  1
#endif

typedef struct PID_DATA{
  //! Last process value, used to find derivative of process value.
  int16_t lastError;
  //! Summation of errors, used for integrate calculations
  int32_t sumError;
  //! The Proportional tuning constant, multiplied with SCALING_FACTOR
  int16_t P_Factor;
  //! The Integral tuning constant, multiplied with SCALING_FACTOR
  int16_t I_Factor;
  //! The Derivative tuning constant, multiplied with SCALING_FACTOR
  int16_t D_Factor;
  //! Maximum allowed error, avoid overflow
  int16_t maxError;
  //! Maximum allowed sum error, avoid overflow
  int32_t maxSumError;
} pidData_t;

typedef struct F_PID_DATA{
  //! Last process value, used to find derivative of process value.
  float lastError;
  //! Summation of errors, used for integrate calculations
  float sumError;
  //! The Proportional tuning constant
  float P_Factor;
  //! The Integral tuning constant
  float I_Factor;
  //! The Derivative tuning constant
  float D_Factor;
  //! Maximum allowed error, avoid runaway
  float maxError;
  //! Maximum allowed sum error, avoid runaway
  float maxSumError;
} fpidData_t;


// Maximum value of variables
#define MAX_INT         INT16_MAX
#define MAX_LONG        INT32_MAX
#define MAX_I_TERM      (MAX_LONG / 2)

// Boolean values
#define FALSE           0
#define TRUE            1

void PID_Init(int16_t p_factor, int16_t i_factor, int16_t d_factor, struct PID_DATA *pid);
int16_t PID_Calculate(int16_t setPoint, int16_t processValue, struct PID_DATA *pid_st);
void PID_Reset_Integrator(pidData_t *pid_st);
void PID_Reset_Differenciator(pidData_t *pid_st);

void PID_fInit(float p_factor, float i_factor, float d_factor, float MaxErr, float MaxSumErr, fpidData_t *pid);
float PID_fCalculate(int samplingTime, float setPoint, float processValue, fpidData_t *pid_st);
void PID_fReset_Integrator(fpidData_t *pid_st);
void PID_fReset_Differenciator(fpidData_t *pid_st);
void PID_fReset(fpidData_t *pid_st);


#endif /* PID_H_ */
