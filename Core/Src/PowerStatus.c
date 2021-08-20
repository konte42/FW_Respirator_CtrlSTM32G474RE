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
	static int PWR_OFF = 0;	//zastavica, ki prikazuje, ce je potrebno ugasniti sistem
	static uint16_t PWR_OFF_counter = 0;
	int PWR_SW_state_new = HAL_GPIO_ReadPin(ON_OFF_STATE_GPIO_Port, ON_OFF_STATE_Pin);

	//preveri, ce si na novo pritisnil tipko
	if(PWR_SW_state != PWR_SW_state_new && PWR_SW_state_new == 1)
	{
		PWR_OFF = 1;
	}

	//ce je bila pritisnjena tipka, zacni steti koliko casa je ze pritisnjena
	if(PWR_OFF)
	{
		PWR_OFF_counter += TIME_SLICE_MS;

		//ce je bila vmes tipka spuscena, ponastavi stetje
		if(PWR_SW_state != PWR_SW_state_new && PWR_SW_state_new == 0)
		{
			PWR_OFF = 0;
			PWR_OFF_counter = 0;
		}
	}

	//ko je tipka pritisnjena dovolj casa (definirano z OFF_SW_HOLD_TIME), ugasni LED na tipki in ugasni sistem
	//tu se lahko implementira se koda, ki naroci RPi, da se ugasne
#warning "ce bo prevec debouncinga na tipki, se se lahko sistem prizge nazaj takoj po spustu tipke"
	if(PWR_OFF_counter >= OFF_SW_HOLD_TIME_MS)
	{
		CAN_XCP_write(RequestedTorque, 0 , 4 , 0 );
		HAL_GPIO_WritePin(OFF_GPIO_Port, OFF_Pin, GPIO_PIN_SET);
		SW_LED_Off();
		while(1){};
	}

	PWR_SW_state = PWR_SW_state_new;

}



void SW_LED_On()
{
	HAL_GPIO_WritePin(TIPKA_LED_GPIO_Port, TIPKA_LED_Pin, GPIO_PIN_SET);
}

void SW_LED_Off()
{
	HAL_GPIO_WritePin(TIPKA_LED_GPIO_Port, TIPKA_LED_Pin, GPIO_PIN_RESET);
}
