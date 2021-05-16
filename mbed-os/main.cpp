#include "mbed.h"
#include "USBMouse.h"
#include "arm_book_lib.h"


//USBMouse mouse;

Serial uartUsb(USBTX, USBRX);
DigitalOut led(LED1);

// https://os.mbed.com/docs/mbed-os/v6.10/apis/timeout.html
InterruptIn pin(BUTTON1);
Timeout riseDelay;
Timeout fallDelay;


bool rising = false;
bool falling = false;
bool risen = false;
bool fallen = false;


void checkRise() {
   risen = pin.read();
   rising = false;
   riseDelay.detach();
}

void checkFall() {
   fallen = ! pin.read();
   falling = false;
   fallDelay.detach();
}


void rise() {
   if (! rising) {
      riseDelay.attach(&checkRise, 0.040);
      rising = true;
   }
}

void fall() {
   if (! falling) {
      fallDelay.attach(&checkFall, 0.040);
      falling = true;
   }
}


int main() {
    uartUsb.printf("\r\nSlither USB version 0.0.3\r\n");
/*    int16_t x = 0;
    int16_t y = 0;
    int32_t radius = 4;
    int32_t angle = 0;
    DigitalOut led(LED1);*/

    pin.fall(&fall);
    pin.rise(&rise);
    while(1) {
       if (fallen) {
          uartUsb.printf("button release\r\n");
          ThisThread::sleep_for(0.040);
          fallen = false;
       }
       if (risen) {
          uartUsb.printf("button press\r\n");
          ThisThread::sleep_for(0.040);
          risen = false;
       }
       led = pin.read();

    }
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
