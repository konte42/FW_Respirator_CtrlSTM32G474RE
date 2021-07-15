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


//funkcija TurnOff ugasne sistem po spustu on/off tipke tre prizge led na tipki, ko je tipka pritisnjena
void TurnOff(void)
{
	static int PWR_SW_state = 1; //predstavlja stanje tipke
	static int PWR_OFF = 0;	//zastavica, ki prikazuje, če je potrebno ugasniti sistem
	int PWR_SW_state_new = HAL_GPIO_ReadPin(ON_OFF_STATE_GPIO_Port, ON_OFF_STATE_Pin);

	//preveri, ce si pritisnil tipko, da ugasnes napravo
	if(PWR_SW_state != PWR_SW_state_new && PWR_SW_state_new == 1)
	{
		PWR_OFF = 1;
	}
	//ko tipko spustis, se naprava ugasne
	else if(PWR_OFF == 1 && PWR_SW_state_new == 0)
	{
		HAL_GPIO_WritePin(OFF_GPIO_Port, OFF_Pin, GPIO_PIN_SET);
	}

	//priziganje in ugasanje LED na ON OFF tipki - prizgana, ko je tipka pritisnejan
	if(PWR_SW_state_new == 1)
	{
		HAL_GPIO_WritePin(TIPKA_LED_GPIO_Port, TIPKA_LED_Pin, GPIO_PIN_SET);
	}
	else if(PWR_SW_state_new == 0)
	{
		HAL_GPIO_WritePin(TIPKA_LED_GPIO_Port, TIPKA_LED_Pin, GPIO_PIN_RESET);
	}

	PWR_SW_state = PWR_SW_state_new;

}
