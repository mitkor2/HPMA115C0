# HPMA115C0    [![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org) 
#
#
##### Developed with the help of 
[![Zekeng](https://i.ibb.co/7SYVMvS/rsz-logo-zekeng-234x150.png)](https://zekeng.com)

## Description

 Arduino Library for Honeywell's Particle Sensor (HPMA115C0-003 / HPMA115C0-004)  
## Documentation
    
The documentation of HPMA115C0-003 / HPMA115C0-004 can be found in the docs folder in the repository or in this [link.](https://sensing.honeywell.com/honeywell-sensing-particulate-hpm-series-datasheet-32322550)
## Commands
  - Read Particle Measuring Results - **0x68, 0x01, 0x04, 0x93**
  - Start Particle Measurement - **0x68, 0x01, 0x01, 0x96**
  - Stop Particle Measurement - **0x68, 0x01, 0x02, 0x95**
  - Stop Auto Send - **0x68, 0x01, 0x20, 0x77**
  - Enable Auto Send - **0x68, 0x01, 0x40, 0x57**

## Functions
 - Init
    --StartParticleMeasurement
    --DisableAutoSend
  - ReadParticleMeasurement
  -- ReadCmdResp
  - StartParticleMeasurement
  - StopParticleMeasurement
  - DisableAutoSend
  - EnableAutoSend

## Example
HPMA115C0-003 / HPMA115C0-004 are measuring PM1.0, PM2.5, PM4.0 and PM10. 

**NOTE: The delay before initialization should be minimum 2000ms otherwise the sensor is going to autosend mode and it is not received commands anymore.**
```sh
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
SoftwareSerial hpmaSerial(10, 11); 	// Feather TX, Feather RX

//Create an instance of the hpma115C0 library

HPMA115C0 hpma115C0(hpmaSerial);

void setup() 
{
  Serial.begin(115200);
  hpmaSerial.begin(9600); 			    // Default sensor baud rate - 9600
  delay(2000); 						          // The delay should be 2000-5000 ms 
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
```
License
----

MIT
