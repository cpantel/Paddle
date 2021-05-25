#include "mbed.h"
#include "USBMouse.h"
#include "arm_book_lib.h"
#include "Debouncer.h"
#include "Encoder.h"
#include "MouseClick.h"


#define BOUNCE_DELAY      0.005
#define COLLECT_INTERVAL  0.1

int main() {
    Serial uartUsb(USBTX, USBRX);

    uartUsb.printf("\r\nUSB Paddle version 0.1.47\r\n");

    EventQueue lookupQueue;
    Thread     lookupThread;

    Ticker     collectTicker;

    EventQueue emitQueue;
    Thread     emitThread;

    Semaphore  semaphore(1);

    uartUsb.printf("Waiting for USB connection...");

    USBMouse mouse;

    uartUsb.printf(" connected!\r\n");

 // MouseClick leftButton(&mouse, MOUSE_LEFT);

    Encoder    encoder   (&mouse, &emitQueue, &lookupQueue, &semaphore);
    encoder.setSerial(&uartUsb);

 // Debouncer<MouseClick> leftButtonDebouncer(BUTTON1, &leftButton, &MouseClick::press, &MouseClick::release, 0.10); // BUTTON1/ PC_10
    Debouncer<Encoder>    clkEncoderDebouncer(PC_8,    &encoder,    &Encoder::clkUp,    &Encoder::clkDown,    BOUNCE_DELAY); // PC_8
    Debouncer<Encoder>    dtEncoderDebouncer (PC_9,    &encoder,    &Encoder::dtUp,     &Encoder::dtDown,     BOUNCE_DELAY); // PC_9


    collectTicker.attach(callback(&encoder,&Encoder::collect),COLLECT_INTERVAL);

    emitThread.start  (callback(&emitQueue,   &EventQueue::dispatch_forever));
    lookupThread.start(callback(&lookupQueue, &EventQueue::dispatch_forever));

    wait(osWaitForever);

}
