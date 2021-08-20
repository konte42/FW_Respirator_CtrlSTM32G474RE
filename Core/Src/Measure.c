/*
 * Measure.c
 *
 * Created: 6. 04. 2020 07:06:55
 *  Author: maticpi
 */ 
#include "Measure.h"
#include <math.h>

void MeasureInit()
{
}

void MeasureFlow(MeasuredParams_t* Measured)
{
	uint16_t *ADC_Results;
	float a,b,c,d; //koeficienti polinoma za kalibracijo
	float flow_positive;
	float flow; // l/min
	float x;
	
	ADC_Results=ADC_results_p();
	flow_positive = *(ADC_Results+ADC_CH_FLOW  );

	x = flow_positive - FLOW_ZERO;	//odstej offset



	if (x < -2900.0)  //polinom 1
	{
		if (x<FLOW_RANGE_MIN) x = FLOW_RANGE_MIN;
		a=0.0;
		b=1.78655E-07;
		c=0.01418606;
		d=-14.7;
	}
	else if (x<0.0)	//polinom 2
	{
		a=-7.94439E-10;
		b=-2.66738E-06;
		c=0.0176807;
		d=0.0;
	}
	else if (x<3200.0) //polinom 3
	{
		a=-3.04523E-10;
		b=6.15348E-07;
		c=0.01833781;
		d=0.0;
	}
	else	//polinom 4
	{
		if (x>FLOW_RANGE_MAX) x = FLOW_RANGE_MAX;
		a=0.0;
		b=-1.14179E-07;
		c=0.01353111;
		d=12.9;
	}

	flow = ((a*x+b)*x+c)*x+d;	//more efficient coding.

	if (flow < FLOW_ZERO_TRESHOLD && flow > -FLOW_ZERO_TRESHOLD) flow = 0.0;
	if (flow >FLOW_MAX_LPERMIN) flow=FLOW_MAX_LPERMIN;
	if (flow <FLOW_MIN_LPERMIN) flow=FLOW_MIN_LPERMIN;
	Measured->flow=flow;
}

void MeasurePressure(MeasuredParams_t* Measured)
{
	int32_t pressure;
	uint16_t *ADC_Results;
	
	ADC_Results=ADC_results_p();
	
	pressure = (int32_t)(*(ADC_Results+ADC_CH_PRESSURE)) - PRESSURE_RANGE_MIN;
	if (pressure<PRESSURE_ZERO_TRESHOLD && pressure>-PRESSURE_ZERO_TRESHOLD) pressure = 0;
	if (pressure>PRESSURE_ABSOLUTE_MAX) pressure = PRESSURE_ABSOLUTE_MAX;
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

