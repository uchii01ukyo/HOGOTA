
#include <Arduino.h>
#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

#define PHSensor A0   //pH
#define LED 13        //LED

#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40

int pHArray[ArrayLenth];      //pH data
int pHArrayIndex=0;

void setup(void){
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
}

void loop(void){
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float phValue,phVoltage;
  
  //sampling time
  if(millis() - samplingTime > samplingInterval){
      samplingTime = millis();
      pHArray[pHArrayIndex++] = analogRead(PHSensor);
      if(pHArrayIndex == ArrayLenth) pHArrayIndex = 0;
      phVoltage = avergeArray(pHArray, ArrayLenth)*5.0/1024;
      phValue = 2.9738*phVoltage + 1.2511; //Calc
  }
  
  //print time
  if(millis() - printTime > printInterval){
    printTime = millis();
    //LCD
    digitalWrite(LED,digitalRead(LED)^1);
    lcd.setCursor(0, 0); //line1
    lcd.print("pH: ");
    lcd.print(phValue,2);
  }
}

double avergeArray(int* arr, int number){
  int i;
  long amount=0;
  double avg;

  if(number <= 0){
    Serial.println("Error number");
    return 0;
  }else{
    for(i = 0; i < number; i++){
      amount += arr[i];
    }
    avg = amount/number;
    return avg;
  }
}
