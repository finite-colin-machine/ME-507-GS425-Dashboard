/** @file    motor.c
 *  @brief   Source file for a simple full-bridge motor driver with PWM control
 *  @details This file contains a motor "class" implemented using pseudo-object-oriented
 *           programming. The class controls a single motor by sending PWM signals to a
 *           full-bridge motor driver.
 *  @date    2025-Jun-06 CB created file
 */

#include "motor.h"

/** @brief   Enable function
 *  @details This is a function used to enable a motor with a 0% duty cycle by
 *           setting both channels low. This is stop mode.
 *  @param   p_mot Motor struct
 */
void enable(motor_t* p_mot)
{
    __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_a, 0);
    __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_b, 0);
}

/** @brief   Disable function
 *  @details This is a function used to disable a motor by setting both channels
 *           to the maximum value based of the timer autoreload and prescaler.
 *           This is brake mode.
 *  @param   p_mot Motor struct
 */
void disable(motor_t* p_mot) // set both channels high (brake motor)
{
    __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_a, 9999); // (auto reload = 10000, prescaler = 0)
    __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_b, 9999); // (auto reload = 10000, prescaler = 0)
}

/** @brief   Set duty function
 *  @details This is a function used to set the duty cycle and direction of a
 *           motor.
 *  @param   p_mot Motor struct
 *  @param   duty Motor duty cycle with sign for direction
 */
void set_duty(motor_t* p_mot, int32_t duty)
{
    p_mot->duty = duty; // assign the duty cycle to a field in the structure

    if (duty > 100) duty = 100;   // saturate duty
    if (duty < -100) duty = -100;


    int32_t duty_scaled = duty * 99; // scale duty based on timer settings (auto reload = 10000, prescaler = 0)

    if (duty == 0) // set both channels low (stop motor)
    {
        __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_a, 0);
        __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_b, 0);
    }
    else if (duty > 0) // set first channel to duty cycle and other low (spin forward)
    {
        __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_a, 0);
        __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_b, duty_scaled);
    }
    else // set second channel to duty cycle and other low (spin reverse)
    {
        __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_a, -1*duty_scaled);
        __HAL_TIM_SET_COMPARE(p_mot->htim, p_mot->channel_b, 0);
    }
}
