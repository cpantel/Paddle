#include "mbed.h"
#include "USBMouse.h"
#include "arm_book_lib.h"
#include "Debouncer.h"
#include "Encoder.h"
#include "MouseClick.h"



int main() {
    Serial uartUsb(USBTX, USBRX);

    uartUsb.printf("\r\nSlither USB version 0.1.229\r\n");

    EventQueue queue;
    Thread eventThread;

    uartUsb.printf("Waiting for USB connection...");

    USBMouse mouse;

    uartUsb.printf(" connected!\r\n");

    MouseClick leftButton(&mouse, MOUSE_LEFT);
    Encoder encoder(&mouse);

    Debouncer<MouseClick> leftButtonDebouncer(BUTTON1, &queue, &leftButton, &MouseClick::press, &MouseClick::release); // BUTTON1/ PC_10
    Debouncer<Encoder> clkEncoderDebouncer(PC_8, &queue, &encoder, &Encoder::clkUp, &Encoder::clkDown);
    Debouncer<Encoder> dtEncoderDebouncer(PC_9, &queue, &encoder, &Encoder::dtUp, &Encoder::dtDown);
    encoder.setSerial(&uartUsb);
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

/*
   #define STEPS 30
   int step = 0;
   float32_t moves[STEPS][2];
   float32_t radian =
      (( STEPS -1) * ( 360 / STEPS) / 360.0 ) * ( 2.0 *  3.14159);

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

     uartUsb.printf("Step: %d x: %.4f y: %.4f\r\n",
         step, moves[step][0], moves[step][1]);
   }

   int radius = 100;
*/
    wait(osWaitForever);
/*   while (1) {
       for (step = 0; step < (STEPS * 2); ++step) {
          mouse.move(moves[step%STEPS][0] * radius,
                     moves[step%STEPS][1] * radius);
          wait(2.0);
       }
       wait(2.0);
       for (step = ((STEPS * 2) - 1); step >= 0; --step) {
          mouse.move(moves[step%STEPS][0] * radius * -1 ,
                     moves[step%STEPS][1] * radius * -1 );
          wait(2.0);
       }
    }
*/

}
