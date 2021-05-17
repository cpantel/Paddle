#ifndef _DEBOUNCER_H_
#define _DEBOUNCER_H_
#include "mbed.h"


#include "USBMouse.h"  //

class Debouncer {
public:
   Debouncer(PinName pin,EventQueue * queue): interrupt(pin), queue(queue) {
      interrupt.rise(callback(this, &Debouncer::rise));
      interrupt.fall(callback(this, &Debouncer::fall));
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

#endif // _DEBOUNCER_H_
