/**
 *
 * @file HPMA115C0.h
 * @author Dimitar Rangelov & Emil Kirilov
 * @date June 2020
 * @brief Arduino Library for Honeywell's Particle Sensor (HPMA115C0-003 // HPMA115C0-004)
 *
 */

#ifndef HPMA115C0_H
#define HPMA115C0_H

#include "Arduino.h"
#include <stdint.h>

#define HPM_CMD_RESP_HEAD 0x40
#define HPM_MAX_RESP_SIZE 16
#define HPM_READ_PARTICLE_MEASURMENT_LEN 13

enum CMD_TYPE_T {
    READ_PARTICLE_MEASURMENT = 0x04,
    START_PARTICLE_MEASURMENT = 0x01,
    STOP_PARTICLE_MEASURMENT = 0x02,
    SET_ADJUSTMENT_COEFFICIENT = 0x08,
    READ_ADJUSTMENT_COEFFICIENT = 0x10, // according to the datasheet
    STOP_AUTO_SEND = 0x20,
    ENABLE_AUTO_SEND = 0x40,
};

enum HPM_PACKET_T {
    HPM_HEAD_IDX,
    HPM_LEN_IDX,
    HPM_CMD_IDX,
    HPM_DATA_START_IDX
};

class HPMA115C0
{
public:
    /**
     * @brief Constructor for HPMA115C0 class
     * @param  a Stream ({Software/Hardware}Serial) object.
     * @note The serial stream should be already initialized
     * @return  void
     */
    HPMA115C0(Stream& serial);

    /**
     * @brief Function that initializes sensor
     * @return  a String containing sensor response
     */
    void Init();

    /**
     * @brief Function that sends a read command to sensor
     * @return  returns true if valid measurements were read from sensor
     */boolean ReadParticleMeasurement(uint32_t * pm1_0 = NULL,uint32_t * pm2_5 = NULL,uint32_t * pm4_0 = NULL, uint32_t * pm10 = NULL)
    ;

    /**
     * @brief Function that starts sensor measurement
     * @return  void
     */
    void StartParticleMeasurement();
    /**
     * @brief Function that stops sensor measurement
     * @return  void
     */
    void StopParticleMeasurement();

    /**
     * @brief Function that enables auto send
     * @return  void
     */
    void EnableAutoSend();

    /**
     * @brief Function that stops auto send
     * @return  void
     */
    void DisableAutoSend();

    /**
    * @brief Function that returns the latest PM 1.0 reading
    * @note Sensor reports new reading ~ every 1 sec.
    * @return  PM 1.0 reading (uint32_t)
    */
    uint32_t GetPM1_0();

    /**
    * @brief Function that returns the latest PM 2.5 reading
    * @note Sensor reports new reading ~ every 1 sec.
    * @return  PM 2.5 reading (uint32_t)
    */
    uint32_t GetPM2_5();

    /**
    * @brief Function that returns the latest PM 4.0 reading
    * @note Sensor reports new reading ~ every 1 sec.
    * @return  PM 4.0 reading (uint32_t)
    */
    uint32_t GetPM4_0();

    /**
    * @brief Function that returns the latest PM 10 reading
    * @note Sensor reports new reading ~ every 1 sec.
    * @return  PM 10 reading (uint32_t)
    */
    uint32_t GetPM10();

private:
    Stream& _serial;

    //Latest PM 1.0 reading
    uint32_t _pm1_0 = 0;
	
	//Latest PM 2.5 reading
    uint32_t _pm2_5 = 0;
	
	//Latest PM 4.0 reading
    uint32_t _pm4_0 = 0;

    //Latest PM 10 reading
    uint32_t _pm10 = 0;

    /**
     * @brief Function that sends serial command to sensor
     * @param  a char * containing the command
     * @param size of buffer
     * @return  void
     */
    void SendCmd(uint8_t * command, uint32_t size);

    /**
    * @brief Function that reads command response from sensor
    * @param Buffer to store data in
    * @param Buffer size
    * @param Expected command type
    * @return  returns number of bytes read from sensor
    */
    int ReadCmdResp(char * dataBuf, uint32_t dataBufSize, uint32_t cmdType);
};

#endif
