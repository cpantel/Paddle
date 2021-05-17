#include "Debouncer.h"

void Debouncer::notifyRise() {
    riseFunction;
//   callback(&object,&riseFunction);
//   mouse.press(MOUSE_LEFT);
}


void Debouncer::notifyFall() {
    fallFunction;
//   callback(&object,&fallFunction);
//   mouse.release(MOUSE_LEFT);
}

void Debouncer::checkRise() {
   riseDelay.detach();
   risen = interrupt.read();
   if (risen) {
      fallen = false;
      queue->call(callback(this,&Debouncer::notifyRise));
   }
   rising = false;
}

void Debouncer::checkFall() {
   fallDelay.detach();
   fallen = ! interrupt.read();
   if (fallen) {
      risen = false;
      queue->call(callback(this,&Debouncer::notifyFall));
   }
   falling = false;
}

void Debouncer::rise() {
   if (! rising) {
      riseDelay.attach(callback(this,&Debouncer::checkRise), 0.040);
      rising = true;
   }
}

void Debouncer::fall() {
   if (! falling) {
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

