/*
 * Measure.h
 *
 * Created: 6. 04. 2020 07:06:36
 *  Author: maticpi
 */ 


#ifndef MEASURE_H_
#define MEASURE_H_

#include "ADC.h"
#include "CommonDefinitions.h"
#include "ErrorHandling.h"
#include "motor.h"

#ifndef AVR
#define PROGMEM
#endif

void MeasureInit();
void MeasureFlow(MeasuredParams_t* Measured);
void MeasurePressure(MeasuredParams_t* Measured);
void MeasureVolume(MeasuredParams_t* Measured);

//#define PRESSURE_MIN	(16*96UL)
//#define PRESSURE_MAX	(16*918UL)
//#define PRESSURE_MIN	(4*16*96UL)
//#define PRESSURE_MAX	(4*16*918UL)

#define PRESSURE_RANGE_MIN	6325L
#define PRESSURE_RANGE_MAX	(4*16*918L)
#define PRESSURE_ABSOLUTE_MAX	(4*16*1023L)
#define PRESSURE_SPAN	(PRESSURE_RANGE_MAX - PRESSURE_RANGE_MIN)
#define PRESSURE_ZERO_TRESHOLD	(2*16)
#define PRESSURE_MAX_MMH2O	(1000.0*1.019716)	//Converting between millibar and centimeter water [4 °C].
#define PRESSURE_MAX_CMH2O	(100.0*1.019716)

#define FLOW_ZERO   33070
#define FLOW_ZERO_TRESHOLD	0.5
#define FLOW_RANGE_MAX	32000
#define FLOW_RANGE_MIN	-32000
#define FLOW_MAX_LPERMIN	320
#define FLOW_MIN_LPERMIN	-320


#endif /* MEASURE_H_ */
