#include "MouseClick.h"
#include "arm_book_lib.h"
void MouseClick::press(){
   mouse->press(button);
   led1 = ON;
}
void MouseClick::release(){
   mouse->release(button);
   led1 = OFF;
}
