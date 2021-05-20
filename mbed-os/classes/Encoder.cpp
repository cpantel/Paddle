#include "Encoder.h"
#include "arm_math.h"

void Encoder::clkUp(){
   clk = true;
   process();
}
void Encoder::clkDown(){
   clk = false;
   process();
}
void Encoder::dtUp(){
   dt = true;
   process();
}
void Encoder::dtDown(){
   dt = false;
   process();
}

void Encoder::moveCCW(){
   steps -= 1;
}

void Encoder::moveCW(){
   steps += 1;
}

void Encoder::dontMove(){

}

void Encoder::itsAMistake(){

}

void Encoder::process() {
   StateName nextState;

   uint16_t  last_steps  = steps;

   led2 = clk;
   led3 = dt;

   if (clk && dt ) {
      nextState = StateName::ONE_ONE;
   } else if ( clk && ! dt ) {
      nextState = StateName::ZERO_ONE;
   } else if ( ! clk && dt ) {
      nextState = StateName::ONE_ZERO;
   } else if ( !clk && ! dt ) {
      nextState = StateName::ZERO_ZERO;
   } else {
      itsAMistake();
   }

   switch (state) {
      case StateName::ZERO_ZERO:
         switch (nextState) {
            case StateName::ZERO_ONE: moveCCW(); break;
            case StateName::ONE_ZERO: moveCW(); break;
            case StateName::ONE_ONE: dontMove(); break;
            default: itsAMistake(); break;
         }
      break;
      case StateName::ZERO_ONE:
         switch (nextState) {
            case StateName::ZERO_ZERO: moveCW(); break;
            case StateName::ONE_ZERO: dontMove(); break;
            case StateName::ONE_ONE: moveCCW(); break;
            default: itsAMistake(); break;
         }
      break;
      case StateName::ONE_ZERO:
         switch (nextState) {
            case StateName::ZERO_ZERO: moveCCW(); break;
            case StateName::ZERO_ONE: dontMove(); break;
            case StateName::ONE_ONE: moveCW(); break;
            default: itsAMistake(); break;
         }
      break;

      case StateName::ONE_ONE:
         switch (nextState) {
            case StateName::ZERO_ZERO: dontMove(); break;
            case StateName::ZERO_ONE: moveCW(); break;
            case StateName::ONE_ZERO: moveCCW(); break;
            default: itsAMistake(); break;
         }
      break;
      default: itsAMistake(); break;
   }

   state = nextState;


/*
import math
PI = math.pi
DOCE_GRADOS = float(12)
TRESCIENTOS_SESENTA_GRADOS = 360
TRESCIENTOS_SESENTA_GRADOS_EN_RADIANES = 2 * PI
print("Doce grados en radianes es {}".format(DOCE_GRADOS/TRESCIENTOS_SESENTA_GRADOS*TRESCIENTOS_SESENTA_GRADOS_EN_RADIANES))
*/
   // radius = analogRead;
   if (last_steps != steps) {
     int32_t  last_x =   x;
     int32_t  last_y =   y;

     float32_t radian = (steps * 6.0 / 360.0 ) * ( 2.0 *  3.14159);
     //float32_t radian = steps * 12 * 3.14/180.0;

     x = cos(radian)*radius; //
     y = sin(radian)*radius;


     mouse->move(x -last_x, y - last_y);


   }
}
