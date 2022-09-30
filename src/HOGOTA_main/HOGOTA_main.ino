/*
 * Multiple sensor value send to Cloud via Unified Endpoint of SORACOM
 * (Sensor: Temp, Humi, WaterLevel, WaterTemp)

 * SORACOM LTE-M Shield for Arduino
 * Original Code: https://github.com/soracom-labs/arduino-dragino-unified/tree/main/send_multiple_sensor_data_with_soracom
*/

  /* for SORACOM */
  #define CONSOLE Serial
  #define INTERVAL_MS (60000)
  #define ENDPOINT "uni.soracom.io"
  #define SKETCH_NAME "send_multiple_sensor_data_with_soracom"
  #define VERSION "1.0"
  /* for LTE-M Shield for Arduino */
  #define RX 10
  #define TX 11
  #define BAUDRATE 9600
  #define BG96_RESET 15
  #define TINY_GSM_MODEM_BG96
  #include <TinyGsmClient.h>
  #include <SoftwareSerial.h>
  SoftwareSerial LTE_M_shieldUART(RX, TX);
  TinyGsm modem(LTE_M_shieldUART);
  TinyGsmClient ctx(modem);
  #include <U8x8lib.h>
  U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
  #define U8X8_ENABLE_180_DEGREE_ROTATION 1
  #define OLED_MAX_CHAR_LENGTH 16
  void drawText(U8X8* u8x8, const char* in_str, int width = OLED_MAX_CHAR_LENGTH);
  void drawText_P(U8X8* u8x8, const char* pgm_s, int width = OLED_MAX_CHAR_LENGTH);
  #define RESET_DURATION 86400000UL // 1day
  void software_reset() { asm volatile ("  jmp 0"); }
  /* i2c */
  #define AQS_ADDRESS   0x32
  #define AQS_DATA_LEN  16
  #define AQS_ID        0x43

  /* Temp & Humi */
  #include "DHT.h"  
  #define DHTPIN 3  
  #define DHTTYPE DHT22
  DHT dht(DHTPIN, DHTTYPE);
  /* WaterTemp */
  #include <DallasTemperature.h>
  #include <OneWire.h>
  #define ONE_WIRE_BUS 4
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire);
  /* Level */
  #include "Ultrasonic.h"
  Ultrasonic ultrasonic(8);
  /* LCD */
  #include <Wire.h>
  #include "rgb_lcd.h" 
  rgb_lcd lcd;
  /* SD memory */
  #include <SPI.h>
  #include <SD.h>
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  const int chipSelect = 4;
  #define AVERAGENUM 10
  /* ? */
  #define lightSensorPin A6
  #define soundSensorPin A2
  #define soundSamplingCount 3

//-----------------------------------------------
void setup() {
  CONSOLE.begin(115200);
  u8x8.begin();
  /* - */
  LTE_M_shieldUART.begin(BAUDRATE);
  u8x8.setFlipMode(U8X8_ENABLE_180_DEGREE_ROTATION);
  u8x8.setFont(u8x8_font_victoriamedium8_r);
  /* - */
  CONSOLE.print(F("Welcome to ")); CONSOLE.print(SKETCH_NAME); CONSOLE.print(F(" ")); CONSOLE.println(VERSION);
  u8x8.clear();
  drawText_P(&u8x8, PSTR("Welcome to ")); drawText(&u8x8, SKETCH_NAME); drawText_P(&u8x8, PSTR(" ")); drawText(&u8x8, VERSION);
  delay(3000);
  /* - */
  CONSOLE.print(F("resetting module..."));
  pinMode(BG96_RESET,OUTPUT);
  digitalWrite(BG96_RESET,LOW);
  delay(300);
  digitalWrite(BG96_RESET,HIGH);
  delay(300);
  digitalWrite(BG96_RESET,LOW);
  CONSOLE.println(F(" done."));
  /* - */
  u8x8.clear();
  drawText_P(&u8x8, PSTR("modem.restart()..."));
  CONSOLE.print(F("modem.restart()..."));
  modem.restart();
  drawText_P(&u8x8, PSTR("done."));
  CONSOLE.println(F(" done."));
  delay(500);
  /* - */
  u8x8.clear();
  drawText_P(&u8x8, PSTR("modem.getModemInfo():"));
  CONSOLE.print(F("modem.getModemInfo()..."));
  String modemInfo = modem.getModemInfo();
  u8x8.println();
  int modem_info_len = modemInfo.length() + 1;
  char modem_info_buf[modem_info_len];
  modemInfo.toCharArray(modem_info_buf, modem_info_len);
  drawText(&u8x8, modem_info_buf);
  CONSOLE.println(F(" done."));
  delay(2000);
  /* - */
  u8x8.clear();
  drawText_P(&u8x8, PSTR("waitForNetwork()..."));
  CONSOLE.print(F("waitForNetwork()..."));
  while (!modem.waitForNetwork());
  drawText_P(&u8x8, PSTR("Ok."));
  CONSOLE.println(F(" OK."));
  delay(500);
  /* - */
  u8x8.clear();
  drawText_P(&u8x8, PSTR("gprsConnect(soracom.io)..."));
  CONSOLE.print(F("gprsConnect(soracom.io)..."));
  modem.gprsConnect("soracom.io", "sora", "sora");
  drawText_P(&u8x8, PSTR("done."));
  CONSOLE.println(F(" done."));
  delay(500);
  /* - */
  u8x8.clear();
  drawText_P(&u8x8, PSTR("isNetworkConnected()..."));
  CONSOLE.print(F("isNetworkConnected()..."));
  while (!modem.isNetworkConnected());
  drawText_P(&u8x8, PSTR("Ok."));
  CONSOLE.println(F(" OK."));
  delay(500);
  /* - */
  u8x8.clear();
  drawText_P(&u8x8, PSTR("My IP addr: "));
  CONSOLE.print(F("My IP addr..."));
  u8x8.println();
  IPAddress ipaddr = modem.localIP();
  CONSOLE.println(ipaddr);
  char ip_addr_buf[20];
  sprintf_P(ip_addr_buf, PSTR("%d.%d.%d.%d"), ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);
  drawText(&u8x8, ip_addr_buf);
  delay(2000);
  /* - */
  Wire.begin();
  dht.begin();
  lcd.begin(16, 2);
  lcd.setRGB(0,0,0);
  Serial.begin(9600);
  while (!Serial) { }// wait for serial port to connect. Needed for native USB port only
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
}

