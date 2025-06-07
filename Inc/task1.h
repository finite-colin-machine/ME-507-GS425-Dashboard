/** @file    task1.h
 *  @brief   Header for a task that controls choke position based on temperature
 *  @details This file contains an FSM to automatically control the motorcycle
 *           choke position. The task is paused until the user flips a toggle
 *           switch on the dashboard to enable choke control. The task will calibrate
 *           the choke position if needed by spinning until it hits a limit switch.
 *           Then, the task will read the engine temperature, calculate a position
 *           setpoint, and move the motor to the correct position with PI control.
 *  @date    2025-Jun-06 CB created file
 */

#ifndef TASK1_H
#define TASK1_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stm32f7xx_hal.h"
#include "motor.h"
#include "max6675.h"
#include "encoder.h"

/** @brief   Class for Task 1 FSM
 *  @details This is a class  to implement Task 1 as a FSM. The task is paused until
 *           the user flips a toggle switch on the dashboard to enable choke control.
 *           The task will calibrate the choke position if needed by spinning until
 *           it hits a limit switch. Then, the task will read the engine temperature,
 *           calculate a position setpoint, and move the motor to the correct
 *           position with PI control.
 */
class TASK1
{

public:
	/** @brief States in Task 1 FSM
	 */
	typedef enum state
	{
        S0_INIT,   ///< Init hardware and variables
        S1_PAUSE,  ///< Wait for toggle switch to start control
        S2_LIMIT,  ///< Calibrate choke position with limit switch
        S3_RUN     ///< PI control of choke position based on engine temperature
	} state_t;

	/** @brief Prototype constructor for class
	 */
	TASK1();

	/** @brief   Prototype run method
	 */
	void run(void); // prototype run method

private:

	state_t state;              ///< current state

	bool calibrated;            ///< calibration flag
	int temp_min;               ///< min temp to control choke position [C]
	int temp_operating;         ///< max temp to control choke position [C]
	int temp_meas;              ///< last temp reading [C]
	uint32_t time_old;          ///< old time [msec]
	uint32_t temp_time_old;     ///< old time for temp reading [msec]
	uint32_t time_new;          ///< new time [msec]
	uint32_t dt;                ///< time difference between readings [msec]
	uint32_t temp_dt;           ///< time difference between temp readings [msec]
	uint32_t thermo_time;       ///< minimum time needed between temp readings [msec]
	int position_meas;          ///< last position reading [encoder ticks]
	int position_ref;           ///< position set point [encoder ticks]
	int position_max;           ///< maximum position corresponding to 45 deg [encoder ticks]
	int err;                    ///< position error [encoder ticks]
	int err_ok;                 ///< acceptable position error [encoder ticks]
	int int_err;                ///< accumulating integral error
	int L;                      ///< duty cycle set point [%]
	float kp;                   ///< proportional gain
	float ki;                   ///< integral gain

	motor_t mot_1;              ///< motor "object"
	encoder_t enc_1;            ///< encoder "object"
	max6675_tc tc_1;            ///< thermocouple "object"
};

#endif // TASK1_H
