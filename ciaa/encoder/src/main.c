/** Copyright Carlos Pantelides 2016
 **
 ** 
 **
 **/


#include "main.h"

#include "sAPI.h" 

uint8_t * itoa(int number, uint8_t buffer[], const int base) {

   static uint8_t map[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVW";
   int size = 32;
   int remainder;
   int error = 0;
   int idx;
   int idy = 0;

   // check buffer length is 33 and zero it
   for ( idx = 0; idx < ( size + 1) ; idx++) {
       if (buffer[idx] == 0) {
          error = 1;
          break;
       }
       buffer[idx] = 0;
   }
   // check buffer is zero terminated
   if (buffer[size + 1] != 0) {
      error = 2;
   }

   // check base between 2 and 32
   if ( base < 2 || base > size ) {
      error = 3;
   }

   if ( error != 0) {
      buffer[0] = "e\0";
      return buffer;
   }

   if ( number == 0 ) {
      buffer[0] = '0';
      buffer[1] = 0;
      return buffer;
   }

   if ( number < 0 ) {
      buffer[0]='-';
      idy = 1;
      number = -number;
   }
   idx = size - 1;
   while (number > 0) {
      remainder = number % base;
      buffer[idx] = map[remainder];
      idx--;
      number = (number - remainder ) / base;
   }
   idx++;
   for ( ; idx < 34; idx++, idy++) {
       buffer[idy] = buffer[idx];
   }
   return buffer;
}

typedef enum {ZERO_ZERO=0, ZERO_ONE=1, ONE_ZERO=2, ONE_ONE=3} STATES;

uint8_t msgI[] = "starting...\n\0";

uint8_t msgE[]   = "error\n\0";

uint8_t msgTR[] = "R\n\0";
uint8_t msgTL[] = "L\n\0";
uint8_t msgU[]  = "U\n\n";

int leftCount  = 0;
int rightCount = 0;
int skipCount  = 0;
int errorCount = 0;

struct record {
    uint8_t state;
    uint8_t nextState;
    uint8_t action;
};
   
uint8_t idx = 0;

struct record  history[20];

STATES state;

STATES nextState;

void sayLeft() {
   leftCount++;
   history[idx].action='L';
}

void sayRight() {
   rightCount++;
   history[idx].action='R';
}

void saySkip() {
   skipCount++;
   history[idx].action='S';
}

void sayError() {
   errorCount++;
   uartWriteString(UART_USB,msgE);
   
}

void report() {
   uint8_t buffer1[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVW";
   itoa(leftCount, buffer1, 10);
   uartWriteString(UART_USB,"left: \0");
   uartWriteString(UART_USB,buffer1);

   uint8_t buffer2[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVW";
   itoa(skipCount, buffer2, 10);
   uartWriteString(UART_USB," skip: \0");
   uartWriteString(UART_USB,buffer2);

   uint8_t buffer4[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVW";
   itoa(errorCount, buffer4, 10);
   uartWriteString(UART_USB," error: \0");
   uartWriteString(UART_USB,buffer4);

   uint8_t buffer5[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVW";
   itoa(rightCount, buffer5, 10);
   uartWriteString(UART_USB," right: \0");
   uartWriteString(UART_USB,buffer5);
   uartWriteString(UART_USB,"\n\0");
   leftCount = 0;
   rightCount = 0;
//   skipCount = 0;
}

void reportOps() {
   uint8_t count = 0;
   for (; count < idx ; count++) {
       uint8_t msgAction[] = "     \0\n";
       uint8_t buffer1[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVW";
       uint8_t buffer2[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVW";
       itoa(history[count].state,buffer1,10);
       itoa(history[count].nextState,buffer2,10);

       msgAction[1] = history[count].action;

       uartWriteString(UART_USB,"state: \0");
       uartWriteString(UART_USB,buffer1);
       uartWriteString(UART_USB," nextState: \0");
       uartWriteString(UART_USB,buffer2);
       uartWriteString(UART_USB,msgAction);
       uartWriteString(UART_USB,"\n\0");
   }
   uartWriteString(UART_USB,"\n\n\0");
}

int main(void)
{
   boardConfig();

   digitalConfig( 0, ENABLE_DIGITAL_IO );

   digitalConfig( DIO3, INPUT );
   digitalConfig( DIO4, INPUT );

   uartConfig(UART_USB,115200);

   state = ZERO_ZERO;

   uartWriteString(UART_USB,msgI);

   int reportCount = 0;
   while(1) {
      if (reportCount++ > 500000) {
         report();
         reportCount = 0;
      }

      uint8_t dt = digitalRead(DIO3);
      uint8_t clk  = digitalRead(DIO4);

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

      //nextState = ( clk != 0 )  + (2 * ( dt != 0));

      if (nextState == state) continue;

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

      history[idx].state = state;
      history[idx].nextState = nextState;      

      if (idx++ > 18 ) {
         reportOps();
         idx = 0;
      }


      state = nextState;
   }
   return 0;
}
