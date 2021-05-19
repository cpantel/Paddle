#include "Debouncer.h"

void Debouncer::notifyRise() {
   (object->*riseFunction)();
   led3 = ON;
}

void Debouncer::notifyFall() {
   (object->*fallFunction)();
   led3 = OFF;
}

void Debouncer::checkRise() {
   riseDelay.detach();
   risen = interrupt.read();
   if (risen) {
      led2 = ON;
      fallen = false;
      queue->call(callback(this,&Debouncer::notifyRise));
   }
   rising = false;
}

void Debouncer::checkFall() {
   fallDelay.detach();
   fallen = ! interrupt.read();
   if (fallen) {
      led2 = OFF;
      risen = false;
      queue->call(callback(this,&Debouncer::notifyFall));
   }
   falling = false;
}

void Debouncer::rise() {
   if (! rising) {
      led1= ON;
      riseDelay.attach(callback(this,&Debouncer::checkRise), 0.040);
      rising = true;
   }
}

void Debouncer::fall() {
   if (! falling) {
      led1 = OFF;
      fallDelay.attach(callback(this,&Debouncer::checkFall), 0.040);
      falling = true;
   }
}

int Debouncer::read() {
   return interrupt.read();
}

bool Debouncer::isPressed() {
   bool t = risen;
   risen = false;
   return t;
}

bool Debouncer::isReleased() {
   bool t = fallen;
   fallen = false;
   return t;
}

