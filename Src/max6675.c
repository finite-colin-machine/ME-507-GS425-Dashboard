/** @file    max6675.c
 *  @brief   Source file for a thermocouple amplifier
 *  @details This file contains a thermocouple amplifier "class" implemented using
 *           pseudo-object-oriented programming. The class works with the MAX6675
 *           and reads temperature over SPI. Based off versions by Justin Ng and Adafruit.
 *  @date    2025-Jun-06 CB created file
 */

#include "max6675.h"

/** @brief   Init function
 *  @details This is a function used to init a thermocouple and configure SPI communication
 *           with the thermocouple amplifier.
 *  @param   sensor Thermocouple struct
 *  @param   CS_PORT Port of CS pin used in SPI communication
 *  @param   CS_PIN CS pin number used in SPI communication
 *  @param   hspi SPI used to communicate
 */
void tc_init(max6675_tc* sensor, GPIO_TypeDef* CS_PORT, uint16_t CS_PIN, SPI_HandleTypeDef* hspi)
{
    sensor->CS_PORT = CS_PORT;
    sensor->CS_PIN = CS_PIN;
    sensor->hspi = hspi;
}

/** @brief   Init function
 *  @details This is a function used to read the temperature from a thermocouple in celsius.
 *           this function can only be called every 250 ms, or the temp will not update.
 *  @param   sensor Thermocouple struct
 */
float tc_readCelsius(max6675_tc* sensor)
{
    uint8_t buf[2];                                                     // init buf to store reading
    HAL_GPIO_WritePin(sensor->CS_PORT, sensor->CS_PIN, GPIO_PIN_RESET); // start reading
    HAL_SPI_Receive(sensor->hspi, buf, 2, HAL_MAX_DELAY);               // store data to buf
    HAL_GPIO_WritePin(sensor->CS_PORT, sensor->CS_PIN, GPIO_PIN_SET);   // end reading

    uint16_t v_raw = (buf[0] | (buf[1] << 8));         // combine buf
    if (v_raw & 0x0004) sensor->tc_connected = false;  // check connection bit and set flag
    else sensor->tc_connected = true;                  // set connection flag
    uint16_t v = (buf[0] | (buf[1] << 8)) >> 3;        // process buf and ignore non temp bits
    float celsius = v*0.25;                            // convert v to deg C
    sensor->tempCelsius = celsius;                     // save temp
    return celsius;                                    // return temp
}
