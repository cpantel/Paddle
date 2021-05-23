#include "mbed.h"
#include "USBMouse.h"
#include "arm_book_lib.h"
#include "Debouncer.h"
#include "Encoder.h"
#include "MouseClick.h"



int main() {
    Serial uartUsb(USBTX, USBRX);

    uartUsb.printf("\r\nSlither USB version 0.1.31\r\n");

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

    wait(osWaitForever);

}
