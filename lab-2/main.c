// ====================
// in this lab, we'll attempt to blink with diode
// from here, you will be able to learn about bouncung and fixes for it and way become acknowledged
// with internal and external interrupts, which are very important in MC programming
//
// config functions and device drivers are in separate folders so they can be used in further
// experiments
//
// note : once again, 1st example is essential for this lab, and anothers are for familiarization
// with another possible approaches that will be mentioned in further lectures
//
// uncomment snippets to run them

#include "./config/config.h"
#include "./config/inti-handlers.h"

#include "./input/input.h"
#include "./output/output.h"

int main() {
  // configuring clocking
  Clocking_config(LL_FLASH_LATENCY_1,
                  LL_RCC_PLLSOURCE_HSI_DIV_2,
                  LL_RCC_PLL_MUL_12,
                  LL_RCC_SYSCLK_DIV_1,
                  LL_RCC_APB1_DIV_1);

  // see ./device-drivers/button.h for details and implementation
  SetButton(GPIOA, PIN_2);

  // see ./device-drivers/7segm.h for details and implementation
  SetSegm(GPIOC);

  // ====================
  // 1st option : ticking inside while (1)
  // here we use cycle iterator as tick for 7segm. the thing is, that you cant enable all 4 numbers
  // simultaneously. if you look closer at datasheet (sunrom-248200.pdf) and circuit scheme in it,
  // you will see that pins for segments in digits are the seme, but we have to choose the digit we
  // want to display. for that, we need some cycle and iterator variable in order to go through all
  // of the digits. let's see first implementation:

  /*
    uint8_t iterator = 0;
    while (1) {
      iterator = (iterator + 1) % 1000;

      // see ./device-drivers/7segm.h for details and implementation
      Segm_SetNum2(GPIOC, 1337, iterator);
    }

    return 0;
  */

  // however, you can notice that if you add some operations to the cycle, 7segm indicator will
  // start to flicker (add for (int i = 0; i < 10000; i++); to the cycle). in order to fix this, we
  // will need to iterate upon something that is invariant of operations in the cycle. From the 1st
  // lab you may remember us using timer as source of update. now, let's do the same, but with
  // 7segm:

  // ====================
  // 2nd option : timer interrupt
  // using same method as in the lab-1

  /*
    // see ./conig/config.h for details and implementation
    Tick_config();

    // see ./config/inti-handlers.h for details
    while (1) {
      // see how this cycle doesn't affect 7segm at all
      for (int i = 0; i < 10000; i++)
        ;
    }

    return 0;
  */

  // now we have achieved clear main() and independent 7segm update alongside with button, hooray!
}
