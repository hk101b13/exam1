#include "mbed.h"
#include "uLCD_4DGL.h"

const double pi = 3.141592653589793238462;
const double amplitude = 0.5f;
const double offset = 65535 / 2;

DigitalOut myled(LED3);
InterruptIn up(A0);
InterruptIn down(A1);
InterruptIn sel(A4);
AnalogIn Ain(A3);

AnalogOut Aout(PA_4);
AnalogOut Sout(PA_5);
uLCD_4DGL uLCD(D1, D0, D2);
EventQueue queue(32 * EVENTS_EVENT_SIZE);

float period = 1;
int sample = 10000;
float ADCdata[10000];
float F;

Thread t1;
//Thread t2;

int rate = 8;
float rate_out = 1/8;



void up_fun(){
   if(up){
          if(rate == 8){
                uLCD.cls() ;
                uLCD.locate(5, 2);
                uLCD.printf("1/4");
                uLCD.set_font(FONT_7X8);
                rate =4;              
          }
          else if(rate == 4){
                uLCD.cls() ;
                uLCD.locate(5, 2);
                uLCD.printf("1/2");
                uLCD.set_font(FONT_7X8);
                rate =2;                                   
          }
          else if(rate == 2){
                uLCD.cls() ;
                uLCD.locate(5, 2);
                uLCD.printf("1");
                uLCD.set_font(FONT_7X8);   
                rate = 1;         
          }
          else if(rate  == 1){
                uLCD.cls() ;
                rate = 1;
                uLCD.locate(5, 2);
                uLCD.printf("1");
                uLCD.set_font(FONT_7X8);             
          }
   }
}

void ISR1(){
  queue.call(up_fun);
}

void down_fun(){
   if(down){
          if(rate == 1){
                uLCD.cls() ;
                uLCD.locate(5, 2);
                uLCD.printf("1/2");
                uLCD.set_font(FONT_7X8);
                rate = 2;              
          }
          else if(rate == 2){
                uLCD.cls() ;
                uLCD.locate(5, 2);
                uLCD.printf("1/4");
                uLCD.set_font(FONT_7X8);
                rate = 4;             
          }
          else if(rate == 4){
                uLCD.cls() ;
                uLCD.locate(5, 2);
                uLCD.printf("1/8");
                uLCD.set_font(FONT_7X8); 
                rate =8;            
          }
          else if(rate  == 8){
                uLCD.cls() ;
                uLCD.locate(5, 2);
                uLCD.printf("1/8");
                uLCD.set_font(FONT_7X8);
                rate =8;             
          }
    }
}

void ISR2(){
  queue.call(down_fun);
}




void select_rates(){
   //uLCD.background_color(0xFF0000);
   if(sel){
            rate_out = 1/rate;
            //period = 1 / freq *100000;
           // t2.start(&Output);
   }
}

void ISR3(){
  queue.call(select_rates);
}



int main() {
    Aout = 1;
    Sout = 1;
    t1.start(callback(&queue, &EventQueue::dispatch_forever));
    up.rise(&ISR1);
    down.rise(&ISR2);
    sel.rise(&ISR3);
}