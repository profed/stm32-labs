#ifndef __INTI_HANDLERS_H_INCLUDED__
#define __INTI_HANDLERS_H_INCLUDED__

#include <stdint.h>

#include "../device-drivers/button.h"
#include "./config.h"

// ====================
// TIM1 TICK HANDLER

void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
  Button_UpdateState(GPIOA, PIN_2);

  // not forgetting to clear interruption flag!
  LL_TIM_ClearFlag_UPDATE(TIM1);
}

#endif