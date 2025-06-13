/** @file    encoder.c
 *  @brief   Source file for a simple encoder driver
 *  @details This file contains an encoder "class" implemented using pseudo-object-oriented
 *           programming. The class works with quadrature or single sensor encoders
 *            interfacing with timer channels. Based off a version by Andrii Ivanchenko.
 *  @date    2025-Jun-06 CB created file
 */

#include "encoder.h"

/** @brief   Init function
 *  @details This is a function used to init an encoder and turn on the appropriate
 *           timer functions for the type of encoder being used.
 *  @param   p_enc Encoder struct
 *  @param   htim  Timer used to store encoder ticks
 *  @param   single_sensor Boolean to indicate single or quadrature encoder
 */
void enc_init(encoder_t* p_enc, TIM_HandleTypeDef* htim, bool single_sensor)
{
    p_enc->timer = htim;
    if (single_sensor == true) HAL_TIM_Base_Start(htim);
    else HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
    p_enc->counter_old = __HAL_TIM_GET_COUNTER(htim);
    p_enc->position = 0;
    p_enc->speed = 0.0f;
    p_enc->time_old = HAL_GetTick();
}

/** @brief   Update function
 *  @details This is a function used to update an encoder by calculating a new position and speed.
 *  @param   p_enc Encoder struct
 */
void enc_update(encoder_t* p_enc)
{
    int16_t counter_new = __HAL_TIM_GET_COUNTER(p_enc->timer); // read encoder
    int16_t delta = counter_new - p_enc->counter_old;          // find difference since last reading
    p_enc->counter_old = counter_new;                          // set up next reading

    // account for timer roll over
    if (delta > 32768) delta -= 65536;
    else if (delta < -32768) delta += 65536;

    p_enc->position += delta;                   // update position [encoder ticks]
    uint32_t time_new = HAL_GetTick();          // update time [msec]
    uint32_t dt = time_new - p_enc->time_old;   // find difference since last reading [msec]
    p_enc->time_old = time_new;                 // set up next reading
    p_enc->speed = delta*1000/dt;               // calculate speed [encoder ticks / sec]
}

/** @brief   Get position function
 *  @details This is a function that returns an encoder position.
 *  @param   p_enc Encoder struct
 */
int enc_get_position(encoder_t* p_enc)
{
    return p_enc->position;
}

/** @brief   Get speed function
 *  @details This is a function that returns an encoder speed.
 *  @param   p_enc Encoder struct
 */
float enc_get_speed(encoder_t* p_enc)
{
    return p_enc->speed;
}

/** @brief   Zero position function
 *  @details This is a function that zeroes an encoder position.
 *  @param   p_enc Encoder struct
 */
void enc_zero(encoder_t* p_enc)
{
	p_enc->counter_old = __HAL_TIM_GET_COUNTER(p_enc->timer);
	p_enc->position = 0;
}
