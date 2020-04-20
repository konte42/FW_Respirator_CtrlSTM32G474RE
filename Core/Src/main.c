/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <modeCMV.h>
#include <modePCV.h>
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PID.h"
#include "CommonDefinitions.h"
#include "motor.h"
#include "Measure.h"
#include "ActuatorControl.h"
#include "CommProtocol.h"
#include "modeSTOP.h"
#include "modeHWtest.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//Global only for debugging purposes move back to main when done
RespSettings_t  Settings;
MeasuredParams_t Measured;
CtrlParams_t Control;
fpidData_t PIDdata;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
#define MSG_SETTINGS_REPLY_LENGTH 200
  /* USER CODE BEGIN 1 */
	  char msg[MSG_SETTINGS_REPLY_LENGTH];
	  int length;
	  char com_data;
	#ifdef AVR
	  uint32_t mark1=0;
	#endif
	  uint32_t mark2=0;
	  uint8_t newSettingsReceived;

	  /*
	  RespSettings_t	Settings;
	  MeasuredParams_t Measured;
	  CtrlParams_t Control;
	  fpidData_t PIDdata;	//Same PID params if regulating P or V ? Probably not.
						  //Maybe make PID params local to ActuatorControl?
*/
	  memset(&Settings,0,sizeof(RespSettings_t));
	  memset(&Measured,0,sizeof(MeasuredParams_t));
	  memset(&Control,0,sizeof(CtrlParams_t));
	  memset(&PIDdata,0,sizeof(fpidData_t));

	  //V končni verziji se to prebere iz eeproma,
	  //da takoj nadaljujemo od koder smo končali,
	  //če se slučajno pobiramo iz nenamernega reseta
	  Settings.current_mode=MODE_DEFAULT;
	  Settings.new_mode=MODE_DEFAULT;
	  Settings.target_Pramp_time=SETTINGS_DEFAULT_RAMPUP_TIME_MS;
	  Settings.target_inspiratory_time=SETTINGS_DEFAULT_INHALE_TIME_MS;
	  Settings.target_expiratory_time=SETTINGS_DEFAULT_EXHALE_TIME_MS;
	  Settings.target_volume=SETTINGS_DEFAULT_TARGET_VOLUME_ML;
	  Settings.PEEP = SETTINGS_DEFAULT_PEEP;
	  Settings.PeakInspPressure = SETTINGS_DEFAULT_MAX_PRESSURE_MBAR;
    Settings.target_pressure = SETTINGS_DEFAULT_TARGET_PRESSURE_MBAR;

    Settings.PID_Pressure.P_Factor = SETTINGS_DEFAULT_PRESSURE_PID_P;
    Settings.PID_Pressure.I_Factor = SETTINGS_DEFAULT_PRESSURE_PID_I;
    Settings.PID_Pressure.D_Factor = SETTINGS_DEFAULT_PRESSURE_PID_D;
    Settings.PID_Pressure.maxError = SETTINGS_DEFAULT_PRESSURE_PID_MAXERR;
    Settings.PID_Pressure.maxSumError = SETTINGS_DEFAULT_PRESSURE_PID_MAXSUMERR;
    Settings.PID_Pressure.maxOut = SETTINGS_DEFAULT_PRESSURE_PID_MAXOUT;
    Settings.PID_Pressure.minOut = SETTINGS_DEFAULT_PRESSURE_PID_MINOUT;

//	  Settings.PID_P = SETTINGS_DEFAULT_PID_P;
//	  Settings.PID_I = SETTINGS_DEFAULT_PID_I;
//	  Settings.PID_D = SETTINGS_DEFAULT_PID_D;

	  //TODO: read current state of the machine
	  //Is it possible the get the exact state?
	  //Direction (Inspiration/Expiration), Position
	  //duration of the break (maybe ms resolution RTC)?
	  //maybe also external RAM with backup battery? Or FRAM?

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  Ringbuf_Init();
  HAL_TIM_Base_Start(&htim1);
  HAL_ADC_Start_IT(&hadc1);
  HAL_ADC_Start_IT(&hadc2);
  HAL_ADC_Start_IT(&hadc3);
  MeasureInit();
  motor_Init();
