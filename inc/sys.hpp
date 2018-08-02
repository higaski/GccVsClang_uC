/// System
///
/// \file   sys.hpp
/// \author Vincent Hamp
/// \date   30/04/2018

#pragma once

HAL_StatusTypeDef sysInit();
void SystemClock_Config();

#ifdef __cplusplus
extern "C" {
#endif

void SysTick_Handler();
void HardFault_Handler();
void BusFault_Handler();
void MemManage_Handler();

#ifdef __cplusplus
}
#endif
