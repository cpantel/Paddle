#ifndef _DEBOUNCED_H_
#define _DEBOUNCED_H_
#include "mbed.h"


#include "USBMouse.h"  //

class Debounced {
public:
   Debounced(PinName pin,EventQueue * queue): interrupt(pin), queue(queue) {
      interrupt.rise(callback(this, &Debounced::rise));
      interrupt.fall(callback(this, &Debounced::fall));
   }

   int read();
   bool isPressed();
   bool isReleased();


private:
   InterruptIn interrupt;
   EventQueue * queue;

   USBMouse mouse;                     //


   Timeout riseDelay;
   Timeout fallDelay;
   bool rising = false;
   bool falling = false;
   bool risen = false;
   bool fallen = false;
   void rise();
   void fall();
   void checkRise();
   void checkFall();
   void notifyRise();      //
   void notifyFall();      //

};

#endif // _DEBOUNCED_H_