/*
  motor_SetDir(MOTOR_DIR_VDIH);
  int dc=0;
  while (1)
  {
	dc+=MOTOR_MAX_DC/10;
	if (dc>MOTOR_MAX_DC) dc=MOTOR_MAX_DC/10;

	motor_SetDutyCycle(dc);
	HAL_Delay(100);

  }*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /*while (1)
  {
    if (UART0_DataReady())
    {
      char data;
      UART0_GetByte(&data);
      UART0_put(data);
    }
  }*/
  int timeout=0;
  while(1)
  {
    // na 2 ms
    if (ADC_scan_complete())
    {
      //LED1_On();
      // branje ADC:
      MeasureFlow(&Measured);
      MeasurePressure(&Measured);
      MeasureVolume(&Measured);

      //TODO: mode state machines must return HW independent control values
      switch (Settings.current_mode)
      {
        case MODE_STOP:
          modeSTOP(&Settings, &Measured, &Control);
          ActuatorControl(&Control,&Measured,&Settings,&PIDdata);
          break;
        case MODE_CMV:
          modeCMV(&Settings, &Measured, &Control);
          ActuatorControl(&Control,&Measured,&Settings,&PIDdata);
          break;
        case MODE_PCV:
          modePCV(&Settings, &Measured, &Control);
          ActuatorControl(&Control,&Measured,&Settings,&PIDdata);
          break;
        case MODE_CPAP_PS:
          modeSTOP(&Settings, &Measured, &Control);
          ActuatorControl(&Control,&Measured,&Settings,&PIDdata);
          break;
        case MODE_HW_TEST:
          modeHWtest(&Settings, &Measured, &Control);
          ActuatorControl(&Control,&Measured,&Settings,&PIDdata);
          break;
        default:
          ReportError(ModeUnknownMode,NULL/*"Unknown operation mode"*/);
          Settings.current_mode = MODE_DEFAULT;
          break;
      }
      //ActuatorControl(&Control,&Measured,&Settings,&PIDdata);
      //LED1_Off();
      //koda traja xy us (140 us before hardware abstraction was implemented)
    }

    // na 2 ms
#ifdef AVR	//On ARM AD conversions are triggered once per ms by a timer
    if (Has_X_MillisecondsPassed(TIME_SLICE_MS,&mark1))
    {
	    ADC_Start_First_Conversion();
    }
#endif


    //Report Status to the GUI
//    if (Has_X_MillisecondsPassed(STATUS_REPORTING_PERIOD,&mark2))
    if (HAL_GetTick()-mark2 >= STATUS_REPORTING_PERIOD)
    {
      //LED6_Tgl();
      mark2+=STATUS_REPORTING_PERIOD;
      length=PrepareStatusMessage(HAL_GetTick(),
           (int16_t)(Measured.flow*100.0), (int16_t)(Measured.pressure*100.0),
           (int16_t)(Measured.volume_t*10.0), (int16_t)(motor_GetPosition()),
           (uint16_t)motor_GetCurrent(), motor_GetPWM(), Control.BreathCounter,
           Control.status, Control.Error, Control.target_pressure, msg);
      UART0_SendBytes(msg,length);
      if (timeout > 0 )
      {
	timeout --;
      }
      else
      {
	//LED2_Off();
      }
    }
    //Listen for commands

    if(UART0_DataReady())	//process received data 1 byte per loop
    {
	    UART0_GetByte(&com_data);
	    ProcessMessages(com_data, &Settings, &newSettingsReceived);
    }
    if (newSettingsReceived)
    {
	    newSettingsReceived = 0;
	    length=ReportAllCurrentSettings(msg,MSG_SETTINGS_REPLY_LENGTH,&Settings);
	    if (length > 0)
	    {
		    //LED2_On();
		    timeout=20;
		    UART0_SendBytes(msg,length);
	    }
    }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_8) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_LPUART1
                              |RCC_PERIPHCLK_ADC12|RCC_PERIPHCLK_ADC345;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  PeriphClkInit.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
