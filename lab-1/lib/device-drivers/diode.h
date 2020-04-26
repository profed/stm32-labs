#ifndef __DIODE_H_INCLUDED__
#define __DIODE_H_INCLUDED__

#include "../config/config.h"

int SetDiode(GPIO_TypeDef* port, unsigned int pin) {
  // enabling clocking on given port
  // see lib/config/config.g for details and implementation
  PortX_EnableClock(port);

  // setting given pin as output pin
  LL_GPIO_SetPinMode(port, pin, LL_GPIO_MODE_OUTPUT);

  return 0;
}

#endif
