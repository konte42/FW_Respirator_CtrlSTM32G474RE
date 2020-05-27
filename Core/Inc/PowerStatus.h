/*
 * PowerStatus.h
 *
 *  Created on: May 9, 2020
 *      Author: maticpi
 */

#ifndef INC_POWERSTATUS_H_
#define INC_POWERSTATUS_H_

#include "CommonDefinitions.h"
#include "ErrorHandling.h"
#include "gpio.h"

#ifdef PROTOTYPE_V2
  #define GetPowerStatusBit1() HAL_GPIO_ReadPin(MAINS_BAT1_GPIO_Port,MAINS_BAT1_Pin)
  #define GetPowerStatusBit2() HAL_GPIO_ReadPin(MAINS_BAT2_GPIO_Port,MAINS_BAT2_Pin)
#elif defined(PROTOTYPE_V1)
  #define GetPowerStatusBit1()  GPIO_PIN_SET
  #define GetPowerStatusBit2()  GPIO_PIN_SET
#else
  #error Prototype version not defined
#endif

typedef enum
{
  POWER_UNKNOWN,
  POWER_MAIN_ONLY,
  POWER_BATTERY_ONLY,
  POWER_MAINS_AND_BATTERY
} PowerStatus_t;

PowerStatus_t DetectPowerStatus();

#endif /* INC_POWERSTATUS_H_ */
