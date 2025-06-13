/** @file    task1.cpp
 *  @brief   Source file for a task that controls choke position based on temperature
 *  @details This file contains an FSM to automatically control the motorcycle
 *           choke position. The task is paused until the user flips a toggle
 *           switch on the dashboard to enable choke control. The task will calibrate
 *           the choke position if needed by spinning into the fully choked position.
 *           Then, the task will read the engine temperature, calculate a position
 *           setpoint, and move the motor to the correct position with PI control.
 *  @date    2025-Jun-06 CB created file
 */

#include "task1.h"

// reference private variables generated in main
extern SPI_HandleTypeDef hspi5;  ///< SPI for thermocouple
extern TIM_HandleTypeDef htim4;  ///< Timer for encoder
extern TIM_HandleTypeDef htim12; ///< Timer for motor PWM
extern UART_HandleTypeDef huart1; ///< UART for printing

/**
 * @brief Init FSM in S0_INIT
 */
TASK1::TASK1() : state(S0_INIT)
{
}

/**
 * @brief Run one state of Task 1
 * @details  Run one of the Task 1 states:
 *          - S0_INIT: Init hardware and variables
 *          - S1_PAUSE: Wait for toggle switch to start control
 *          - S2_LIMIT: Calibrate choke position with limit switch
 *          - S3_RUN: PI control of choke position based on engine temperature
 */
void TASK1::run(void)
{
	switch(state)
	{
	case S0_INIT: // initialize choke control
	{
		// start motor PWM
		HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);

		// init motor
		mot_1 = (motor_t) {0, TIM_CHANNEL_1, TIM_CHANNEL_2, &htim12};

		// init encoder
		enc_init(&enc_1, &htim4, false);

		// init thermocouple
		tc_init(&tc_1, GPIOF, GPIO_PIN_9, &hspi5);

		// init choke control variables
		calibrated = false;    // calibration flag
		temp_min = 16;         // min temp to control choke positon [C]
		temp_operating = 38;   // max temp to control choke positon [C]
		temp_meas = 0;         // last temp reading [C]
		time_old = 0;          // old time [msec]
		temp_time_old = 0;     // old time for temp reading [msec]
		time_new = 0;          // new time [msec]
		dt = 0;                // time difference between readings [msec]
		temp_dt = 0;           // time difference between temp readings [msec]
		thermo_time = 250;     // minimum time needed between temp readings [msec]
		position_meas = 0;     // last position reading [encoder ticks]
		position_ref = 0;      // position set point [encoder ticks]
		position_max = 2200;   // maximum position corresponding to 45 deg [encoder ticks]
		err = 0;               // position error [encoder ticks]
		err_ok = 10;           // acceptable position error [encoder ticks]
		int_err = 0;           // accumulating integral error
		L = 0;                 // duty cycle set point [%]
		kp = 0.4f;             // proportional gain
		ki = 0.0004f;          // integral gain

		state = S1_PAUSE;

		break;
	}

	case S1_PAUSE: // wait for user input to begin control
	{
		if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_1) == GPIO_PIN_SET) // choke control switch on
		{
			autoChokeEnable_share = true;  // set auto choke enable bool
			if (calibrated == false)       // calibrate if not
			{
				time_old = HAL_GetTick();  // save time to check later
				set_duty(&mot_1, -100);    // turn on motor
				state = S2_LIMIT;          // set limit state
			}

			else state = S3_RUN;           // run if calibrated
		}
		break;
	}

	case S2_LIMIT: // calibrate motor position
	{
		if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_1) == GPIO_PIN_SET)     // choke control switch on
		{
			time_new = HAL_GetTick();                                 // update time
			dt = time_new - time_old;                                 // find time difference for motor control
			if (dt > 1000)                                            // after running motor for 1 second
			{
				set_duty(&mot_1, 0);                                  // turn off motor
				enc_zero(&enc_1);                                     // clear encoder
				calibrated = true;                                    // set calibration flag
				state = S3_RUN;                                       // set run state
			}
		}
		else                                                          // choke control switch off
		{
			set_duty(&mot_1, 0);                                      // turn off motor
			state = S1_PAUSE;                                         // go back to pause if control switch is off
		}

		break;
	}

	case S3_RUN: // run choke control
	{
		//if (HAL_GPIO_ReadPin (GPIOG, GPIO_PIN_1) == GPIO_PIN_RESET) state = S1_PAUSE; // choke control switch off

		time_new = HAL_GetTick();           // update time
		dt = time_new - time_old;           // find time difference for motor control
		temp_dt = time_new - temp_time_old; // find time difference for temp measurement

		if (temp_dt > thermo_time)          // once enough time has passed
		{
			temp_time_old = time_new;                  // set time for next reading
			temp_meas = (int)(tc_readCelsius(&tc_1));  // read engine temperature
			if (temp_meas < temp_min)                  // below min temp, set position to fully choked
			{
				position_ref = 0;                      // set min position
			}
			else if (temp_meas > temp_operating)       // once warmed up, set position to fully open
			{
				position_ref = position_max;           // set max position
			}
			else // for transitional temps, scale the position linearly from fully choked to open
			{
				position_ref = (temp_meas-temp_min)*position_max/(temp_operating-temp_min);
			}
		}

		enc_update(&enc_1);                        // read encoder
		position_meas = enc_get_position(&enc_1);  // get current position
		err = position_ref - position_meas;        // calculate position error
		int_err += err*dt;                         // add integral error
		if (int_err > 130000) int_err = 130000;    // saturate integral error high
		if (int_err < -130000) int_err = -130000;  // saturate integral error low
		L = (int)((err * kp)+(int_err * ki));      // calculate duty cycle
		if (L > 100) L = 100;                      // saturate duty cycle
		if (L < -100) L = -100;                    // useful for printing but not needed to run
		if (abs(err) < err_ok) L = 0;              // turn if motor once close enough to setpoint
		set_duty(&mot_1, L);                       // set duty cycle

		if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_1) == GPIO_PIN_RESET)
		{
			set_duty(&mot_1, 0);
			state = S1_PAUSE;
			autoChokeEnable_share = false;			// turn of auto choke enable bool
		}

		break;
	}

	default: // no state found
	{

		break;
	}

	}
}
