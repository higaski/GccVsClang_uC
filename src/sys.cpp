/// System
///
/// \file   sys.cpp
/// \author Vincent Hamp
/// \date   30/04/2018

#include "stm32f4xx_hal.h"
#include "sys.hpp"

/// Enable all relevant clocks
static void enable_all_clks() {
  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
}

/// System initialization
///
/// \return HAL_StatusTypeDef
HAL_StatusTypeDef sysInit() {
  GPIO_InitTypeDef GPIO_InitStruct;

  // Reset of all peripherals, Initializes the Flash interface and the Systick
  HAL_Init();

  // Configure the system clock
  SystemClock_Config();

  // 4 bits for pre-emption priority, 0 bits for subpriority
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  // SysTick_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  // MemoryManagement_IRQn interrupt configuration
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);

  // BusFault_IRQn interrupt configuration
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);

  HAL_NVIC_EnableIRQ(MemoryManagement_IRQn);
  HAL_NVIC_EnableIRQ(BusFault_IRQn);

  // Enable clocks
  enable_all_clks();

  // GPIO Configuration
  GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  return HAL_OK;
}

/// System clock configuration
void SystemClock_Config() {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  // Configure the main internal regulator output voltage
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Initializes the CPU, AHB and APB busses clocks
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  // Initializes the CPU, AHB and APB busses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  // Configure the Systick interrupt time
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

  // Configure the Systick
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  // SysTick_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/// This function handles SysTick
void SysTick_Handler() {
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

///	This function handles Hard fault interrupt
void HardFault_Handler() {
  for (;;)
    asm("nop");
}

///	This function handles Pre-fetch fault, memory access fault
void BusFault_Handler() {
  for (;;)
    asm("nop");
}

///	This function handles Memory management fault
void MemManage_Handler() {
  for (;;)
    asm("nop");
}
