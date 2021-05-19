#ifndef _DEBOUNCER_H_
#define _DEBOUNCER_H_
#include "mbed.h"
#include "arm_book_lib.h"
#include "MouseClick.h"


class Debouncer{
public:
   Debouncer(PinName pin,EventQueue * theQueue, MouseClick* theObject, void(MouseClick::*theRiseFunction)(), void(MouseClick::*theFallFunction)(), float theDebounceDelay = 0.040 )
   : interrupt(pin),
     queue(theQueue),
     object(theObject),
     riseFunction(theRiseFunction),
     fallFunction(theFallFunction),
     debounceDelay(theDebounceDelay),
     led1(LED1),
     led2(LED2),
     led3(LED3)
   {
      interrupt.rise(callback(this, &Debouncer::rise));
      interrupt.fall(callback(this, &Debouncer::fall));
   }

   int read();
   bool isPressed();
   bool isReleased();


private:
   InterruptIn interrupt;
   EventQueue * queue;
   MouseClick * object;

   void (MouseClick::*riseFunction)();
   void (MouseClick::*fallFunction)();
   float debounceDelay;

   DigitalOut led1;
   DigitalOut led2;
   DigitalOut led3;

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
