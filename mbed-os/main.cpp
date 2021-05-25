#include "mbed.h"
#include "USBMouse.h"
#include "arm_book_lib.h"
#include "Debouncer.h"
#include "Encoder.h"
#include "MouseClick.h"



int main() {
    Serial uartUsb(USBTX, USBRX);

    uartUsb.printf("\r\nSlither USB version 0.1.40\r\n");

    EventQueue interruptQueue;
    Thread     outOfISRThread;

    EventQueue outputActivityQueue;
    Thread     outputActivityThread;
    Ticker     ticker;
    Semaphore  semaphore(1);

    uartUsb.printf("Waiting for USB connection...");

    USBMouse mouse;

    uartUsb.printf(" connected!\r\n");

   // MouseClick leftButton(&mouse, MOUSE_LEFT);
   // Encoder encoder(&mouse);
    Encoder encoder(&mouse, &outputActivityQueue, &semaphore);

   // Debouncer<MouseClick> leftButtonDebouncer(BUTTON1, &interruptQueue, &leftButton, &MouseClick::press, &MouseClick::release, 0.10); // BUTTON1/ PC_10
    Debouncer<Encoder> clkEncoderDebouncer(PC_8, &interruptQueue, &encoder, &Encoder::clkUp, &Encoder::clkDown, 0.02);  //PC_8
    Debouncer<Encoder> dtEncoderDebouncer(PC_9, &interruptQueue, &encoder, &Encoder::dtUp, &Encoder::dtDown, 0.02);     //PC_9
    encoder.setSerial(&uartUsb);

    //outOfISRThread.start(callback(&interruptQueue, &EventQueue::dispatch_forever));
    ticker.attach(callback(&encoder,&Encoder::collect),0.1);

    outputActivityThread.start(callback(&outputActivityQueue, &EventQueue::dispatch_forever));

    wait(osWaitForever);

}
