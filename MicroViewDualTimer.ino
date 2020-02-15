//  Prior Art:
//    https://learn.sparkfun.com/tutorials/microview-hookup-guide/microview-library-installation
//    https://www.sunfounder.com/learn/Super-Kit-V2-0-for-Arduino/lesson-14-rotary-encoder-super-kit.html

// * This program is free software; you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation; either version 3 of the License, or
// * (at your option) any later version.

#include <MicroView.h>

 #define pulsePin1 1
 #define pulsePin2 2
 #define encoderSw 3     
 #define encoderClock 5
 #define encoderDt 6
 
 int lastEncoderSwState,calculatedInterval1,calculatedInterval2;
 unsigned long previousMillis1 = 0;
 unsigned long previousMillis2 = 0;
 unsigned long currentMillis;
 int pulseWidth = 50; //mS
 int counter1 = 100; 
 int counter2 = 100;
 String activeCounter = "top";
 void setup() { 
  pinMode (encoderClock,INPUT);
  pinMode (encoderDt,INPUT);
  pinMode (encoderSw, INPUT_PULLUP);

  uView.begin();        // start MicroView  
  uView.clear(PAGE);
  uView.setCursor(0,0);
  uView.setFontType(2);
  uView.print(counter1);
  uView.print(".");
  uView.setCursor(0,25);
  uView.print(counter2);
  uView.display();
  digitalWrite(pulsePin1, LOW);
  
 }


void loop() {
  if ((digitalRead(encoderSw) == LOW)&&(lastEncoderSwState == HIGH)){
      if (activeCounter=="top"){activeCounter="bottom";}else{activeCounter="top";}
    }
   lastEncoderSwState = digitalRead(encoderSw);
   int change = getEncoderTurn();
   if (activeCounter == "top"){
    counter1 = ((counter1<1000)&&(counter1>0))?counter1+change:counter1;
   }else{
    counter2 = (counter2<1000)?counter2+change:counter2;
   }


  uView.clear(PAGE);
  uView.setCursor(0,0);
  uView.setFontType(2);
  uView.print(counter1);
  if (activeCounter == "top"){uView.print(".");}else{uView.print("   ");}
  uView.setCursor(0,25);
  uView.print(counter2);
  if (activeCounter == "bottom"){uView.print(".");}else{uView.print("   ");}
  uView.display();
    
   //first clock routine
   currentMillis = millis();
   //calculatedInterval = (60/counter1)*1000;
   calculatedInterval1 = (600/counter1)*50;
   if(
      (currentMillis - previousMillis1 >= calculatedInterval1)
         &&
      (currentMillis - previousMillis1 <= (2*calculatedInterval1))
      ){
       digitalWrite(pulsePin1,HIGH);
     }else{
      digitalWrite(pulsePin1, LOW);
     }
     if(currentMillis - previousMillis1 >= (2*calculatedInterval1)){
      previousMillis1 = currentMillis;
     }

   //second clock routine
   currentMillis = millis();
   //calculatedInterval = (60/counter1)*1000;
   calculatedInterval2 = (600/counter2)*50;
   if(
      (currentMillis - previousMillis2 >= calculatedInterval2)
         &&
      (currentMillis - previousMillis2 <= (2*calculatedInterval2))
      ){
       digitalWrite(pulsePin2,HIGH);
     }else{
      digitalWrite(pulsePin2, LOW);
     }
     if(currentMillis - previousMillis2 >= (2*calculatedInterval2)){
      previousMillis2 = currentMillis;
     }
     
}

int getEncoderTurn(void){
  static int oldA = HIGH; //set the oldA as HIGH
  static int oldB = HIGH; //set the oldB as HIGH
  int result = 0;
  int newA = digitalRead(encoderClock);//read the value of clkPin to newA
  int newB = digitalRead(encoderDt);//read the value of dtPin to newB
  if (newA != oldA || newB != oldB) //if the value of clkPin or the dtPin has changed
  {
  // something has changed
    if (oldA == HIGH && newA == LOW)
      {
      result = (oldB * 2 - 1);
      }
  }
  oldA = newA;
  oldB = newB;
  return result;
  }
