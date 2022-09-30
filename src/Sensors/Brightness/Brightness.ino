#include <Wire.h>
#include <BH1750.h>

BH1750 bh1750_1;
BH1750 bh1750_2;

void setup() {
  Serial.begin(115200);
  Wire.begin(21,22);
  bh1750_1.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire); // Sensor 1 connected to GND
  bh1750_2.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C, &Wire); // Sensor 2 connected to VCC
}

void loop() {
  float light_level_1;
  float light_level_2;
  light_level_1 = bh1750_a.readLightLevel();
  light_level_2 = bh1750_b.readLightLevel();
  Serial.print(light_level_1);
  Serial.print(" ");
  Serial.println(light_level_2);
  delay(1000);
}
