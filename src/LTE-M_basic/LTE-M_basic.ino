/* 
 * SORACOM LTE-M Shield for Arduino
 * Multiple sensor value send to Cloud via Unified Endpoint of SORACOM
 *
 * Original Code: https://github.com/soracom-labs/arduino-dragino-unified/tree/main/send_multiple_sensor_data_with_soracom
 * Manual: https://soracom.jp/recipes_index/4919/ 
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
  #include <Wire.h>
  
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
  CONSOLE.println("Complete.");
}

//-------------------------------------
void loop() {
  /* Set Data */
  float averaged_T=30;
  float averaged_H=80;
  float averaged_L1=10;
  float averaged_WT=30;
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
