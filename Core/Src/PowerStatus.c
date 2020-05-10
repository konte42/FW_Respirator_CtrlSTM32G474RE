/*
 * PowerStatus.c
 *
 *  Created on: May 9, 2020
 *      Author: maticpi
 */

#include "PowerStatus.h"

PowerStatus_t DetectPowerStatus()
{
  PowerStatus_t result;
  uint8_t u8_PwrStat=0;

  if (GPIO_PIN_SET == GetPowerStatusBit1()) u8_PwrStat |= 1<<0;
  if (GPIO_PIN_SET == GetPowerStatusBit2()) u8_PwrStat |= 1<<1;

  switch (u8_PwrStat)
  {
    case 0x00:
      result = POWER_BATTERY_ONLY;
      ErrorStatusUpadate(ErrPowerSupply,ERR_STATUS_WARNING);
      ErrorStatusUpadate(ErrPowerNoBattery,ERR_STATUS_OK);
      break;

    case 0x03:
      result = POWER_MAIN_ONLY;
      ErrorStatusUpadate(ErrPowerSupply,ERR_STATUS_OK);
      ErrorStatusUpadate(ErrPowerNoBattery,ERR_STATUS_WARNING);
      break;

    case 0x02:
      result = POWER_MAINS_AND_BATTERY;
      ErrorStatusUpadate(ErrPowerSupply,ERR_STATUS_OK);
      ErrorStatusUpadate(ErrPowerNoBattery,ERR_STATUS_OK);
      break;

    default:
      result = POWER_UNKNOWN;
      ErrorStatusUpadate(ErrPowerSupply,ERR_STATUS_ERROR);
      ErrorStatusUpadate(ErrPowerNoBattery,ERR_STATUS_ERROR);
      break;
  }
  return result;
}
