/** @file    motor.h
 *  @brief   Header for a simple full-bridge motor driver with PWM control
 *  @details This file contains a motor "class" implemented using pseudo-object-oriented
 *           programming. The class controls a single motor by sending PWM signals to a
 *           full-bridge motor driver.
 *  @date    2025-Jun-06 CB created file
 */

#ifndef SRC_MOTOR_H_
#define SRC_MOTOR_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "stm32f7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @brief   Motor struct
 *  @details This is a struct for a motor "object" that contains the information
 *           needed to use the functions in the motor driver "class".
 *  @param   duty PWM duty cycle used to set motor voltage
 *  @param   channel_a First timer channel used to generate PWM
 *  @param   channel_b Second timer channel used to generate PWM
 *  @param   *htim Timer used to generate PWM
 */
typedef struct {
    int32_t  duty;
    uint32_t channel_a;
    uint32_t channel_b;
    TIM_HandleTypeDef *htim;
} motor_t;

/** @brief   Enable function
 *  @details This is a function used to enable a motor with a 0% duty cycle by
 *           setting both channels low. This is stop mode.
 *  @param   p_mot Motor struct
 */
void enable(motor_t* p_mot);

/** @brief   Disable function
 *  @details This is a function used to disable a motor by setting both channels
 *           to the maximum value based of the timer autoreload and prescaler.
 *           This is brake mode.
 *  @param   p_mot Motor struct
 */
void disable(motor_t* p_mot);

/** @brief   Set duty function
 *  @details This is a function used to set the duty cycle and direction of a
 *           motor.
 *  @param   p_mot Motor struct
 *  @param   duty Motor duty cycle with sign for direction
 */
void set_duty(motor_t* p_mot, int32_t duty);

#ifdef __cplusplus
}
#endif

#endif /* SRC_MOTOR_H_ */
