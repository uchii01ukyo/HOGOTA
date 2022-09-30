/* 
 * Temp & Humi
 * Library: https://github.com/adafruit/DHT-sensor-library/blob/master/DHT.h
*/
#include <Wire.h>
#include "rgb_lcd.h" 
rgb_lcd lcd;

#include "DHT.h"  
#define DHTPIN 3  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define AVERAGENUM 10

void setup() {
  Serial.begin(115200);
  Wire.begin();
  lcd.begin(16, 2);
  dht.begin();
}

void loop() {
  int cnt1 = 0;
  float averaged_H = 0; //Humi
  float averaged_T = 0; //Temp
  
  for(cnt1=0; cnt1<AVERAGENUM; cnt1++){
    float read_H = dht.readHumidity(); 
    averaged_H = averaged_H + read_H / AVERAGENUM ;
    float read_T = dht.readTemperature(); 
    averaged_T = averaged_T + read_T / AVERAGENUM ;
  }

  //LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:      H:      ");
  lcd.setCursor(2, 0);
  lcd.print(averaged_T);
  lcd.setCursor(10, 0);
  lcd.print(averaged_H);
  delay(1000);
}
