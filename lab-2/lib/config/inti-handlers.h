#ifndef __INTI_HANDLERS_H_INCLUDED__
#define __INTI_HANDLERS_H_INCLUDED__

#include <stdint.h>

#include "../device-drivers/7segm.h"
#include "../device-drivers/button.h"
#include "./config.h"

// ====================
// TIMER TICK HANDLER

static uint8_t tick = 0;

int  number = 0;
void HandlerOnPress(void* params) {
  number++;
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void) {
  Button_SetHandler_turn_on(HandlerOnPress);

  Button_UpdateState(GPIOA, PIN_2);

  // displaying current amount of clicks
  Segm_SetNum2(GPIOC, number, tick);

  tick = (tick + 1) % 1000;

  LL_TIM_ClearFlag_UPDATE(TIM1);
}

#endif