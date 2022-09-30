#include <SoftwareSerial.h>

SoftwareSerial swSer(8, 7);  //Rx, Tx parts
uint8_t cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; //start measuring
uint8_t reset[9] = {0xFF,0x01,0x87,0x00,0x00,0x00,0x00,0x00,0x78}; //reset = calibration
uint8_t res[9] = {}; //data storage
uint8_t idx = 0;
bool flag = false;
uint16_t co2=0;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  swSer.begin(9600); //CO2 sensor (MH-Z14A)
  delay(100);
}

void loop() {
  swSer.write(cmd,9);
  delay(500);
  while(swSer.available()>0){
    res[idx++]=swSer.read();
    flag=true;
  }
  idx = 0;
  if(flag){
    flag=false;
    co2 = 0;
    co2 += (uint16_t)res[2] <<8;
    co2 += res[3];
  }
  Serial.println(co2);
}
