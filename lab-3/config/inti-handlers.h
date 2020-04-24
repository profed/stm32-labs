#ifndef __INTI_HANDLERS_H_INCLUDED__
#define __INTI_HANDLERS_H_INCLUDED__

#include <stdint.h>

#include "./config.h"

// ====================
// SYSTICK HANDLER

static uint32_t tick = 0;

void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {

  LL_TIM_ClearFlag_UPDATE(TIM1);

  tick = (tick + 1) % 1000;
}

uint8_t SysTick_GetTick() {
  return tick;
}

#endif