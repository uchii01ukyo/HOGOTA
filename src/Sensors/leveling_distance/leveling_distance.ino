/*
 * Water Level Sensor
*/

#define LiquidLevSENSORPIN A0 //eTape connected to A0
float eTapeReading;
float eTapeLevel;

void setup() {  
   Serial.begin(115200);
   Serial.println("OK");
}

void loop(){
  eTapeReading = analogRead(LiquidLevSENSORPIN);
  eTapeLevel = -eTapeReading*0.105048316+53.27103414; //Equation derived from data
  //eTapeArray[ArrayIndex] = analogRead(LiquidLevSENSORPIN);
  //eTapeLevel = -averageArrayInt(eTapeArray, ArrayLength)*0.105048316+53.27103414; //Equation derived from data
  Serial.print("H2O level "); 
  Serial.print(eTapeReading);
  Serial.println("V "); 
  Serial.print("H2O level "); 
  Serial.print(eTapeLevel);
  Serial.println("cm "); 
  Serial.println(F("*****"));
  delay(500);
}
