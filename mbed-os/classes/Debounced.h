#ifndef _DEBOUNCED_H_
#define _DEBOUNCED_H_
#include "mbed.h"
class Debounced {
public:
   Debounced(PinName pin): interrupt(pin) {
      interrupt.rise(callback(this, &Debounced::rise));
      interrupt.fall(callback(this, &Debounced::fall));
   }
   void rise();
   void fall();
   void checkRise();
   void checkFall();
   int read();
   bool isPressed();
   bool isReleased();


private:
   InterruptIn interrupt;
   PinName pin;
   Timeout riseDelay;
   Timeout fallDelay;
   bool rising = false;
   bool falling = false;
   bool risen = false;
   bool fallen = false;

};

#endif // _DEBOUNCED_H_
