#ifndef __CONFIG_H_INCLUDED__
#define __CONFIG_H_INCLUDED__

#include <stddef.h>

#include "../core/stm32f051x8.h"
#include "../plib/stm32f0xx_ll_bus.h"
#include "../plib/stm32f0xx_ll_exti.h"
#include "../plib/stm32f0xx_ll_gpio.h"
#include "../plib/stm32f0xx_ll_rcc.h"
#include "../plib/stm32f0xx_ll_system.h"
#include "../plib/stm32f0xx_ll_tim.h"

// ====================
// RCC (tacting)

void rcc_config(unsigned int flash_lat, unsigned int pll_div, unsigned int pll_mul,
                unsigned int sysclk_div, unsigned int apb1_div) {
  // setting amount of waiting cycles for FLASH memory
  LL_FLASH_SetLatency(flash_lat);

  // enabling internal high-speed oscillator, thus setting it as the clocking source
  LL_RCC_HSI_Enable();
  while (LL_RCC_HSI_IsReady() != 1)
    ;

  // enabling phase-locked-loop in order to scale internal frequency
  LL_RCC_PLL_Enable();
  while (LL_RCC_PLL_IsReady() != 1)
    ;

  // the quatrz's frequency is 8 MHz, we need to set in to 48MHz in order for our periphery to work
  // correctly. so we set selector's prescale to / 2 and PLL's multiplier to 12, resulting in needed
  // 48 MHz (assuming main.c parameters)
  LL_RCC_PLL_ConfigDomain_SYS(pll_div, pll_mul);

  // now, we need to set system clock to the PLL's frequency
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    ;

  // as we need main bus (DMA, RCC, GPIO, USART, TIM, etc) to operate on the same 48MHz, we set
  // their prescalers to / 1 (assuming main.c parameters)
  LL_RCC_SetAHBPrescaler(sysclk_div);
  LL_RCC_SetAPB1Prescaler(apb1_div);
}

// ====================
// GPIO

#define PIN_0 LL_GPIO_PIN_0
#define PIN_1 LL_GPIO_PIN_1
#define PIN_2 LL_GPIO_PIN_2
#define PIN_3 LL_GPIO_PIN_3
#define PIN_4 LL_GPIO_PIN_4
#define PIN_5 LL_GPIO_PIN_5
#define PIN_6 LL_GPIO_PIN_6
#define PIN_7 LL_GPIO_PIN_7
#define PIN_8 LL_GPIO_PIN_8
#define PIN_9 LL_GPIO_PIN_9
#define PIN_10 LL_GPIO_PIN_10
#define PIN_11 LL_GPIO_PIN_11
#define PIN_12 LL_GPIO_PIN_12
#define PIN_13 LL_GPIO_PIN_13
#define PIN_14 LL_GPIO_PIN_14
#define PIN_15 LL_GPIO_PIN_15

// this function is designated to enable clocking on certain port
// because LL_..._EnableClock is idempotent (can be called multiple times resulting in the
// same result) operation (if you examine plib files, you'll se that it performs plain bit-oring and
// can be repeated indefinitely without any consequences) can be safely called multiple times
//
// this function is left as it is due to parametric nature of external device drivers
int PortX_EnableClock(GPIO_TypeDef* port) {
  if (port == GPIOA) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  } else if (port == GPIOB) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  } else if (port == GPIOC) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  } else if (port == GPIOD) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  } else if (port == GPIOF) {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
  } else {
    return -1;
  }

  return 0;
}

// ====================
// TIMER

// you may add more timers, i only needed 3 of them. also, same commentary as with PortX_EnableClock
int TimerX_EnableClock(TIM_TypeDef* timer) {
  if (timer == TIM2) {
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
  } else if (timer == TIM3) {
    LL_AHB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  } else {
    return -1;
  }

  return 0;
}

// ====================
// INTERRUPT

// i couldn't find a way to enable all interrupts parametrically in a beautiful way, so these
// functions are kind of fixed, need to be updated every time interrupt is added / deleted.
void EXTI_config() {
  // enable clocking
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);

  // setting pins 0 && 1 of port one as external interrupt sources and linking them to correspoding
  // interrupt lines. enabling interrupts on these lines
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE1);
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_1);
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);

  // enabling trigger on both falling and rising trigger for both lines, as we need to read both
  // rotation directions
  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_1);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_1);

  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_0);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);

  // enabling handlers for corresponding interrupts in interrupt controller and setting priority
  NVIC_EnableIRQ(EXTI0_1_IRQn);
  NVIC_SetPriority(EXTI0_1_IRQn, 1);
}

#endif