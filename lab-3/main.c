// ====================
// in this lab, we'll attempt to work with encoder
// from here, you will be able to learn about using timers in alternative modes and about basic work
// with encoder. these ckills may prove useful later on, when doing final project, as encoder is
// basically the mousewheel and can be used to track not only the direction of rotation, but also
// it's speed
//
// config functions and device drivers are in separate folders so they can be used in further
// experiments
//
// note : 1st option is vital, another ones are for your familiarization and should be explained in
// details in further lectures
//
// uncomment snippets to run them

#include "./lib/config/config.h"
#include "./lib/config/exti-handlers.h"

#include "./lib/device-drivers/button.h"
#include "./lib/device-drivers/diode.h"
#include "./lib/device-drivers/encoder.h"

void HandlerTurnLeft(void* params) {
  LL_GPIO_SetOutputPin(GPIOB, PIN_4);
  LL_GPIO_ResetOutputPin(GPIOB, PIN_3);
}

void HandlerTurnRight(void* params) {
  LL_GPIO_SetOutputPin(GPIOB, PIN_3);
  LL_GPIO_ResetOutputPin(GPIOB, PIN_4);
}

int main() {
  // configuring clocking
  rcc_config(LL_FLASH_LATENCY_1,
             LL_RCC_PLLSOURCE_HSI_DIV_2,
             LL_RCC_PLL_MUL_12,
             LL_RCC_SYSCLK_DIV_1,
             LL_RCC_APB1_DIV_1);

  // see ./device-dravers/encoder.h for details and implementation
  SetEncoder(GPIOA, TIM2, PIN_0, PIN_1);

  // diodes for rotation indication
  SetDiode(GPIOB, PIN_3);
  SetDiode(GPIOB, PIN_4);

  // LL_GPIO_SetOutputPin(GPIOB, PIN_3);

  // =====================
  // 1st option :
  // as usual, first option is boring and bad. constant call of Encoder_UpdateState function in
  // endless cycle. let's see how it works:

  while (1) {
    // see ./device-drivers/encoder.h for details and implementation
    int rot = Encoder_UpdateState(TIM2);

    if (state_encoder.status == Turn_left) {
      LL_GPIO_SetOutputPin(GPIOB, PIN_4);
      LL_GPIO_ResetOutputPin(GPIOB, PIN_3);
    } else if (state_encoder.status == Turn_right) {
      LL_GPIO_SetOutputPin(GPIOB, PIN_3);
      LL_GPIO_ResetOutputPin(GPIOB, PIN_4);
    }
  }

  return 0;

  // assuming you did previous labs, it's obvious why such implementation is poor. it depends on
  // cycle's busyness

  // =====================
  // option 2 : (exercise)
  // now, you can turn on ticking interrupt and put Encoder_GetRotation inside of it,
  // just as we did with button and 7segm, nothing difficult

  // ====================
  // option 3 : external interrupts
  // with encoder, it's possible to implement adequate and simple debounce using external
  // interrupts. below, i will demonstrate this

  /*
    EXTI_config();

    Encoder_SetHandler_turn_left(HandlerTurnLeft);
    Encoder_SetHandler_turn_right(HandlerTurnRight);

    return 0;
  */

  // this version works pretty well without any bouncing problems
}
