#ifndef _DEBOUNCER_H_
#define _DEBOUNCER_H_
#include "mbed.h"
#include "arm_book_lib.h"

template <typename T>
class Debouncer{
public:
   Debouncer(PinName pin,EventQueue * theQueue, T* theObject, void(T::*theRiseFunction)(), void(T::*theFallFunction)(), float theDebounceDelay = 0.040 )
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
   T * object;

   void (T::*riseFunction)();
   void (T::*fallFunction)();
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

#include "Debouncer.h"

template <typename T>
void Debouncer<T>::notifyRise() {
   (object->*riseFunction)();
   led3 = ON;
}

template <typename T>
void Debouncer<T>::notifyFall() {
   (object->*fallFunction)();
   led3 = OFF;
}

template <typename T>
void Debouncer<T>::checkRise() {
   riseDelay.detach();
   risen = interrupt.read();
   if (risen) {
      led2 = ON;
      fallen = false;
      queue->call(callback(this,&Debouncer<T>::notifyRise));
   }
   rising = false;
}

template <typename T>
void Debouncer<T>::checkFall() {
   fallDelay.detach();
   fallen = ! interrupt.read();
   if (fallen) {
      led2 = OFF;
      risen = false;
      queue->call(callback(this,&Debouncer<T>::notifyFall));
   }
   falling = false;
}

template <typename T>
void Debouncer<T>::rise() {
   if (! rising) {
      led1= ON;
      riseDelay.attach(callback(this,&Debouncer<T>::checkRise), 0.040);
      rising = true;
   }
}

template <typename T>
void Debouncer<T>::fall() {
   if (! falling) {
      led1 = OFF;
      fallDelay.attach(callback(this,&Debouncer<T>::checkFall), 0.040);
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
