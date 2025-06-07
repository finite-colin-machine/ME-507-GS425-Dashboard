/** @file    task2.h
 *  @brief   Header for a task that collects measurements used by the dashboard
 *  @details This file contains an FSM to collect measurements used by the
 *           dashboard. The tasks continuously measures wheel speed and engine
 *           temperature and stores the values for display driver use.
 *  @date    2025-Jun-06 CB created file
 */

#ifndef TASK2_H
#define TASK2_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stm32f7xx_hal.h"
#include "max6675.h"
#include "encoder.h"

/** @brief   Class for Task 2 FSM
 *  @details This is a class  to implement Task 2 as a FSM. This file contains
 *           an FSM to collect measurements used by the dashboard. The tasks
 *           continuously measures wheel speed and engine temperature and stores
 *           the values for display driver use.
 */
class TASK2
{

public:
	/** @brief States in Task 2 FSM
	 */
	typedef enum state
	{
		S0_INIT,   ///< Init hardware and variables
		S1_RUN     ///< Collect measurements
	} state_t;

	int temp_meas_share;   ///< Engine temp accessible outside of class
	int wheel_speed_share; ///< Wheel speed accessible outside of class

	/** @brief Prototype constructor for class
	 */
	TASK2();

	/** @brief   Prototype run method
	 */
	void run(void); // prototype run method

private:

	state_t state; // things outside class cannot change state

	int temp_meas;               ///< temp reading [C]
	uint32_t time_old;           ///< old time [msec]
	uint32_t time_new;           ///< new time [msec]
	uint32_t dt;                 ///< time difference between readings [msec]
	uint32_t thermo_time;        ///< minimum time needed between temp readings [msec]
	int wheel_speed;             ///< front wheel speed [MPH]
	float speed_conversion;      ///< convert encoder ticks / second to MPH

	encoder_t enc_2;             ///< encoder "object"
	max6675_tc tc_2;             ///< thermocouple "object"

};

#endif // TASK2_H