//-------------------------------------
void loop() {
  int cnt1 = 0;
  float averaged_H = 0; //Humi
  float averaged_T = 0; //Temp
  float averaged_L1 = 0; //Level
  float averaged_WT = 0; //WaterTemp
  /* measure */
  for(cnt1=0; cnt1<AVERAGENUM; cnt1++){
    float read_H = dht.readHumidity(); 
    averaged_H = averaged_H + read_H / AVERAGENUM ;
    float read_T = dht.readTemperature(); 
    averaged_T = averaged_T + read_T / AVERAGENUM ;
    float RangeInCentimeters = ultrasonic.MeasureInCentimeters(); 
    averaged_L1 = averaged_L1 + RangeInCentimeters / AVERAGENUM ;
    sensors.requestTemperatures();
    float WaterTempC = sensors.getTempCByIndex(0);
    averaged_WT = averaged_WT + WaterTempC / AVERAGENUM ;
  }
  /* SD memory */
  File dataFile = SD.open("datalog2.csv", FILE_WRITE);
  if (dataFile){
    dataFile.print(averaged_H);
    dataFile.print(F(","));
    dataFile.print(averaged_T);
    dataFile.print(F(","));
    dataFile.print(averaged_L1);
    dataFile.print(F(","));
    dataFile.print(averaged_WT);
    dataFile.print(F(","));
    dataFile.println(F(""));
  }else{
    Serial.println("couldn't write a file");
  }
  dataFile.close();
  /* LCD */
  lcd.setCursor(0, 0);
  lcd.print("T:      H:      ");
  lcd.setCursor(2, 0);
  lcd.print(averaged_T);
  lcd.setCursor(10, 0);
  lcd.print(averaged_H);
  lcd.setCursor(0, 2);
  lcd.print("L:      W:      ");
  lcd.setCursor(2, 2);
  lcd.print(averaged_L1);
  lcd.setCursor(10, 2);
  lcd.print(averaged_WT);

  /* LTE-M */
  char buf_Temp[10];
  dtostrf(averaged_T, 4, 1, buf_Temp);
  char buf_Humi[10];
  dtostrf(averaged_H, 4, 1, buf_Humi);
  char buf_Level[10];
  dtostrf(averaged_L1, 4, 1, buf_Level);
  char buf_WaterTemp[10];
  dtostrf(averaged_WT, 4, 1, buf_WaterTemp);
  char payload[128];
  sprintf_P(payload, PSTR("{\"Level\":%d,\"WaterTemp\":%d}"), buf_Level, buf_WaterTemp);
  CONSOLE.print(payload);
  CONSOLE.print(F(" -> Send..."));
  drawText_P(&u8x8, PSTR("Send..."));
  /* connect */
  if (!ctx.connect(ENDPOINT, 80)) {
    CONSOLE.println(F("failed."));
    drawText_P(&u8x8, PSTR("failed."));
    delay(3000);
    return;
  }
  /* send request */
  char hdr_buf[40];
  ctx.println(F("POST / HTTP/1.1"));
  sprintf_P(hdr_buf, PSTR("Host: %s"), ENDPOINT);
  ctx.println(hdr_buf);
  ctx.println(F("Content-Type: application/json"));
  sprintf_P(hdr_buf, PSTR("Content-Length: %d"), strlen(payload));
  ctx.println(hdr_buf);
  ctx.println();
  ctx.println(payload);
  /* receive response */
  // NOTE: response header and body are ignore due to saving for memory
  ctx.stop();
  CONSOLE.println(F("done."));
  u8x8.clearLine(6);
  /* interval */
  delay(INTERVAL_MS);
  /* reset */
  #ifdef RESET_DURATION
  if(millis() > RESET_DURATION ){
    CONSOLE.println("Execute software reset...");
    delay(1000);
    software_reset();
  }
  #endif
}

//---------------------------------------------
void drawText(U8X8* u8x8, const char* in_str, int width) {
  size_t len = strlen(in_str);
  for (int i = 0 ; i < len ; i++) {
    if (u8x8->tx > 16 - 1) {
      u8x8->tx = 0; // CR
      u8x8->ty++;   // LF
    }
    u8x8->print(in_str[i]);
  }
}

void drawText_P(U8X8* u8x8, const char* pgm_s, int width) {
  size_t len = strlen_P(pgm_s);
  for (int i = 0 ; i < len ; i++) {
    if (u8x8->tx > 16 - 1) {
      u8x8->tx = 0; // CR
      u8x8->ty++;   // LF
    }
    char c = pgm_read_byte(pgm_s++);
    u8x8->print(c);
  }
}
