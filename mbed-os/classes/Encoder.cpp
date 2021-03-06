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

void Encoder::adjustStep(int delta) {
   step += delta;
   if (step == STEPS) {
      step = 0;
   }
   if (step > STEPS) {
      step -= STEPS;
   }
   if (step < 0) {
      step += STEPS;
   }
}

void Encoder::moveCCW(){
  // if ( state == StateName::ZERO_ONE | state == StateName::ONE_ZERO ) return;
   //usbUart->printf("    moveCCW\r\n");
   prevStep = step;
   prevDirection = direction;
   adjustStep(-1);
   direction = DirectionName::CCW;
   //led2 = OFF;
   //led3 = OFF;
}

void Encoder::moveCW(){
  // if ( state == StateName::ZERO_ONE | state == StateName::ONE_ZERO ) return;
   //usbUart->printf("    moveCW\r\n");
   prevStep = step;
   prevDirection = direction;
   adjustStep(+1);
   direction = DirectionName::CW;


   //led2 = OFF;
   //led3 = OFF;
}

void Encoder::dontMove(){

}

void Encoder::itsAMistake(){
   //usbUart->printf("    itsAMistake\r\n");
  // mistaken = !mistaken;
  // led3 = mistaken;
}

void Encoder::emit(int16_t x,int16_t y) {
   if (x != 0 && y != 0)  {
      mouse->move(x, y);
//     led03.write(~led03.read());
   }
}

void Encoder::collect() {
   bool update = false;
   int16_t update_x;
   int16_t update_y;

   if ( semaphore->try_acquire() ) {
      if (accum_x != 0 && accum_y != 0 ) {
         update_x = accum_x;
         update_y = accum_y;
         accum_x = 0;
         accum_y = 0;
         update = true;
      }
      semaphore->release();
      if (update) {
         lookupQueue->call(callback(this,&Encoder::emit), update_x, update_y);
//         led01.write(~led03.read());
      }
   }
}


void Encoder::lookup(){
   //   if ( state == StateName::ZERO_ONE | state == StateName::ONE_ZERO ) return;
   float32_t fixed_radius;

   int16_t rev_x = 0;
   int16_t rev_y = 0;
   if (direction != prevDirection) {
     fixed_radius = prevDirection == DirectionName::CW ? -radius : radius;
     rev_x = moves[prevStep][0] * fixed_radius;
     rev_y = moves[prevStep][1] * fixed_radius;
   }
   fixed_radius = direction == DirectionName::CW ? radius : -radius;
   bool adquired = false;

   while ( ! adquired) {
      adquired = semaphore->try_acquire();
      if ( adquired ) {
         accum_x += moves[step][0] * fixed_radius + rev_x;
         accum_y += moves[step][1] * fixed_radius + rev_y;
         semaphore->release();
      }
   }
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

//   led2 = dt;
//   led3 = clk;


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
     lookupQueue->call(callback(this,&Encoder::lookup));
   }
}
