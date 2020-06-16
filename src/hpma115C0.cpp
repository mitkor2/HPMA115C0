/**
 * @file HPMA115C0.cpp
 * @author Dimitar Rangelov & Emil Kirilov
 * @date June 2020
 * @brief Arduino Library for Honeywell's Particle Sensor (HPMA115C0-003 // HPMA115C0-004)
 * @license MIT
 */

#include "Arduino.h"
#include "HPMA115C0.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
}

/**
 * @brief Constructor for HPMA115C0 class
 * @param  a Stream ({Software/Hardware}Serial) object.
 * @note The serial stream should be already initialized
 * @return  void
 */
HPMA115C0::HPMA115C0(Stream& serial):
  _serial(serial)
{
  _serial.setTimeout(100);
}

/**
 * @brief Function that initializes sensor
 * @return  a String containing sensor response
 */
void HPMA115C0::Init() {
  //Serial.println("PS- Initializing...");
  delay(100);
  StartParticleMeasurement();
  delay(100);
  DisableAutoSend();
}

/**
 * @brief Function that sends serial command to sensor
 * @param  a unsigned char * containing the command
 * @param size of buffer
 * @return  void
 */
void HPMA115C0::SendCmd(uint8_t * cmdBuf, uint32_t cmdSize) {
  //Clear RX
  while (_serial.available())
    _serial.read();

  //Send command
  //Serial.print("PS- Sending cmd: ");
  uint32_t index = 0;
  for (index = 0; index < cmdSize; index++) {
    //Serial.print(cmdBuf[index], HEX);
    //Serial.print(" ");
    _serial.write(cmdBuf[index]);
  }
  //Serial.println("");
  return;
}

/**
 * @brief Function that reads command response from sensor
 * @param Buffer to store data in
 * @param Buffer size
 * @param Expected command type
 * @return  returns number of bytes read from sensor
 */
int HPMA115C0::ReadCmdResp(char * dataBuf, uint32_t dataBufSize, uint32_t cmdType)
{
  static char respBuf[HPM_MAX_RESP_SIZE];
  static uint32_t respIdx = 0;
  static uint32_t calChecksum = 0;

  //Read response
  respIdx = 0;
  calChecksum = 0;

  memset(respBuf, 0, sizeof(respBuf));

  _serial.setTimeout(100);

  //Serial.println("PS- Waiting for cmd resp...");
  if (_serial.readStringUntil(HPM_CMD_RESP_HEAD))
  {
    delay(1); //wait for the rest of the bytes to arrive
    respBuf[HPM_HEAD_IDX] = HPM_CMD_RESP_HEAD;
    
	uint32_t timeout = millis() + 100;
	
	while (_serial.available() == 0)
	{
		if (millis() > timeout)
		{
			break;
		}
	}
	
	respBuf[HPM_LEN_IDX] = _serial.read(); //Read the command length
	
	uint8_t len = respBuf[HPM_LEN_IDX];
	//Serial.print("len = "); Serial.println(len);
	
    //Ensure buffers are big enough
    if (
		 (len > 0) &&
		((len + 1) <=  sizeof(respBuf) - 2) &&
		((len - 1) <= dataBufSize )
	)
    {
      if (_serial.readBytes(&respBuf[HPM_CMD_IDX], len + 1) == (len + 1))
      {
		//read respBuf[HPM_LEN_IDX] num of bytes + calChecksum byte
        //check if CMD type matches
        if (respBuf[HPM_CMD_IDX] == cmdType)
        {
          //Calculate and validate checksum
          for (respIdx = 0; respIdx < (2 + len); respIdx++)
          {
            calChecksum += respBuf[respIdx];
          }
		  
          calChecksum = (65536 - calChecksum) % 256;

          if (calChecksum == respBuf[2 + len])
          {
            memset(dataBuf, 0, dataBufSize);
            memcpy(dataBuf, &respBuf[HPM_DATA_START_IDX], len - 1);
            return (len - 1);
          }
        }
      }
    }
  }

  return false;
}


/**
 * @brief Function that sends a read command to sensor
 * @return  returns true if valid measurements were read from sensor
 */
boolean HPMA115C0::ReadParticleMeasurement(uint32_t * pm1_0 /* = NULL */,uint32_t * pm2_5 /* = NULL */,uint32_t * pm4_0 /* = NULL */, uint32_t * pm10 /* = NULL */)
{
  uint8_t cmdBuf[] = {0x68, 0x01, 0x04, 0x93};
  //Send command
  SendCmd(cmdBuf, 4);
 
 char dataBuf[HPM_READ_PARTICLE_MEASURMENT_LEN - 1];

  //Read response
  if (ReadCmdResp(dataBuf, sizeof(dataBuf), READ_PARTICLE_MEASURMENT) == (HPM_READ_PARTICLE_MEASURMENT_LEN - 1))
  {
	_pm1_0 = dataBuf[0] * 256 + dataBuf[1];  
    _pm2_5 = dataBuf[2] * 256 + dataBuf[3];
	_pm4_0 = dataBuf[4] * 256 + dataBuf[5];
    _pm10 = dataBuf[6] * 256 + dataBuf[7];

    if (pm1_0)
    {
        *pm1_0 = _pm1_0;
    }

    if (pm2_5)
    {
        *pm2_5 = _pm2_5;
    }
	
    if (pm4_0)
    {
        *pm4_0 = _pm4_0;
    }

    if (pm10)
    {
        *pm10 = _pm10;
    }
	
    return true;
  }

  return false;
}

/**
 * @brief Function that starts sensor measurement
 * @return  void
 */
void HPMA115C0::StartParticleMeasurement() {
  uint8_t cmd[] = {0x68, 0x01, 0x01, 0x96};
  SendCmd(cmd, 4);
}

/**
 * @brief Function that stops sensor measurement
 * @return  void
 */
void HPMA115C0::StopParticleMeasurement() {
  uint8_t cmd[] = {0x68, 0x01, 0x02, 0x95};
  SendCmd(cmd, 4);
}

/**
 * @brief Function that enables auto send
 * @return  void
 */
void HPMA115C0::EnableAutoSend() {
  uint8_t cmd[] = {0x68, 0x01, 0x40, 0x57};
  SendCmd(cmd, 4);
}

/**
 * @brief Function that stops auto send
 * @return  void
 */
void HPMA115C0::DisableAutoSend() {
  uint8_t cmd[] = {0x68, 0x01, 0x20, 0x77};
  SendCmd(cmd, 4);
}

/**
* @brief Function that returns the latest PM 1.0 reading
* @note Sensor reports new reading ~ every 1 sec.
* @return  PM 1.0 reading (uint32_t)
*/
uint32_t HPMA115C0::GetPM1_0() {
  return _pm1_0;
}

/**
* @brief Function that returns the latest PM 2.5 reading
* @note Sensor reports new reading ~ every 1 sec.
* @return  PM 2.5 reading (uint32_t)
*/
uint32_t HPMA115C0::GetPM2_5() {
  return _pm2_5;
}

/**
* @brief Function that returns the latest PM 4.0 reading
* @note Sensor reports new reading ~ every 1 sec.
* @return  PM 4.0 reading (uint32_t)
*/
uint32_t HPMA115C0::GetPM4_0() {
  return _pm4_0;
}

/**
* @brief Function that returns the latest PM 10 reading
* @note Sensor reports new reading ~ every 1 sec.
* @return  PM 10 reading (uint32_t)
*/
uint32_t HPMA115C0::GetPM10() {
  return _pm10;
}
