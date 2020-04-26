#ifndef __ENCODER_H_INCLUDED__
#define __ENCODER_H_INCLUDED__

#include "../../core/stm32f051x8.h"
#include "../../plib/stm32f0xx_ll_tim.h"
#include "../config/config.h"

// ====================
// ENCODER

static void (*Encoder_handler_turn_left_)(void* params)  = NULL;
static void (*Encoder_handler_turn_right_)(void* params) = NULL;
static void (*Encoder_handler_left_)(void* params)       = NULL;
static void (*Encoder_handler_right_)(void* params)      = NULL;

void Encoder_SetHandler_turn_left(void (*Encoder_handler_turn_left)(void* params)) {
  Encoder_handler_turn_left_ = Encoder_handler_turn_left;
}

void Encoder_SetHandler_turn_right(void (*Encoder_handler_turn_right)(void* params)) {
  Encoder_handler_turn_right_ = Encoder_handler_turn_right;
}

void Encoder_SetHandler_left(void (*Encoder_handler_left)(void* params)) {
  Encoder_handler_left_ = Encoder_handler_left;
}

void Encoder_SetHandler_right(void (*Encoder_handler_right)(void* params)) {
  Encoder_handler_right_ = Encoder_handler_right;
}

int SetEncoder(GPIO_TypeDef* port, TIM_TypeDef* timer, unsigned int pin_l, unsigned int pin_r) {
  // enable clocking on given port. see ../config/config.h for details
  PortX_EnableClock(port);

  // setting pins modes as alternative and assigning corresponding alternative functions from the
  // table. also, setting pull direction up, because middle pin of encoder is grounded
  LL_GPIO_SetPinMode(port, pin_r, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinMode(port, pin_l, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(port, pin_r, LL_GPIO_AF_2);
  LL_GPIO_SetAFPin_0_7(port, pin_l, LL_GPIO_AF_2);
  LL_GPIO_SetPinPull(port, pin_r, LL_GPIO_PULL_UP);
  LL_GPIO_SetPinPull(port, pin_l, LL_GPIO_PULL_UP);

  // enabling timer to work with encoder
  TimerX_EnableClock(timer);

  // setting timer to encoder mode and setting up channels 1 and 2. enabling timer
  LL_TIM_SetEncoderMode(timer, LL_TIM_ENCODERMODE_X4_TI12);
  LL_TIM_IC_SetPolarity(timer, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_FALLING);
  LL_TIM_IC_SetPolarity(timer, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_FALLING);
  LL_TIM_SetAutoReload(timer, 0xFFFF);
  LL_TIM_EnableCounter(timer);

  return 0;
}

enum EncoderStatus {
  Left,
  Right,
  Turn_left,
  Turn_right,
  Undefined,
};

// same debounce strategy as with button, see ../config/exti-handlers.h for details
typedef struct {
  uint8_t            counter_cur;
  uint8_t            counter_max;
  uint8_t            delta;
  enum EncoderStatus status;

} EncoderState;
// if results are bad, change counter_max && delta accordingly
static EncoderState state_encoder = {8, 16, 4, Undefined};

// same commentary as with Button_UpdateState. Only one encoder is designed to work
enum EncoderStatus Encoder_UpdateState(TIM_TypeDef* timer) {
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
}

#endif