/** @file    task2.cpp
 *  @brief   Source file for a task that collects measurements used by the dashboard
 *  @details This file contains an FSM to collect measurements used by the
 *           dashboard. The tasks continuously measures wheel speed and engine
 *           temperature and stores the values for display driver use.
 *  @date    2025-Jun-06 CB created file
 */

#include "task2.h"

// reference private variables generated in main
extern SPI_HandleTypeDef hspi5;   ///< SPI for thermocouple
extern TIM_HandleTypeDef htim2;   ///< Timer for encoder
extern UART_HandleTypeDef huart1; ///< UART for printing

/**
 * @brief Init FSM in S0_INIT
 */
TASK2::TASK2() : state(S0_INIT)
{
}

/**
 * @brief Run one state of Task 2
 * @details  Run one of the Task 2 states:
 *          - S0_INIT: Init hardware and variables
 *          - S1_RUN: Collect measurements
 */
void TASK2::run(void)
{
	switch(state)
	{
	case S0_INIT: // initialize choke control
	{
		// init encoder
		enc_init(&enc_2, &htim2, true);

		// init thermocouple
		tc_init(&tc_2, GPIOF, GPIO_PIN_9, &hspi5);

		// init variables
		temp_meas = 0;           // temp reading [C]
		temp_meas_share = 0;     // temp reading to share [C]
		time_old = 0;            // old time [msec]
		time_new = 0;            // new time [msec]
		dt = 0;                  // time difference between readings [msec]
		thermo_time = 250;       // minimum time needed between temp readings [msec]
		wheel_speed = 0;         // front wheel speed [MPH]
		wheel_speed_share = 0;   // front wheel speed to share [MPH]
		speed_conversion = 2.3f; // convert encoder ticks / second to MPH

		state = S1_RUN; // set run state

		break;
	}

	case S1_RUN: // run measurement collection
	{
		time_new = HAL_GetTick(); // update time
		dt = time_new - time_old; // find time difference for temp measurement

		if (dt > thermo_time)   // once enough time has passed
		{
			time_old = time_new;                       // set time for next reading
			temp_meas = (int)(tc_readCelsius(&tc_2));  // read engine temperature
			temp_meas_share = temp_meas;               // set public temp
		}

		enc_update(&enc_2);                                          // read encoder
		wheel_speed = (int)(enc_get_speed(&enc_2)*speed_conversion); // calculate wheel speed
		wheel_speed_share = wheel_speed;                             // set public wheel speed

		// print for debugging
		char buffer_tx[120];
		sprintf(buffer_tx, "Wheel Speed: %d | Temp: %d\r\n", wheel_speed, temp_meas);
		HAL_UART_Transmit(&huart1, (uint8_t*)buffer_tx, strlen(buffer_tx), HAL_MAX_DELAY);

		break;
	}

	default: // no state found
	{

		break;
	}

	}
}
