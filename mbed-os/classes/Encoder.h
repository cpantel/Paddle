#ifndef _ENCODER_
#define _ENCODER_
#include "mbed.h"
#include "arm_book_lib.h"
#include "USBMouse.h"
#include "arm_math.h"


#define STEPS 60

enum class StateName {ONE_ONE, ZERO_ONE, ONE_ZERO, ZERO_ZERO};
enum class DirectionName { CW, CCW };
class Encoder {
public:
   Encoder(USBMouse * mouse, EventQueue * theEmitQueue, EventQueue * theLookupQueue, Semaphore * theSemaphore):
      mouse(mouse),
      emitQueue(theEmitQueue),
      lookupQueue(theLookupQueue),
      semaphore(theSemaphore),
      led01(LED1),
      led02(LED2),
      led03(LED3),
      led2(D7),  // LED2 D7
      led3(D5)   // LED3 D5
   {

      state = StateName::ONE_ONE;
      clk = true;
      dt = true;
      x = 0.0;
      y = 0.0;
      radius = 150.0;
      step = 0;
      mistaken = 0;
      buildLookup();

   }
   void clkUp();
   void clkDown();
   void dtUp();
   void dtDown();
   void collect();

   void setSerial(Serial * theUsbUart) {
     usbUart = theUsbUart;
   }

private:
   USBMouse * mouse;
   EventQueue * emitQueue;
   EventQueue * lookupQueue;
   Semaphore * semaphore;
   DigitalOut led01;
   DigitalOut led02;
   DigitalOut led03;
   DigitalOut led2;
   DigitalOut led3;


   StateName state;
   DirectionName direction;
   DirectionName prevDirection;


   float32_t x;
   float32_t y;
   float32_t moves[STEPS][2];
   float32_t radius;
   int16_t step;
   int16_t prevStep;

   bool clk;
   bool dt;

   bool mistaken;

   void emit(int16_t,int16_t);
   void process();
   void moveCCW();
   void moveCW();
   void adjustStep(int delta);
   void dontMove();
   void itsAMistake();
   void buildLookup();
   void lookup();


   Serial * usbUart;

   int16_t accum_x;
   int16_t accum_y;
   int accum;


};


/*


*/

#endif // _ENCODER_
