/** @file    encoder.h
 *  @brief   Header for a simple encoder driver
 *  @details This file contains an encoder "class" implemented using pseudo-object-oriented
 *           programming. The class works with quadrature or single sensor encoders
 *            interfacing with timer channels. Based off a version by Andrii Ivanchenko.
 *  @date    2025-Jun-06 CB created file
 */

#ifndef SRC_ENCODER_H_
#define SRC_ENCODER_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief   Encoder struct
 *  @details This is a struct for an encoder "object" that contains the
 *           information needed to use the functions in the encoder driver "class".
 *  @param   timer Timer used to store encoder ticks
 *  @param   single_sensor Boolean to indicate single or quadrature encoder
 *  @param   counter_old Last measured encoder ticks
 *  @param   position Current angular position
 *  @param   speed Current angular speed
 *  @param   time_old Time of previous encoder read
 */
typedef struct {
    TIM_HandleTypeDef* timer;
    bool single_sensor;
    int16_t counter_old;
    int position;
    float speed;
    uint32_t time_old;
} encoder_t;

/** @brief   Init function
 *  @details This is a function used to init an encoder and turn on the appropriate
 *           timer functions for the type of encoder being used.
 *  @param   p_enc Encoder struct
 *  @param   htim  Timer used to store encoder ticks
 *  @param   single_sensor Boolean to indicate single or quadrature encoder
 */
void enc_init(encoder_t* p_enc, TIM_HandleTypeDef* htim, bool single_sensor);

/** @brief   Update function
 *  @details This is a function used to update an encoder by calculating a new position and speed.
 *  @param   p_enc Encoder struct
 */
void enc_update(encoder_t* p_enc);

/** @brief   Get position function
 *  @details This is a function that returns an encoder position.
 *  @param   p_enc Encoder struct
 */
int enc_get_position(encoder_t* p_enc);

/** @brief   Get speed function
 *  @details This is a function that returns an encoder speed.
 *  @param   p_enc Encoder struct
 */
float enc_get_speed(encoder_t* p_enc);

/** @brief   Zero position function
 *  @details This is a function that zeroes an encoder position.
 *  @param   p_enc Encoder struct
 */
void enc_zero(encoder_t* p_enc);

#ifdef __cplusplus
}
#endif

#endif /* SRC_ENCODER_H_ */
