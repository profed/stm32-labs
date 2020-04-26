#ifndef __EXTI_HANDLERS_H_INCLUDED__
#define __EXTI_HANDLERS_H_INCLUDED__

#include <stdint.h>

#include "../device-drivers/encoder.h"
#include "./config.h"

// ====================
// ENCODER HANDLER

void EXTI0_1_IRQHandler(void) {
  Encoder_UpdateState(TIM2);

  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
}

#endif