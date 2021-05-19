#ifndef _ENCODER_
#define _ENCODER_

#include "USBMouse.h"

class Encoder {
public:
   Encoder(USBMouse * mouse): mouse(mouse) { }
   void clkUp();
   void clkDown();
   void dtUp();
   void dtDown();
private:
   USBMouse * mouse;
};
#endif // _ENCODER_
