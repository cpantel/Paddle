#ifndef _DEBOUNCER_H_
#define _DEBOUNCER_H_
#include "mbed.h"
#include "arm_book_lib.h"

template <typename T>
class Debouncer{
public:
   Debouncer(PinName pin, EventQueue * theQueue, T* theObject, void(T::*theRiseFunction)(), void(T::*theFallFunction)(), float theDebounceDelay = 0.040 )
    : interrupt(pin),
      queue(theQueue),
      object(theObject),
      riseFunction(theRiseFunction),
      fallFunction(theFallFunction),
      debounceDelay(theDebounceDelay)
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
   T * object;

   void (T::*riseFunction)();
   void (T::*fallFunction)();
   float debounceDelay;

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
}

template <typename T>
void Debouncer<T>::notifyFall() {
   (object->*fallFunction)();
}

template <typename T>
void Debouncer<T>::checkRise() {
   riseDelay.detach();
   risen = interrupt.read();
   if (risen) {
      fallen = false;
      notifyRise();
//      queue->call(callback(this,&Debouncer<T>::notifyRise));
   }
   rising = false;
}

template <typename T>
void Debouncer<T>::checkFall() {
   fallDelay.detach();
   fallen = ! interrupt.read();
   if (fallen) {
      risen = false;
      notifyFall();
//      queue->call(callback(this,&Debouncer<T>::notifyFall));
   }
   falling = false;
}

template <typename T>
void Debouncer<T>::rise() {
   if (! rising) {
      riseDelay.attach(callback(this,&Debouncer<T>::checkRise), debounceDelay);
      rising = true;
   }
}

template <typename T>
void Debouncer<T>::fall() {
   if (! falling) {
      fallDelay.attach(callback(this,&Debouncer<T>::checkFall), debounceDelay);
      falling = true;
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
