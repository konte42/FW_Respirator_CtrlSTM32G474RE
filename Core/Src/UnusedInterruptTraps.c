#include "main.h"
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "gpio.h"

void uart_dump_putc (char c)
{
  LPUART1->TDR = c;
  while((LPUART1->ISR & USART_ISR_TC_Msk) == 0); /* Wait for transmission to complete */
  LPUART1->ICR |= USART_ICR_TCCF_Msk; /* Clear tx coomplete flag */
}

void uart_dump_put_str (char *str)
{
  while (*str)
  {
    uart_dump_putc(*str++);
  }
}

/*void NMI_Handler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}
void HardFault_Handler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}
void MemManage_Handler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}
void BusFault_Handler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}
void UsageFault_Handler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}*/

/*
void SVC_Handler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}
void DebugMon_Handler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}
void PendSV_Handler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}*/
/*
void SysTick_Handler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}*/

void WWDG_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: WWDG ");
  }
}
void PVD_PVM_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: PVD_PVM ");
  }
}
void RTC_TAMP_LSECSS_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: RTC_TAMP_LSECSS ");
  }
}
void RTC_WKUP_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: RTC_WKUP");
  }
}
void FLASH_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FLASH ");
  }
}
void RCC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: RCC ");
  }
}
void EXTI0_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: EXTI0 ");
  }
}
void EXTI1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: EXTI1 ");
  }
}
void EXTI2_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: EXTI2 ");
  }
}
void EXTI3_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: EXTI3 ");
  }
}
void EXTI4_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: EXTI4 ");
  }
}
void DMA1_Channel1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA1_Channel1 ");
  }
}
void DMA1_Channel2_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA1_Channel2 ");
  }
}
void DMA1_Channel3_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA1_Channel3 ");
  }
}
void DMA1_Channel4_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA1_Channel4 ");
  }
}
void DMA1_Channel5_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA1_Channel5 ");
  }
}
void DMA1_Channel6_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA1_Channel6 ");
  }
}
void DMA1_Channel7_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA1_Channel7 ");
  }
}
/*void ADC1_2_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
      uart_dump_put_str ("BADISR:ADC1_2 ");
  }
}*/
void USB_HP_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: USB_HP ");
  }
}
void USB_LP_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: USB_LP ");
  }
}
void FDCAN1_IT0_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FDCAN1_IT0 ");
  }
}
void FDCAN1_IT1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FDCAN1_IT1 ");
  }
}
void EXTI9_5_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: EXTI9_5 ");
  }
}
void TIM1_BRK_TIM15_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM1_BRK_TIM15 ");
  }
}
void TIM1_UP_TIM16_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM1_UP_TIM16 ");
  }
}
void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM1_TRG_COM_TIM17 ");
  }
}
void TIM1_CC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM1_CC ");
  }
}
void TIM2_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM2 ");
  }
}
void TIM3_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM3 ");
  }
}
void TIM4_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM4 ");
  }
}
void I2C1_EV_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: I2C1_EV ");
  }
}
void I2C1_ER_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: I2C1_ER ");
  }
}
void I2C2_EV_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: I2C2_EV ");
  }
}
void I2C2_ER_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: I2C2_ER ");
  }
}
void SPI1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: SPI1 ");
  }
}
void SPI2_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: SPI2 ");
  }
}
/*void USART1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}
void USART2_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: USART2 ");
  }
}*/
void USART3_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: USART3 ");
  }
}
/*void EXTI15_10_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}*/
void RTC_Alarm_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: RTC_Alarm ");
  }
}
void USBWakeUp_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: USBWakeUp ");
  }
}
void TIM8_BRK_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM8_BRK ");
  }
}
void TIM8_UP_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM8_UP ");
  }
}
void TIM8_TRG_COM_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM8_TRG_COM ");
  }
}
void TIM8_CC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM8_CC ");
  }
}
void ADC3_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}
void FMC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FMC ");
  }
}
void LPTIM1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: LPTIM1 ");
  }
}
void TIM5_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM5 ");
  }
}
void SPI3_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: SPI3 ");
  }
}
void UART4_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: UART4 ");
  }
}
void UART5_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: UART5 ");
  }
}
void TIM6_DAC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM6_DAC ");
  }
}
void TIM7_DAC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM7_DAC ");
  }
}
void DMA2_Channel1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA2_Channel1 ");
  }
}
void DMA2_Channel2_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA2_Channel2 ");
  }
}
void DMA2_Channel3_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA2_Channel3 ");
  }
}
void DMA2_Channel4_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA2_Channel4 ");
  }
}
void DMA2_Channel5_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA2_Channel5 ");
  }
}/*
void ADC4_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ADC4 ");
  }
}*/
void ADC5_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ADC5 ");
  }
}
void UCPD1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: UCPD1 ");
  }
}
void COMP1_2_3_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: COMP1_2_3 ");
  }
}
void COMP4_5_6_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: COMP4_5_6 ");
  }
}
void COMP7_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: COMP7 ");
  }
}
void HRTIM1_Master_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR:  HRTIM1_Master");
  }
}
void HRTIM1_TIMA_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: HRTIM1_TIMA ");
  }
}
void HRTIM1_TIMB_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: HRTIM1_TIMB ");
  }
}
void HRTIM1_TIMC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: HRTIM1_TIMC ");
  }
}
void HRTIM1_TIMD_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: HRTIM1_TIMD ");
  }
}
void HRTIM1_TIME_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: HRTIM1_TIME ");
  }
}
void HRTIM1_FLT_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: HRTIM1_FLT ");
  }
}
void HRTIM1_TIMF_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: HRTIM1_TIMF ");
  }
}
void CRS_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: CRS ");
  }
}
void SAI1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: SAI1 ");
  }
}
void TIM20_BRK_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM20_BRK ");
  }
}
void TIM20_UP_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM20_UP ");
  }
}
void TIM20_TRG_COM_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM20_TRG_COM ");
  }
}
void TIM20_CC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: TIM20_CC ");
  }
}
void FPU_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FPU ");
  }
}
void I2C4_EV_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: I2C4_EV ");
  }
}
void I2C4_ER_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: I2C4_ER ");
  }
}
void SPI4_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: SPI4 ");
  }
}
/*
void FDCAN2_IT0_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FDCAN2_IT0 ");
  }
}
void FDCAN2_IT1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FDCAN2_IT1 ");
  }
}*/
void FDCAN3_IT0_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FDCAN3_IT0 ");
  }
}
void FDCAN3_IT1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FDCAN3_IT1 ");
  }
}
void RNG_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: RNG ");
  }
}
/*void LPUART1_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: ");
  }
}*/
void I2C3_EV_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: I2C3_EV ");
  }
}
void I2C3_ER_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: I2C3_ER ");
  }
}
void DMAMUX_OVR_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMAMUX_OVR ");
  }
}
void QUADSPI_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: QUADSPI ");
  }
}
void DMA1_Channel8_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA1_Channel8 ");
  }
}
void DMA2_Channel6_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA2_Channel6 ");
  }
}
void DMA2_Channel7_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA2_Channel7 ");
  }
}
void DMA2_Channel8_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: DMA2_Channel8 ");
  }
}
void CORDIC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: CORDIC ");
  }
}
void FMAC_IRQHandler(void)
{
  volatile int i;
  LED1_Off();
  LED2_Off();
  LED3_Off();
  LED4_Off();
  LED5_Off();
  LED6_Off();
  LED7_Off();
  while(1)
  {
    for (i=0;i<TRAP_LOOP_DELAY_LENGTH;i++){}
    LED1_Tgl();
    uart_dump_put_str ("BADISR: FMAC ");
  }
}
