#include "Encoder.h"
#include "arm_math.h"

void Encoder::clkUp(){
   clk = true;
   //usbUart->printf("clkUp\r\n");
   process();
}
void Encoder::clkDown(){
   //usbUart->printf("clkDown\r\n");
   clk = false;
   process();
}
void Encoder::dtUp(){
   //usbUart->printf("dtUp\r\n");
   dt = true;
   process();
}
void Encoder::dtDown(){
   //usbUart->printf("dtDown\r\n");
   dt = false;
   process();
}

void Encoder::moveCCW(){
   //usbUart->printf("    moveCCW\r\n");
   step -= 1;
   if (step < 0)
      step += STEPS;
//   step = step%STEPS;
   direction = DirectionName::CCW;
   led2 = OFF;
   led3 = OFF;
}

void Encoder::moveCW(){
   //usbUart->printf("    moveCW\r\n");
   step += 1;
   step = step%STEPS;
   direction = DirectionName::CW;
   led2 = OFF;
   led3 = OFF;
}

void Encoder::dontMove(){
   //usbUart->printf("    dontMove\r\n");
   led2 = ON;
}

void Encoder::itsAMistake(){
   //usbUart->printf("    itsAMistake\r\n");
   led3 = ON;
}

void Encoder::jit(){
   float32_t  last_x =   x;
   float32_t  last_y =   y;

   float32_t radian = (step * 6.0 / 360.0 ) * ( 2.0 *  3.14159);

   x = cos(radian)*radius;
   y = sin(radian)*radius;


   mouse->move(x -last_x, y - last_y);
}

void Encoder::lookup(){
   float32_t dir = direction == DirectionName::CW ? 1.0 : -1.0;
   mouse->move(moves[step][0] * radius * dir, moves[step][1] * radius * dir);
   //usbUart->printf("      step: %d \r\n", step);
}

void Encoder::buildLookup(){
   float32_t radian = (( STEPS -1) * ( 360 / STEPS) / 360.0 ) * ( 2.0 *  3.14159);

   float32_t prev_x = cos(radian);
   float32_t prev_y = sin(radian);

   for (step = 0; step < STEPS; ++step) {
      float32_t radian = (step * ( 360 / STEPS) / 360.0 ) * ( 2.0 *  3.14159);
      float32_t next_x = cos(radian);
      float32_t next_y = sin(radian);

      moves[step][0] =  cos(radian) - prev_x;
      moves[step][1] =  sin(radian) - prev_y;
      prev_x = next_x;
      prev_y = next_y;
   }
}

void Encoder::process() {
   //usbUart->printf("  process\r\n");
   StateName nextState;

   uint16_t  last_step  = step;

   //led2 = clk;
   //led3 = dt;

//   return;

   if (clk && dt ) {
      nextState = StateName::ONE_ONE;
   } else if ( clk && ! dt ) {
      nextState = StateName::ONE_ZERO;
   } else if ( ! clk && dt ) {
      nextState = StateName::ZERO_ONE;
   } else if ( !clk && ! dt ) {
      nextState = StateName::ZERO_ZERO;
   } else {
      itsAMistake(); dontMove();
   }
/*

  0 0 CW  |
  1 0     v
  1 1     ^
  0 1 CCW |

*/
   switch (state) {
      case StateName::ZERO_ZERO:
         switch (nextState) {
            case StateName::ONE_ZERO: moveCW(); break;
            case StateName::ZERO_ONE: moveCCW(); break;
            case StateName::ONE_ONE: dontMove(); break;
            default: itsAMistake(); break;
         }
      break;

      case StateName::ONE_ZERO:
         switch (nextState) {
            case StateName::ONE_ONE: moveCW(); break;
            case StateName::ZERO_ZERO: moveCCW(); break;
            case StateName::ZERO_ONE: dontMove(); break;
            default: itsAMistake(); break;
         }
      break;

      case StateName::ONE_ONE:
         switch (nextState) {
            case StateName::ZERO_ONE: moveCW(); break;
            case StateName::ONE_ZERO: moveCCW(); break;
            case StateName::ZERO_ZERO: dontMove(); break;
            default: itsAMistake(); break;
         }
      break;

      case StateName::ZERO_ONE:
         switch (nextState) {
            case StateName::ZERO_ZERO: moveCW(); break;
            case StateName::ONE_ONE: moveCCW(); break;
            case StateName::ONE_ZERO: dontMove(); break;
            default: itsAMistake(); break;
         }
      break;


      default: itsAMistake(); break;
   }

   state = nextState;

   // radius = analogRead;
   if (last_step != step) {
     //jit();
     lookup();


   }
}
