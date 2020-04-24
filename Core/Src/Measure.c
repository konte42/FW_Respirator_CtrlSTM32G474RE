/*
 * Measure.c
 *
 * Created: 6. 04. 2020 07:06:55
 *  Author: maticpi
 */ 
#include "Measure.h"
#include <math.h>
#define FLOW_ZERO   32410

void MeasureInit()
{
}

void MeasureFlow(MeasuredParams_t* Measured)
{
	uint16_t *ADC_Results;
	float flow_positive;
#ifdef PROTOTYPE_V1
	float flow_negative;
#endif
	float flow; // l/min
	float x;
	
	ADC_Results=ADC_results_p();
	flow_positive = *(ADC_Results+ADC_CH_FLOW  );
#ifdef PROTOTYPE_V1
	flow_negative = *(ADC_Results+ADC_CH_FLOW_N);

	if (flow_positive > flow_negative)
	{
		x=flow_positive;
		flow = -2.60967E-08*x*x + 5.33758E-03*x - 4.0E+01;//pozitivna stran
	}
	else
	{
		x=flow_negative;
		flow = -(-3.56553E-08*x*x + 5.68075E-03*x - 4.1E+01);
	}
#elif defined(PROTOTYPE_V2)
  x=flow_positive - FLOW_ZERO;
	if (x > 0)  //inspiria
  {
	  x=flow_positive - FLOW_ZERO;
    flow = -2.7090229E-07*x*x + 1.7542158E-02*x;
  }
  else  //expiria
  {
    x=FLOW_ZERO - flow_positive;
    flow = -(-4.23993E-07*x*x + 1.86869E-02*x);
  }
	if (fabs(flow) < 0.2) flow = 0;
#else
#error Prototype version not defined.
#endif
	Measured->flow=flow;
}

void MeasurePressure(MeasuredParams_t* Measured)
{
	int32_t pressure;
	uint16_t *ADC_Results;
	
	ADC_Results=ADC_results_p();
	
	pressure = (int32_t)(*(ADC_Results+ADC_CH_PRESSURE)) - PRESSURE_MIN;
	if (pressure<PRESSURE_ZERO_TRESHOLD && pressure>-PRESSURE_ZERO_TRESHOLD) pressure = 0;
	
	Measured->pressure=(float)pressure/(float)PRESSURE_SPAN * (float)PRESSURE_MAX_CMH2O;
}

void MeasureVolume(MeasuredParams_t* Measured)
{
	static float volume;
	//int volume_check=motor_GetPosition();
	int relative_vol_dif __attribute__((unused));
	
	switch (Measured->volume_mode)
	{
		case VOLUME_INTEGRATE:
			volume += Measured->flow / 60.0 * TIME_SLICE_MS;	//l/min /1000 / 60 0000
		break;
		
		case VOLUME_RESET:
			volume = 0;
		break;
		
		default://do nothing
		break;
	}
	
	if (volume<0) volume=0;

	
	Measured->volume_t=volume;
}

