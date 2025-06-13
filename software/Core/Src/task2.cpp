/** @file    task2.cpp
 *  @brief   Source file for a task that collects measurements used by the
 * 			 dashboard
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
		temp_meas = 0;                     // temp reading [C]
		temp_meas_share = 0;               // temp reading to share [C]
		time_old = HAL_GetTick();          // old encoder time [msec]
		temp_time_old = HAL_GetTick();     // old temp time [msec]
		time_new = 0;                      // new time [msec]
		dt = 0;            // time difference between encoder readings [msec]
		temp_dt = 0;       // time difference between temp readings [msec]
		enc_time = 200;    // minimum time needed between encoder readings[msec]
		thermo_time = 250; // minimum time needed between temp readings[msec]
		wheel_speed = 0;                   // front wheel speed [MPH]
		engine_speed = 0;                  // engine speed [RPM]
		distance = 0;                      // distance traveled [miles]
		wheel_speed_share = 0;             // front wheel speed to share [MPH]
		engine_speed_share = 0;            // engine speed to share [MPH]
		distance_share = 0;               // distance traveled [mi]
		wheel_speed_conversion = 1.2852f; // convert encoder ticks/second to MPH
		engine_speed_conversion = 4.0f;   // convert encoder ticks/second to RPM
        distance_conversion = 1.885f;     // convert encoder ticks to distance
        memset(gear_share, 0,
				sizeof(gear_share)); // array of pin states for gear selection

		state = S1_RUN; // set run state

		break;
	}

	case S1_RUN: // run measurement collection
	{
		time_new = HAL_GetTick(); 			// update time
	    dt = time_new - time_old; 			// find time difference for encoder
											//	measurement
		temp_dt = time_new - temp_time_old; // find time difference for temp
											// measurement

		if (dt > enc_time) 		// once enough time has passed to read speed
		{
			time_old = time_new;                   // set time for next reading
			enc_update(&enc_2);                         // read encoder
			engine_speed = (int)(enc_get_speed(&enc_2)*
							engine_speed_conversion); // calculate engine speed
			engine_speeds[0] = engine_speed;          // save most recent speed

			// calculate rolling average of engine speed and update for next run
			engine_speed_share = (engine_speeds[0]+engine_speeds[1]+
									engine_speeds[2]+engine_speeds[3]+
									engine_speeds[4])/5;
			engine_speeds[4] = engine_speeds[3];
			engine_speeds[3] = engine_speeds[2];
			engine_speeds[2] = engine_speeds[1];
			engine_speeds[1] = engine_speeds[0];

			/* THIS SECTION NOT CURRENLTY IMPLEMENTED ON THE BOARD
			// calculate wheel speed
			wheel_speed = (int)(enc_get_speed(&enc_2)*speed_conversion);
			// set public wheel speed
			wheel_speed_share = wheel_speed;
			// calculate odometer reading
			distance = (int)(enc_get_position(&enc_2)*distance_conversion);
			// set public odometer reading
			distance_share = distance;
			*/

			// read and update all gear indicator pins
			gear_share[0] = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4);
			gear_share[1] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
			gear_share[2] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11);
			gear_share[3] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12);
			gear_share[4] = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0);
			gear_share[5] = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1);
			gear_share[6] = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2);
		}

		// once enough time has passed to read temperature
		if (temp_dt > thermo_time)
		{
			temp_time_old = time_new;               // set time for next reading
			temp_meas = (int)(tc_readCelsius(&tc_2)); // read engine temperature
			temp_meas_share = temp_meas;              // set public temp
		}

		break;
	}

	default: // no state found
	{

		break;
	}

	}
}
