#include "Debounced.h"

void Debounced::checkRise() {
   risen = interrupt.read();
   rising = false;
   riseDelay.detach();
}

void Debounced::checkFall() {
   fallen = ! interrupt.read();
   falling = false;
   fallDelay.detach();
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

