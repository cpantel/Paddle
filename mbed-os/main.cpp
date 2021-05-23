#include "mbed.h"
#include "USBMouse.h"
#include "arm_book_lib.h"

#include "arm_math.h"

#define STEPS 30
#define DELAY 0.08


    Serial uartUsb(USBTX, USBRX);
int16_t fixStep(int16_t step) {
    if (step == STEPS) {
      //uartUsb.printf("  ==  STEPS \r\n");
      return 0;
    }
    if (step > STEPS) {
      //uartUsb.printf("  >  STEPS \r\n");
      return ( step - STEPS);
    }
    if (step < 0) {
      //uartUsb.printf("  <  STEPS \r\n");
      return (step + STEPS);
    }
    return step;
}



int main() {


    uartUsb.printf("\r\nTesting usbmouse vs host\r\n");

    uartUsb.printf("Waiting for USB connection...");

    USBMouse mouse;

    uartUsb.printf(" connected!\r\n");

   float32_t moves[STEPS][2];
   float32_t radius = 100;
   int16_t step = 0;

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

   uartUsb.printf("--------------------------------------------------\r\n");
   for (step = 0; step < STEPS; ++step) {
        uartUsb.printf("step : %d delta x : %.4f delta y : %.4f\r\n", step, moves[step][0], moves[step][1]);
   }
   uartUsb.printf("--------------------------------------------------\r\n");
   int moveDuration[30] = {4,5,6,7,8,12,3,32,7,32,53,47,18,34,43,57,32,42,48,24,73,24,21,34,24,87,12,10,14,90};

   int pos = 0;
   int idx = 0;
   step = 0;
   mouse.press(MOUSE_LEFT);
   while (idx < 30)  {
      for (pos = 0; pos < moveDuration[idx]; ++pos) {
        step = fixStep(step);
        //uartUsb.printf("+ pos : %d duration: %d step : %d delta x : %.4f delta y : %.4f\r\n", pos, moveDuration[idx], step, moves[step][0], moves[step][1]);
        mouse.move(moves[step][0] * radius * 1, moves[step][1] * radius * 1);
        wait(DELAY);

        step =fixStep(step +1);
      }

        step =fixStep(step -1);
      ++idx;
      for (pos = 0;  pos < moveDuration[idx]; ++pos) {

        //uartUsb.printf("- pos : %d duration: %d step : %d delta x : %.4f delta y : %.4f\r\n", pos, moveDuration[idx], step, moves[step][0], moves[step][1]);
        mouse.move(moves[step][0] * radius * -1, moves[step][1] * radius * -1);
        wait(DELAY);


        step =fixStep(step -1);
     }

        step =fixStep(step +1);
      ++idx;
    wait(DELAY);


   }
   mouse.release(MOUSE_LEFT);
  while(1) {}




}
