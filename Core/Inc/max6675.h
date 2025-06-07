/** @file    max6675.h
 *  @brief   Header for a thermocouple amplifier
 *  @details This file contains a thermocouple amplifier "class" implemented using
 *           pseudo-object-oriented programming. The class works with the MAX6675
 *           and reads temperature over SPI. Based off versions by Justin Ng and Adafruit.
 *  @date    2025-Jun-06 CB created file
 */

#ifndef MAX6675_H_
#define MAX6675_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief   Thermocouple struct
 *  @details This is a struct for a thermocouple "object" that contains the
 *           information needed to use the functions in the thermocouple
 *           driver "class".
 *  @param   tempCelsius Temperature reading
 *  @param   CS_PORT Port of CS pin used in SPI communication
 *  @param   CS_PIN CS pin number used in SPI communication
 *  @param   hspi SPI used to communicate
 *  @param   tc_connected Boolean flag to indicate connection status
 */
typedef struct {
    float tempCelsius;
    GPIO_TypeDef* CS_PORT;
    uint16_t CS_PIN;
    SPI_HandleTypeDef* hspi;
    bool tc_connected;
} max6675_tc;

/** @brief   Init function
 *  @details This is a function used to init a thermocouple and configure SPI communication
 *           with the thermocouple amplifier.
 *  @param   sensor Thermocouple struct
 *  @param   CS_PORT Port of CS pin used in SPI communication
 *  @param   CS_PIN CS pin number used in SPI communication
 *  @param   hspi SPI used to communicate
 */
void tc_init(max6675_tc* sensor, GPIO_TypeDef* CS_PORT, uint16_t CS_PIN, SPI_HandleTypeDef* hspi);

/** @brief   Init function
 *  @details This is a function used to read the temperature from a thermocouple in celsius.
 *           this function can only be called every 250 ms, or the temp will not update.
 *  @param   sensor Thermocouple struct
 */
float tc_readCelsius(max6675_tc* sensor);

#ifdef __cplusplus
}
#endif

#endif /* MAX6675_H_ */
