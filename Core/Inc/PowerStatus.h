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

#define GetPowerStatusBit1() HAL_GPIO_ReadPin(PWR_SRC_BAT_GPIO_Port,PWR_SRC_BAT_Pin)
#define GetPowerStatusBit2() HAL_GPIO_ReadPin(PWR_SRC_AC_GPIO_Port,PWR_SRC_AC_Pin)


typedef enum
{
  POWER_UNKNOWN,
  POWER_MAIN_ONLY,
  POWER_BATTERY_ONLY,
  POWER_MAINS_AND_BATTERY
} PowerStatus_t;

PowerStatus_t DetectPowerStatus();
void TurnOff();

#endif /* INC_POWERSTATUS_H_ */
