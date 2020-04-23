#ifndef __EXTI_HANDLERS_H_INCLUDED__
#define __EXTI_HANDLERS_H_INCLUDED__

#include <stdint.h>

#include "../device-drivers/button.h"
#include "./config.h"

// ====================
// BUTTON HANDLER

void EXTI2_3_IRQHandler(void) {

  // checking if interrupt indeed comes from second pin
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) == 0)
    return;

  LL_GPIO_TogglePin(GPIOB, PIN_3);

  // not forgetting to clear interruption flag!
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
}

#endif