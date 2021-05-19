#include "Encoder.h"
void Encoder::clkUp(){
   mouse->press(MOUSE_LEFT);
}
void Encoder::clkDown(){
   mouse->press(MOUSE_LEFT);
}
void Encoder::dtUp(){
   mouse->press(MOUSE_LEFT);
}
void Encoder::dtDown(){
   mouse->press(MOUSE_LEFT);
}

/*
   state = ZERO_ZERO;




      if (clk && dt ) {
          nextState = ONE_ONE;
      } else if ( clk && ! dt ) {
          nextState = ZERO_ONE;
      } else if ( ! clk && dt ) {
          nextState = ONE_ZERO;
      } else if ( !clk && ! dt ) {
          nextState = ZERO_ZERO;
      } else {
          sayError();
      }

      switch (state) {
         case ZERO_ZERO:
            switch (nextState) {
               case ZERO_ONE: sayLeft(); break;
               case ONE_ZERO: sayRight(); break;
               case ONE_ONE: saySkip(); break;
               default: sayError(); break;
            }
         break;
         case ZERO_ONE:
            switch (nextState) {
               case ZERO_ZERO: sayRight(); break;
               case ONE_ZERO: saySkip(); break;
               case ONE_ONE: sayLeft(); break;
               default: sayError(); break;
            }
         break;

         case ONE_ZERO:
            switch (nextState) {
               case ZERO_ZERO: sayLeft(); break;
               case ZERO_ONE: saySkip(); break;
               case ONE_ONE: sayRight(); break;
               default: sayError(); break;
            }
         break;

         case ONE_ONE:
            switch (nextState) {
               case ZERO_ZERO: saySkip(); break;
               case ZERO_ONE: sayRight(); break;
               case ONE_ZERO: sayLeft(); break;
               default: sayError(); break;
            }
         break;
         default: sayError(); break;
      }

      state = nextState;
*/
