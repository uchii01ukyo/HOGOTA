/*
 * Ultrasonic distance sensor
 * Library: https://github.com/ErickSimoes/Ultrasonic
 */

#include <Wire.h>
#include "rgb_lcd.h" 
rgb_lcd lcd;

#include "Ultrasonic.h"
Ultrasonic ultrasonic(8);

#define AVERAGENUM 10

void setup() {
  Wire.begin();
  lcd.begin(16, 2);
  Serial.begin(115200);
}


void loop() {
  int cnt1 = 0;
  float averaged_L1 = 0; //Level
  
  for(cnt1=0; cnt1<AVERAGENUM; cnt1++){ 
    float RangeInCentimeters = ultrasonic.MeasureInCentimeters(); 
    averaged_L1 = averaged_L1 + RangeInCentimeters / AVERAGENUM ;
  }

  Serial.println(averaged_L1);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(averaged_L1);
  lcd.print(" cm");
  delay(1000);
}
