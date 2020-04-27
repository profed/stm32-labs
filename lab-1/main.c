// ====================
// in this lab, we'll attempt to blink with diode
// from here, you will be able to learn about bouncung and fixes for it and way become acknowledged
// with internal and external interrupts, which are very important in MC programming
//
// config functions and device drivers are in separate folders so they can be used in further
// experiments
//
// note : 2nd and 3rd options are here just for familiarization and will be axplained on further
// lectures, no need to worry
//
// uncomment snippets to run them

#include "./lib/config/config.h"
#include "./lib/config/exti-handlers.h"
#include "./lib/config/inti-handlers.h"

#include "./lib/device-drivers/button.h"
#include "./lib/device-drivers/diode.h"

void HandlerOnPress(void* params) {
  LL_GPIO_TogglePin(GPIOB, PIN_3);
}

int main() {
  // configuring clocking
  rcc_config(LL_FLASH_LATENCY_1,
             LL_RCC_PLLSOURCE_HSI_DIV_2,
             LL_RCC_PLL_MUL_12,
             LL_RCC_SYSCLK_DIV_1,
             LL_RCC_APB1_DIV_1);

  // see ./device-drivers/button.h for details and implementation
  SetButton(GPIOA, PIN_2);

  // see ./device-drivers/diode.h for details and implementation
  SetDiode(GPIOB, PIN_3);

  // ====================
  // 1st option : mundane constant query to the port state
  // here we are simply asking port about it's state in endless cycle

  /*
      while (1) {
        int is_button_pressed = LL_GPIO_IsInputPinSet(GPIOA, PIN_2);

        if (is_button_pressed) {
          LL_GPIO_TogglePin(GPIOB, PIN_3);
        }
      }

      return 0;
  */

  // in theory, every time button is pressed, diode will be toggled. however, you'll be able to
  // notice, that sometimes diode doesn't change it's state or flickers like mad. this is caused by
  // button bouncing. let's try to fix it:

  /*
    Button_SetHandler_turn_on(HandlerOnPress);

    while (1) {
      // see lib/device-drivers/button.h for implementation and explanantions
      Button_UpdateState(GPIOA, PIN_2);
    }

    return 0;
  */

  // ====================
  // 2nd option : external interrupt
  // here we set up external interrupt to pins 2-3, thus avoiding constant queries and cycles, like
  // in upper examples

  /*
    // see lib/config/config.h && lib/config/exti-handlers.h
    EXTI_config();

    return 0;
  */

  // however, once again arises the problem of debouncing. it is not that easy to resolve it in
  // external interrupt handler without using additional timer interrupt or smth like this. now, i
  // will show one way of dealing with debounce, that will be explicitly explained in further
  // lectures (should be timers and stuff)

  // ====================
  // 3rd option : internal interrupt
  // idea behind this is to call previously discussed function Button_UpdateState(...) every tick of
  // system timer. to do so, we'll incorporate this function inside the ticking handler. thus, we
  // have actual info on button state

  /*
    // see lib/config/config.h && ./config/inti-handlers.h
    INTI_config();

    Button_SetHandler_turn_on(HandlerOnPress);

    return 0;
  */

  // as you can see, we were able to get rid of cycles in our programm, which is obviously goob,
  // because our processor is alost free to all of the other tasks we can imagine for it
}
