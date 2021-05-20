#ifndef _ENCODER_
#define _ENCODER_
#include "mbed.h"
#include "USBMouse.h"

enum class StateName {ONE_ONE, ZERO_ONE, ONE_ZERO, ZERO_ZERO};

class Encoder {
public:
   Encoder(USBMouse * mouse):
      mouse(mouse),
      led2(LED2),
      led3(LED3)
   {

      state = StateName::ZERO_ZERO;
      clk = true;
      dt = true;
      x = 0;
      y = 0 ;
      radius = 100;
      steps = 0;
      delta = 18;

   }
   void clkUp();
   void clkDown();
   void dtUp();
   void dtDown();
private:
   USBMouse * mouse;
   DigitalOut led2;
   DigitalOut led3;

   StateName state;

   int32_t x;
   int32_t y;
   int32_t radius;
   int32_t steps;
   int32_t delta;
   bool clk;
   bool dt;

   void process();
   void moveCCW();
   void moveCW();
   void dontMove();
   void itsAMistake();

};


/*


*/

#endif // _ENCODER_
