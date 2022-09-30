# HOGOTA
Agricultural Sensing Systems
  
Agricultural data system that can be checked with a smartphone
  
## System
Assuming outdoor agriculture and using cellular lines (LTE-M communication)  
  
<img src="https://github.com/uchii01ukyo/HOGOTA/blob/main/doc/Picture1.png" width="400px">
  
<img src="https://github.com/uchii01ukyo/HOGOTA/blob/main/doc/System.png" width="500px">
  
Sensing data
+ Atmospheric temperature + humidity
+ CO2
+ Brightness
+ Water Level (ultrasonic + electrostatic capacity + infrared rays)
+ Water temperature + Solid temperature
  
## SORACOM 
 Multiple sensor value send to Cloud via Unified Endpoint of SORACOM
  
### SORACOM LTE-M Shield for Arduino
 Original Code: https://github.com/soracom-labs/arduino-dragino-unified/tree/main/send_multiple_sensor_data_with_soracom
  
### Service used
+ [SORACOM UserConsole](https://console.soracom.io/#/subscribers?coverage_type=jp)
  - Virtual SIM Setting, Verify data
+ [SORACOM Lagoon](https://jp-v2.lagoon.soracom.io/?orgId=1864)
  - Data visualization, Dashboard setting
  
+ [SORACOM Harvest](https://soracom.jp/services/harvest/)
+ [SORACOM Air](https://soracom.jp/services/air/)
  
## Author
Uchii Ukyo(https://github.com/uchii01ukyo)
  
Kagatani Shogo
