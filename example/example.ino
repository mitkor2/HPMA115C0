/**
 * @file example.ino
 * @author Dimitar Rangelov
 * @date June 2020
 * @brief Example using HPMA115C0 sensor library on a Feather 32u4
 * @license MIT
 */

#include <hpma115C0.h>
#include <SoftwareSerial.h>

//Create an instance of software serial
SoftwareSerial hpmaSerial(10, 11);  // Feather TX, Feather RX

//Create an instance of the hpma115C0 library

HPMA115C0 hpma115C0(hpmaSerial);

void setup() 
{
  Serial.begin(115200);
  hpmaSerial.begin(9600)            // Default sensor baud rate - 9600
  delay(2000);                      // The delay should be 2000-5000 ms 
  Serial.println("Starting...");
  hpma115C0.Init();
}

void loop() 
{
  uint32_t pm1_0,pm2_5,pm4_0,pm10;
  if (hpma115C0.ReadParticleMeasurement(&pm1_0,&pm2_5,&pm4_0,&pm10))
  {
	  Serial.println("PM 1.0: " + String(pm1_0) + " ug/m3" );
	  Serial.println("PM 2.5: " + String(pm2_5) + " ug/m3" );
    Serial.println("PM 4.0: " + String(pm4_0) + " ug/m3" );
    Serial.println("PM 10: " + String(pm10) + " ug/m3" );
  }
  delay(1000);
}
