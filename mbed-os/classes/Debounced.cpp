#include "Debounced.h"

void Debounced::notifyRise() {
   mouse.press(MOUSE_LEFT);
}


void Debounced::notifyFall() {
   mouse.release(MOUSE_LEFT);
}

void Debounced::checkRise() {
   riseDelay.detach();
   risen = interrupt.read();
   if (risen) {
      fallen = false;
      queue->call(callback(this,&Debounced::notifyRise));
   }
   rising = false;
}

void Debounced::checkFall() {
   fallDelay.detach();
   fallen = ! interrupt.read();
   if (fallen) {
      risen = false;
      queue->call(callback(this,&Debounced::notifyFall));
   }
   falling = false;
}

void Debounced::rise() {
   if (! rising) {
      riseDelay.attach(callback(this,&Debounced::checkRise), 0.040);
      rising = true;
   }
}

void Debounced::fall() {
   if (! falling) {
      fallDelay.attach(callback(this,&Debounced::checkFall), 0.040);
      falling = true;
   }
}

int Debounced::read() {
   return interrupt.read();
}

bool Debounced::isPressed() {
   bool t = risen;
   risen = false;
   return t;
}

bool Debounced::isReleased() {
   bool t = fallen;
   fallen = false;
   return t;
}

