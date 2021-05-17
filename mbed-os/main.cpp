#include "mbed.h"
#include "USBMouse.h"
#include "arm_book_lib.h"
#include "Debounced.h"



Serial uartUsb(USBTX, USBRX);
DigitalOut led(LED1);






int main() {
    uartUsb.printf("\r\nSlither USB version 0.0.4\r\n");
/*    int16_t x = 0;
    int16_t y = 0;
    int32_t radius = 4;
    int32_t angle = 0;
    DigitalOut led(LED1);*/

    EventQueue queue;

    Debounced d(BUTTON1, &queue);


    Thread eventThread;
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

    wait(osWaitForever);

/*
    int skip = 0;
    while (1) {
        // radius = analogRead;
        // angle = encoderPosition;
        // mouse.press(1);
        // mouse.release(1);
        angle += 1;
        if (angle > 360) {
            angle = 0;
        } else if (angle < 0) {
            angle = 360 + angle;
        }

        x = cos((double)angle*3.14/180.0)*radius;
        y = sin((double)angle*3.14/180.0)*radius;

        mouse.move(x, y);

        delay(100);

           uartUsb.printf( "x: %d y: %d\r\n",x,y);
    }
*/
}
