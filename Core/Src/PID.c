/*
 * PID.c
 *
 * Created: 27.3.2020 13:42:23
 *  Author: Kristjan
 */ 

#include "PID.h"


void PID_Init(int16_t p_factor, int16_t i_factor, int16_t d_factor, struct PID_DATA *pid)
// Set up PID controller parameters
{
  // Start values for PID controller
  pid->sumError = 0;
  pid->lastError = 0;
  // Tuning constants for PID loop
  pid->P_Factor = p_factor;
  pid->I_Factor = i_factor;
  pid->D_Factor = d_factor;
  // Limits to avoid overflow
  pid->maxError = MAX_INT / (pid->P_Factor + 1);
  pid->maxSumError = MAX_I_TERM / (pid->I_Factor + 1);
}


/*! \brief PID control algorithm.
 *
 *  Calculates output from setpoint, process value and PID status.
 *
 *  \param samplingTime sampling time in milliseconds.
 *  \param setPoint  Desired value.
 *  \param processValue  Measured value.
 *  \param pid_st  PID status struct.
 */
int16_t PID_Calculate(int16_t setPoint, int16_t processValue, struct PID_DATA *pid_st)
{
  int16_t error, p_term, d_term;
  int32_t i_term, ret, temp;

  error = setPoint - processValue;

  // Calculate Pterm and limit error overflow
  if (error > pid_st->maxError){
    p_term = MAX_INT;
  }
  else if (error < -pid_st->maxError){
    p_term = -MAX_INT;
  }
  else{
    p_term = pid_st->P_Factor * error;
  }

  // Calculate Iterm and limit integral runaway
  temp = pid_st->sumError + error;
  if(temp > pid_st->maxSumError){
    i_term = MAX_I_TERM;
    pid_st->sumError = pid_st->maxSumError;
  }
  else if(temp < -pid_st->maxSumError){
    i_term = -MAX_I_TERM;
    pid_st->sumError = -pid_st->maxSumError;
  }
  else{
    pid_st->sumError = temp;
    i_term = pid_st->I_Factor * pid_st->sumError;
  }

  // Calculate Dterm
  d_term = pid_st->D_Factor * (error - pid_st->lastError);

  pid_st->lastError = error;

  ret = (p_term + i_term * (samplingTime / 1000) + d_term / (samplingTime / 1000)) / SCALING_FACTOR;
  if(ret > MAX_INT){
    ret = MAX_INT;
  }
  else if(ret < -MAX_INT){
    ret = -MAX_INT;
  }

  return((int16_t)ret);
}

/*! \brief Resets the integrator.
 *
 *  Calling this function will reset the integrator in the PID regulator.
 */
void PID_Reset_Integrator(pidData_t *pid_st)
{
  pid_st->sumError = 0;
}

/*! \brief Resets the differenciator.
 *
 *  Calling this function will reset the differenciator in the PID regulator.
 */
void PID_Reset_Differenciator(pidData_t *pid_st)
{
	pid_st->lastError = 0;
}



//////////////////////////////////////////////////////////////////////////////
// float
//////////////////////////////////////////////////////////////////////////////
void PID_fInit(float p_factor, float i_factor, float d_factor, float MaxErr, float MaxSumErr, fpidData_t *pid)
// Set up PID controller parameters
{
  // Start values for PID controller
  pid->sumError = 0;
  pid->lastError = 0;
  // Tuning constants for PID loop
  pid->P_Factor = p_factor;
  pid->I_Factor = i_factor;
  pid->D_Factor = d_factor;
  // Limits
  pid->maxError = MaxErr;
  pid->maxSumError = MaxSumErr;
}


/*! \brief PID control algorithm.
 *
 *  Calculates output from setpoint, process value and PID status.
 *
 *  \param samplingTime sampling time in milliseconds.
 *  \param setPoint  Desired value.
 *  \param processValue  Measured value.
 *  \param pid_st  PID status struct.
 */
float PID_fCalculate(int samplingTime, float setPoint, float processValue, fpidData_t *pid_st)
{
  float error, p_term, d_term;
  float i_term, ret, temp;

  error = setPoint - processValue;

  //limit error
  if (error > pid_st->maxError){
    error = pid_st->maxError;
  }
  else if (error < -pid_st->maxError){
	error = -pid_st->maxError;
  }
  // Calculate Pterm
  p_term = pid_st->P_Factor * error;

  //limit integral runaway
  temp = pid_st->sumError + error;
  if(temp > pid_st->maxSumError){
	temp = pid_st->maxSumError;
  }
  else if(temp < -pid_st->maxSumError){
	temp = -pid_st->maxSumError;
  }
  // Calculate Iterm
  pid_st->sumError = temp;
  i_term = pid_st->I_Factor * pid_st->sumError;

  // Calculate Dterm
  d_term = pid_st->D_Factor * (error - pid_st->lastError);

  pid_st->lastError = error;

  ret = (p_term + i_term * (samplingTime / 1000.0) + d_term / (samplingTime / 1000.0));
  if(ret > MAX_INT){
    ret = MAX_INT;
  }
  else if(ret < -MAX_INT){
    ret = -MAX_INT;
  }

  return ret;
}

/*! \brief Resets the integrator.
 *
 *  Calling this function will reset the integrator in the PID regulator.
 */
void PID_fReset_Integrator(fpidData_t *pid_st)
{
  pid_st->sumError = 0;
}

/*! \brief Resets the differenciator.
 *
 *  Calling this function will reset the differenciator in the PID regulator.
 */
void PID_fReset_Differenciator(fpidData_t *pid_st)
{
	pid_st->lastError = 0;
}

/*! \brief Resets the integrator and differenciator.
 *
 *  Calling this function will reset the integrator and the differenciator of the PID regulator.
 */
void PID_fReset(fpidData_t *pid_st)
{
  pid_st->sumError = 0;
  pid_st->lastError = 0;
}




