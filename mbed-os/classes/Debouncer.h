#ifndef _DEBOUNCER_H_
#define _DEBOUNCER_H_
#include "mbed.h"


#include "USBMouse.h"  //

/*template<typename T>
class Debouncer {
public:
   Debouncer<T>(PinName pin,EventQueue * queue, T* obj, void (MouseClick::*)() riseFunction,  void (MouseClick::*)() fallFunction )
   : interrupt(pin),
     queue(queue),
     object(obj),
     riseFunction(riseFunction),
     fallFunction(fallFunction)*/



class Debouncer {
public:
   Debouncer(PinName pin,EventQueue * queue, Callback<void()> riseFunction, Callback<void()> fallFunction )
   : interrupt(pin),
     queue(queue),
     riseFunction(riseFunction),
     fallFunction(fallFunction)
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

    Callback<void()> riseFunction;
    Callback<void()> fallFunction;

  // T* object;
//   T riseFunction;
  // T fallFunction;
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
