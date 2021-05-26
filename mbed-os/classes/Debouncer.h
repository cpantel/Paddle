#ifndef _DEBOUNCER_H_
#define _DEBOUNCER_H_
#include "mbed.h"
#include "arm_book_lib.h"

template <typename T>
class Debouncer{
public:
   Debouncer(PinName pin, T* theObject, void(T::*theRiseFunction)(), void(T::*theFallFunction)(), float theDebounceDelay, EventQueue * theLookupQueue)
    : interrupt(pin),
      object(theObject),
      riseFunction(theRiseFunction),
      fallFunction(theFallFunction),
      debounceDelay(theDebounceDelay),
      lookupQueue(theLookupQueue),
      led01(D4),
      led02(D5),
      led03(D6),
      led04(D7)
   {
      interrupt.rise(callback(this, &Debouncer::rise));
      interrupt.fall(callback(this, &Debouncer::fall));
   }

   int read();
   bool isPressed();
   bool isReleased();


private:
   InterruptIn interrupt;
   T * object;

   void (T::*riseFunction)();
   void (T::*fallFunction)();
   float debounceDelay;
   EventQueue * lookupQueue;
   DigitalOut led01;
   DigitalOut led02;
   DigitalOut led03;
   DigitalOut led04;

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

#include "Debouncer.h"

template <typename T>
void Debouncer<T>::notifyRise() {
   (object->*riseFunction)();
   led03 = ON;
}

template <typename T>
void Debouncer<T>::notifyFall() {
   (object->*fallFunction)();
   led03 = OFF;
}

template <typename T>
void Debouncer<T>::checkRise() {
   //riseDelay.detach();
   risen = interrupt.read();
   if (risen) {
      fallen = false;
      lookupQueue->call(this, &Debouncer::notifyRise);
      led02 = ON;
   }
   rising = false;
}

template <typename T>
void Debouncer<T>::checkFall() {
   //fallDelay.detach();
   fallen = ! interrupt.read();
   if (fallen) {
      risen = false;
      lookupQueue->call(this, &Debouncer::notifyFall);
      led02 = OFF;
   }
   falling = false;
}

template <typename T>
void Debouncer<T>::rise() {
   if (! rising) {
      riseDelay.attach(callback(this,&Debouncer<T>::checkRise), debounceDelay);
      rising = true;
      led01 = ON;
   }
}

template <typename T>
void Debouncer<T>::fall() {
   if (! falling) {
      fallDelay.attach(callback(this,&Debouncer<T>::checkFall), debounceDelay);
      falling = true;
      led01 = OFF;
   }
}

template <typename T>
int Debouncer<T>::read() {
   return interrupt.read();
}

template <typename T>
bool Debouncer<T>::isPressed() {
   bool t = risen;
   risen = false;
   return t;
}
template <typename T>
bool Debouncer<T>::isReleased() {
   bool t = fallen;
   fallen = false;
   return t;
}


#endif // _DEBOUNCER_H_
