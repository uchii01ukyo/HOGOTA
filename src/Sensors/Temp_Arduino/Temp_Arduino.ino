/*
 * Temp Sensor
 * Library:
 * - https://github.com/milesburton/Arduino-Temperature-Control-Library
 * - https://github.com/PaulStoffregen/OneWire
 */
#include <Wire.h>
#include "rgb_lcd.h" 
rgb_lcd lcd;

#include <DallasTemperature.h>
#include <OneWire.h>
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define AVERAGENUM 10

void setup() {
  Wire.begin();
  lcd.begin(16, 2);
  Serial.begin(115200);
}

void loop() {
  int cnt1 = 0;
  float averaged_WT = 0; //WaterTemp

  for(cnt1=0; cnt1<AVERAGENUM; cnt1++){
    sensors.requestTemperatures();
    float WaterTempC = sensors.getTempCByIndex(0);
    averaged_WT = averaged_WT + WaterTempC / AVERAGENUM ;
  }

  Serial.println(averaged_WT);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(averaged_WT);
  lcd.print(" C");
  delay(1000);
}
