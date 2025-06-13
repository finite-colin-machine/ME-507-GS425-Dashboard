/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BACK_LIGHT_PWM_Pin GPIO_PIN_6
#define BACK_LIGHT_PWM_GPIO_Port GPIOF
#define NOT_CS_Pin GPIO_PIN_9
#define NOT_CS_GPIO_Port GPIOF
#define OIL_LIGHT_Pin GPIO_PIN_0
#define OIL_LIGHT_GPIO_Port GPIOG
#define AUTO_CHOKE_ENABLE_Pin GPIO_PIN_1
#define AUTO_CHOKE_ENABLE_GPIO_Port GPIOG
#define LIMIT_1_Pin GPIO_PIN_4
#define LIMIT_1_GPIO_Port GPIOG
#define LIMIT_2_Pin GPIO_PIN_5
#define LIMIT_2_GPIO_Port GPIOG
#define LTDC_DISP_Pin GPIO_PIN_6
#define LTDC_DISP_GPIO_Port GPIOG
#define WHEEL_SPEED_Pin GPIO_PIN_15
#define WHEEL_SPEED_GPIO_Port GPIOA
#define _1_GEAR_Pin GPIO_PIN_10
#define _1_GEAR_GPIO_Port GPIOC
#define _2_GEAR_Pin GPIO_PIN_11
#define _2_GEAR_GPIO_Port GPIOC
#define _3_GEAR_Pin GPIO_PIN_12
#define _3_GEAR_GPIO_Port GPIOC
#define _4_GEAR_Pin GPIO_PIN_0
#define _4_GEAR_GPIO_Port GPIOD
#define _5_GEAR_Pin GPIO_PIN_1
#define _5_GEAR_GPIO_Port GPIOD
#define _6_GEAR_Pin GPIO_PIN_2
#define _6_GEAR_GPIO_Port GPIOD
#define N_GEAR_Pin GPIO_PIN_4
#define N_GEAR_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
