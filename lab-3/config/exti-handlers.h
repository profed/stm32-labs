#ifndef __EXTI_HANDLERS_H_INCLUDED__
#define __EXTI_HANDLERS_H_INCLUDED__

#include <stdint.h>

#include "../device-drivers/encoder.h"
#include "./config.h"

// ====================
// ENCODER HANDLER

void EXTI0_1_IRQHandler(void) {
  static EncoderState state_encoder = {5, 10, 1, Undefined};

  // this function automatically does what we'd have to do manually to get rotation. it gets signals
  // from both channels and compares them, thus identifying rotation direction
  unsigned int rot = LL_TIM_GetCounterMode(TIM2);

  // same strategy as with button
  if (rot == LL_TIM_COUNTERMODE_DOWN && state_encoder.counter_cur > 0)
    state_encoder.counter_cur--;
  else if (rot == LL_TIM_COUNTERMODE_UP && state_encoder.counter_cur < state_encoder.counter_max)
    state_encoder.counter_cur++;

  if (state_encoder.status == !Right && state_encoder.counter_cur > state_encoder.delta) {
    state_encoder.status = Turn_right;
    if (Encoder_handler_turn_right_ != NULL)
      Encoder_handler_turn_right_(NULL);
  } else if (state_encoder.status == !Left &&
             state_encoder.counter_cur < state_encoder.counter_max - state_encoder.delta) {
    state_encoder.status = Turn_left;
    if (Encoder_handler_turn_left_ != NULL)
      Encoder_handler_turn_left_(NULL);
  } else if ((state_encoder.status == Turn_left || state_encoder.status == Undefined) &&
             state_encoder.counter_cur < state_encoder.delta) {
    state_encoder.status = Left;
    if (Encoder_handler_left_ != NULL)
      Encoder_handler_left_(NULL);
  } else if ((state_encoder.status == Turn_right || state_encoder.status == Undefined) &&
             state_encoder.counter_cur > state_encoder.counter_max - state_encoder.delta) {
    state_encoder.status = Right;
    if (Encoder_handler_right_ != NULL)
      Encoder_handler_right_(NULL);
  }

  // ! never forget to clear interruption flags !
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
  LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
}

#endif