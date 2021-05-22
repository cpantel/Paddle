#ifndef _ENCODER_
#define _ENCODER_
#include "mbed.h"
#include "arm_book_lib.h"
#include "USBMouse.h"
#include "arm_math.h"


#define STEPS 30

enum class StateName {ONE_ONE, ZERO_ONE, ONE_ZERO, ZERO_ZERO};
enum class DirectionName { CW, CCW };
class Encoder {
public:
   Encoder(USBMouse * mouse):
      mouse(mouse),
      led2(LED2),  // LED2 D7
      led3(LED3)   // LED3 D5
   {

      state = StateName::ONE_ONE;
      clk = true;
      dt = true;
      x = 0.0;
      y = 0.0;
      radius = 100.0;
      step = 0;
      buildLookup();

   }
   void clkUp();
   void clkDown();
   void dtUp();
   void dtDown();
   void setSerial(Serial * theUsbUart) {
     usbUart = theUsbUart;
   }
private:
   USBMouse * mouse;
   DigitalOut led2;
   DigitalOut led3;

   StateName state;
   DirectionName direction;

   float32_t x;
   float32_t y;
   float32_t moves[STEPS][2];
   float32_t radius;
   int16_t step;

   bool clk;
   bool dt;

   void process();
   void moveCCW();
   void moveCW();
   void dontMove();
   void itsAMistake();
   void jit();
   void buildLookup();
   void lookup();

   Serial * usbUart;

};


/*


*/

#endif // _ENCODER_
