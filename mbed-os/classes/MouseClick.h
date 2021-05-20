#ifndef _MOUSE_CLICK_
#define _MOUSE_CLICK_
#include "mbed.h"

#include "USBMouse.h"

class MouseClick {
public:
   MouseClick(USBMouse * mouse, uint8_t button)
    : mouse(mouse),
      button(button),
      led1(LED1)
   { }
   void press();
   void release();
private:
   USBMouse * mouse;
   uint8_t button;
   DigitalOut led1;

};


#endif // _MOUSE_CLICK_
