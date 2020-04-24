#ifndef __7SEGM_H_INCLUDED__
#define __7SEGM_H_INCLUDED__

#include "../config/config.h"
#include "../config/inti-handlers.h"

// ====================
// 7SEGM

// redefines for better understanding. all naming is in according with datasheet
#define DIOD_A PIN_11
#define DIOD_B PIN_7
#define DIOD_C PIN_4
#define DIOD_D PIN_2
#define DIOD_E PIN_1
#define DIOD_F PIN_10
#define DIOD_G PIN_5
#define DIOD_DP PIN_3
#define DIGIT1 PIN_12
#define DIGIT2 PIN_9
#define DIGIT3 PIN_8
#define DIGIT4 PIN_6

// below are listed masks for different outputs. use them to experiment

// putting all pins togrther with bitwise "or"
const uint32_t SEGM_USED_PINS = DIOD_E | DIOD_D | DIOD_DP | DIOD_C | DIOD_G | DIGIT4 | DIOD_B |
                                DIGIT3 | DIGIT2 | DIOD_F | DIOD_A | DIGIT1;

const unsigned int SEGM_LETTERS[] = {
    DIOD_A | DIOD_B | DIOD_C | DIOD_E | DIOD_F | DIOD_G,          // A
    DIOD_A | DIOD_B | DIOD_C | DIOD_D | DIOD_E | DIOD_F | DIOD_G, // B
    DIOD_A | DIOD_D | DIOD_E | DIOD_F,                            // C
    DIOD_A | DIOD_B | DIOD_C | DIOD_D | DIOD_E | DIOD_F,          // D
    DIOD_A | DIOD_D | DIOD_E | DIOD_F | DIOD_G,                   // E
    DIOD_A | DIOD_E | DIOD_F | DIOD_G,                            // F
    DIOD_A | DIOD_C | DIOD_D | DIOD_E | DIOD_F,                   // G
    DIOD_B | DIOD_C | DIOD_E | DIOD_F | DIOD_G,                   // H
    DIOD_B | DIOD_C,                                              // I
    DIOD_B | DIOD_C | DIOD_D,                                     // J
    DIOD_A | DIOD_C | DIOD_E | DIOD_F | DIOD_G,                   // K
    DIOD_D | DIOD_E | DIOD_F,                                     // L
    DIOD_B | DIOD_C | DIOD_E | DIOD_F | DIOD_G,                   // M
    DIOD_A | DIOD_B | DIOD_C | DIOD_E | DIOD_F,                   // N
    DIOD_A | DIOD_B | DIOD_C | DIOD_D | DIOD_E | DIOD_F,          // O
    DIOD_A | DIOD_B | DIOD_E | DIOD_F | DIOD_G,                   // P
    DIOD_A | DIOD_B | DIOD_C | DIOD_F | DIOD_G,                   // Q
    DIOD_A | DIOD_B | DIOD_C | DIOD_E | DIOD_F | DIOD_G,          // R
    DIOD_A | DIOD_C | DIOD_D | DIOD_F | DIOD_G,                   // S
    DIOD_A | DIOD_B | DIOD_C,                                     // T
    DIOD_B | DIOD_C | DIOD_D | DIOD_E | DIOD_F,                   // U
    DIOD_A | DIOD_C | DIOD_D | DIOD_E | DIOD_F,                   // V
    DIOD_A | DIOD_C | DIOD_D | DIOD_E | DIOD_F,                   // W
    DIOD_B | DIOD_C | DIOD_E | DIOD_F | DIOD_G,                   // X
    DIOD_B | DIOD_C | DIOD_D | DIOD_F | DIOD_G,                   // Y
    DIOD_A | DIOD_B | DIOD_D | DIOD_E | DIOD_G                    // Z
};

const uint32_t SEGM_SYMBOLS[] = {
    DIOD_G,                            // -
    DIOD_DP,                           // .
    DIOD_D,                            // _
    DIOD_A | DIOD_D | DIOD_E | DIOD_F, // [
    DIOD_A | DIOD_B | DIOD_C | DIOD_D, // ]
};

const unsigned int SEGM_NUMBERS[] = {
    DIOD_A | DIOD_B | DIOD_C | DIOD_D | DIOD_E | DIOD_F,          // 0
    DIOD_B | DIOD_C,                                              // 1
    DIOD_A | DIOD_B | DIOD_G | DIOD_E | DIOD_D,                   // 2
    DIOD_A | DIOD_B | DIOD_G | DIOD_C | DIOD_D,                   // 3
    DIOD_F | DIOD_G | DIOD_B | DIOD_C,                            // 4
    DIOD_A | DIOD_F | DIOD_G | DIOD_C | DIOD_D,                   // 5
    DIOD_A | DIOD_F | DIOD_G | DIOD_C | DIOD_D | DIOD_E,          // 6
    DIOD_A | DIOD_B | DIOD_C,                                     // 7
    DIOD_A | DIOD_B | DIOD_F | DIOD_C | DIOD_D | DIOD_E | DIOD_G, // 8
    DIOD_A | DIOD_G | DIOD_B | DIOD_C | DIOD_D | DIOD_F           // 9
};

const unsigned int SEGM_QUARTERS[] = {
    DIGIT3 | DIGIT2 | DIGIT1, // only 4th digit (unitary)
    DIGIT4 | DIGIT2 | DIGIT1, // only 3d digit (decimals)
    DIGIT3 | DIGIT1 | DIGIT4, // only 2nd digit (centenials)
    DIGIT3 | DIGIT2 | DIGIT4  // only 1st digit (millenials)
};

// i pre-defined pins from 1 to 12 to be pins of 7 segm, so user only chooses port. maybe will
// change in further implementations
void SetSegm(GPIO_TypeDef* port) {
  // enabling clocking on given port
  // see ../config/config.g for details and implementation
  PortX_EnableClock(port);

  LL_GPIO_SetPinMode(port, DIGIT1, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIGIT2, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIGIT3, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIGIT4, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_A, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_B, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_C, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_D, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_E, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_F, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_G, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinMode(port, DIOD_DP, LL_GPIO_MODE_OUTPUT);
}

const uint32_t div[4] = {1, 10, 100, 1000};

// funtion that uses external ticking as source
void Segm_SetNum2(GPIO_TypeDef* port, uint32_t num, uint8_t tick) {
  // reading port state in order to conserve it later
  uint32_t port_state = LL_GPIO_ReadOutputPort(GPIOC);

  // as we have only 4 numbers on display, we get %4 of tick
  uint8_t tick_cur = tick % 4;

  // getting tick%4 th digit in number
  uint32_t num_cur = (num / div[tick_cur]) % 10;

  // now i'll explain this line
  // let port state be 10011010 (really, it's 32-bit unsigned value, but 8 bit will work as
  // explanation)
  //
  // when we do port_state & ~SEGM_USED_PINS, we set all pins that are allowed to be changed to zero
  // so we get the image that we want
  //
  // next, we do  | SEGM_NUMBERS[num_cur] - set necessary bits of the number as we want them
  //
  // | SEGM_QUARTERS[tick_cur] - we leave empty the digit we want to display (setting 1 on other
  // ones means that there will be dU = 0 (see circuit scheme)).
  //
  // thus, we get needed number displayed
  port_state = (port_state & ~SEGM_USED_PINS) | SEGM_NUMBERS[num_cur] | SEGM_QUARTERS[tick_cur];

  // writing new port state to the port
  LL_GPIO_WriteOutputPort(port, port_state);
}

#endif