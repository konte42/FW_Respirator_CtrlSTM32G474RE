/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "main.h"
#include "adc.h"
#include "fdcan.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PID.h"
#include "CommonDefinitions.h"
#include "PowerStatus.h"
#include "motor.h"
#include "Measure.h"
#include "ActuatorControl.h"
#include "CommProtocol.h"
#include "modeSTOP.h"
#include "modeCMV.h"
//#include "modeCMVmpc.h"
#include "modePCV.h"
#include "modePS.h"
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
//mpcData_t MPCdata;
ControlData_t ControlData =
{
	&PIDdata,
	//&MPCdata
};

int _write(int file, char *ptr, int len)
{
  HAL_UART_Transmit_IT(&huart1, (uint8_t*)ptr, len);
  return UART_OK;
}

void dummy()
{

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
#define MSG_SETTINGS_REPLY_LENGTH 200

	  char msg[MSG_SETTINGS_REPLY_LENGTH];
	  int length;
	  char com_data;
	#ifdef AVR
	  uint32_t mark1=0;
	#endif
    uint32_t mark2=0;
    uint32_t mark1ms=0;
	  uint8_t newSettingsReceived;
	  ErrCodes_t err;
    ProcMsgState_t PMSuart0 = {0};
    ProcMsgState_t PMSuart1 = {0};

	  /*
	  RespSettings_t	Settings;
	  MeasuredParams_t Measured;
	  CtrlParams_t Control;
	  fpidData_t PIDdata;	//Same PID params if regulating P or V ? Probably not.
						  //Maybe make PID params local to ActuatorControl?
*/
	  ErrQueue_Init(&DefaultErrorQueue);
	  memset(&Settings,0,sizeof(RespSettings_t));
	  memset(&Measured,0,sizeof(MeasuredParams_t));
	  memset(&Control,0,sizeof(CtrlParams_t));
	  memset(&PIDdata,0,sizeof(fpidData_t));
	  //memset(&MPCdata,0,sizeof(mpcData_t));

	  //V končni verziji se to prebere iz eeproma,
	  //da takoj nadaljujemo od koder smo končali,
	  //če se slučajno pobiramo iz nenamernega reseta
	  Settings.current_mode=MODE_STOP;
	  Settings.new_mode=MODE_DEFAULT;
    Settings.trigger_pressure=SETTINGS_DEFAULT_INSP_TRIGGER;
    Settings.target_Pramp_time=SETTINGS_DEFAULT_RAMPUP_TIME_MS;
	  Settings.target_inspiria_time=SETTINGS_DEFAULT_INHALE_TIME_MS;
    Settings.target_expiria_time=SETTINGS_DEFAULT_EXHALE_TIME_MS;
    Settings.limit_apnea_time_max=SETTINGS_DEFAULT_APNEA_LIMIT_TIME_MS;
	  Settings.target_tidal_volume=SETTINGS_DEFAULT_TARGET_VOLUME_ML;
	  Settings.PEEP = SETTINGS_DEFAULT_PEEP;
	  Settings.limit_PeakInspPressure = SETTINGS_DEFAULT_MAX_PRESSURE_MBAR;
    Settings.target_pressure = SETTINGS_DEFAULT_TARGET_PRESSURE_MBAR;
    Settings.target_motor_speed = SETTINGS_DEFAULT_TARGET_MOTOR_SPEED;
    Settings.limit_tidal_volume_min = SETTINGS_DEFAULT_LIMIT_TIDAL_VOLUME_MIN;
    Settings.limit_tidal_volume_max = SETTINGS_DEFAULT_LIMIT_TIDAL_VOLUME_MAX;
    Settings.limit_minute_volume_min = SETTINGS_DEFAULT_MINUTE_VOLUME_MIN;
    Settings.limit_minute_volume_max = SETTINGS_DEFAULT_MINUTE_VOLUME_MAX;
    Settings.limit_breath_rate_min = SETTINGS_DEFAULT_BREATH_RATE_MIN;
    Settings.limit_breath_rate_max = SETTINGS_DEFAULT_BREATH_RATE_MAX;
    Settings.limit_InspPressure_min = SETTINGS_DEFAULT_MIN_INSP_PRESSURE;
    Settings.ETS = SETTINGS_DEFAULT_ETS;

    Settings.PID_Pressure.P_Factor = SETTINGS_DEFAULT_PRESSURE_PID_P;
    Settings.PID_Pressure.I_Factor = SETTINGS_DEFAULT_PRESSURE_PID_I;
    Settings.PID_Pressure.D_Factor = SETTINGS_DEFAULT_PRESSURE_PID_D;
    Settings.PID_Pressure.maxError = SETTINGS_DEFAULT_PRESSURE_PID_MAXERR;
    Settings.PID_Pressure.maxSumError = SETTINGS_DEFAULT_PRESSURE_PID_MAXSUMERR;
    Settings.PID_Pressure.maxOut = SETTINGS_DEFAULT_PRESSURE_PID_MAXOUT;
    Settings.PID_Pressure.minOut = SETTINGS_DEFAULT_PRESSURE_PID_MINOUT;

    Settings.PID_Flow.P_Factor = SETTINGS_DEFAULT_FLOW_PID_P;
    Settings.PID_Flow.I_Factor = SETTINGS_DEFAULT_FLOW_PID_I;
    Settings.PID_Flow.D_Factor = SETTINGS_DEFAULT_FLOW_PID_D;
    Settings.PID_Flow.maxError = SETTINGS_DEFAULT_FLOW_PID_MAXERR;
    Settings.PID_Flow.maxSumError = SETTINGS_DEFAULT_FLOW_PID_MAXSUMERR;
    Settings.PID_Flow.maxOut = SETTINGS_DEFAULT_FLOW_PID_MAXOUT;
    Settings.PID_Flow.minOut = SETTINGS_DEFAULT_FLOW_PID_MINOUT;

    Settings.PID_Volume.P_Factor = SETTINGS_DEFAULT_VOLUME_PID_P;
    Settings.PID_Volume.I_Factor = SETTINGS_DEFAULT_VOLUME_PID_I;
    Settings.PID_Volume.D_Factor = SETTINGS_DEFAULT_VOLUME_PID_D;
    Settings.PID_Volume.maxError = SETTINGS_DEFAULT_VOLUME_PID_MAXERR;
    Settings.PID_Volume.maxSumError = SETTINGS_DEFAULT_VOLUME_PID_MAXSUMERR;
    Settings.PID_Volume.maxOut = SETTINGS_DEFAULT_VOLUME_PID_MAXOUT;
    Settings.PID_Volume.minOut = SETTINGS_DEFAULT_VOLUME_PID_MINOUT;

    Settings.MPC_Volume.modelT=SETTINGS_DEFAULT_VOLUME_MPC_modelT;
	Settings.MPC_Volume.Amat=SETTINGS_DEFAULT_VOLUME_MPC_Amat;
	Settings.MPC_Volume.Bmat=SETTINGS_DEFAULT_VOLUME_MPC_Bmat;
	Settings.MPC_Volume.Cmat=SETTINGS_DEFAULT_VOLUME_MPC_Cmat;
	Settings.MPC_Volume.modelTm=SETTINGS_DEFAULT_VOLUME_MPC_modelTm;
	Settings.MPC_Volume.Hmpc=SETTINGS_DEFAULT_VOLUME_MPC_Hmpc;
	Settings.MPC_Volume.maxError=SETTINGS_DEFAULT_VOLUME_MPC_maxError;


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
  MX_ADC2_Init();
  MX_ADC4_Init();
  MX_FDCAN2_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);
    Ringbuf0_Init();
    Ringbuf1_Init();
    HAL_TIM_Base_Start(&htim3);
    HAL_TIM_Base_Start(&htim4);

    HAL_ADC_Start_IT(&hadc2);
    HAL_ADC_Start_IT(&hadc4);
    MeasureInit();
    motor_Init();
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);	//BUZZ ON
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);	//BUZZ OFF
    //BUZZ_Off();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    while(1)
    {
      //Check power status every ms, regardless of everything else
      //At least some error checking should also be done here
      if (HAL_GetTick() != mark1ms)
      {
        mark1ms=HAL_GetTick();
        DetectPowerStatus();
      }

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
            ActuatorControl(&Settings,&Measured,&Control,&ControlData);
            break;

          case MODE_CMV:
//            modeCMVmpc(&Settings, &Measured, &Control);
            modeCMV(&Settings, &Measured, &Control);
            ActuatorControl(&Settings,&Measured,&Control,&ControlData);
            break;

          case MODE_PCV:
            modePCV(&Settings, &Measured, &Control);
            ActuatorControl(&Settings,&Measured,&Control,&ControlData);
            break;

          case MODE_PS:
            modePS(&Settings, &Measured, &Control);
            ActuatorControl(&Settings,&Measured,&Control,&ControlData);
            break;

          case MODE_HW_TEST:
            modeHWtest(&Settings, &Measured, &Control);
            ActuatorControl(&Settings,&Measured,&Control,&ControlData);
            break;

          default:
            ReportError(ModeUnknownMode,FSH("Unknown operation mode"));
            Settings.current_mode = MODE_DEFAULT;
            break;
        }
        //ActuatorControl(&Control,&Measured,&Settings,&PIDdata);
        //LED1_Off();
        //koda traja xy us (140 us before hardware abstraction was implemented)
        ErrorBuzzer();
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
      if (Control.ReportReady & (1<<CTR_REPRDY_INSP))
      {
        Control.ReportReady &= ~(1<<CTR_REPRDY_INSP);
        length=PrepareMetricsMessage(msg, MSG_SETTINGS_REPLY_LENGTH, &Metrics);
        UART0_SendBytes(msg,length);
        UART1_SendBytes(msg,length);
      }
      if (Control.ReportReady & (1<<CTR_REPRDY_EXP))
      {
        Control.ReportReady &= ~(1<<CTR_REPRDY_EXP);
        length=PrepareMetricsMessage(msg, MSG_SETTINGS_REPLY_LENGTH, &Metrics);
        UART0_SendBytes(msg,length);
        UART1_SendBytes(msg,length);
       /* length=PrepareStatisticsMessage(msg, MSG_SETTINGS_REPLY_LENGTH, &Statistics);
        UART0_SendBytes(msg,length);
        UART1_SendBytes(msg,length);*/
      }

      if (HAL_GetTick()-mark2 >= STATUS_REPORTING_PERIOD)
      {

        ErrQueue_GetErr(&err,&DefaultErrorQueue);
        mark2+=STATUS_REPORTING_PERIOD;

        length=PrepareStatusMessage(mark1ms,
             (int16_t)(Measured.flow*100.0), (int16_t)(Measured.pressure*100.0),
             (int16_t)(Measured.volume_t*10.0), (int16_t)motor_GetPosition(),
             (uint16_t)motor_GetCurrent(), (uint16_t) ceil(motor_GetPower()), Control.BreathCounter,
             Control.status, err, Control.target_volume, msg);
        UART0_SendBytes(msg,length);
        UART1_SendBytes(msg,length);
      }

      //Listen for commands
      if(UART0_DataReady())	//process received data 1 byte per loop
      {
  	    UART0_GetByte(&com_data);
  	    ProcessMessages(com_data, &Settings, &PMSuart0, &newSettingsReceived);
      }
      if (newSettingsReceived)
      {
  	    newSettingsReceived = 0;
  	    length=ReportAllCurrentSettings(msg,MSG_SETTINGS_REPLY_LENGTH,&Settings);
  	    if (length > 0)
  	    {
          UART0_SendBytes(msg,length);
          UART1_SendBytes(msg,length);
  	    }
      }

      //Listen for commands
      if(UART1_DataReady()) //process received data 1 byte per loop
      {
        UART1_GetByte(&com_data);
        ProcessMessages(com_data, &Settings, &PMSuart1, &newSettingsReceived);
      }
      if (newSettingsReceived)
      {
        newSettingsReceived = 0;
        length=ReportAllCurrentSettings(msg,MSG_SETTINGS_REPLY_LENGTH,&Settings);
        if (length > 0)
        {
          UART0_SendBytes(msg,length);
          UART1_SendBytes(msg,length);
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
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_I2C2|RCC_PERIPHCLK_ADC12
                              |RCC_PERIPHCLK_ADC345|RCC_PERIPHCLK_FDCAN;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
