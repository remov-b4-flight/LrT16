/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

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
#define TIM_PERIOD_NPIX 279
#define Lr_PRODUCT "LrT16"
#define PWM_HI 38
#define TIM_PERIOD_RGBLED 24576
#define PWM_LO 15
#define PWM_PERIOD 59
#define Lr_PID 0xA380
#define TIM_PERIOD_ENC 1600
#define TIM_PRESC_100uS 4799
#define Lr_VENDOR "Ruffles Inc."
#define TIM_PRESC_1uS 47
#define TIM_PERIOD_MATRIX 7999
#define TIM_PERIOD_OLED 32767
#define L0_Pin GPIO_PIN_13
#define L0_GPIO_Port GPIOC
#define L1_Pin GPIO_PIN_14
#define L1_GPIO_Port GPIOC
#define L2_Pin GPIO_PIN_15
#define L2_GPIO_Port GPIOC
#define SW2_Pin GPIO_PIN_2
#define SW2_GPIO_Port GPIOA
#define LED_DON_Pin GPIO_PIN_6
#define LED_DON_GPIO_Port GPIOA
#define M0_Pin GPIO_PIN_0
#define M0_GPIO_Port GPIOB
#define M1_Pin GPIO_PIN_1
#define M1_GPIO_Port GPIOB
#define M2_Pin GPIO_PIN_2
#define M2_GPIO_Port GPIOB
#define M10_Pin GPIO_PIN_10
#define M10_GPIO_Port GPIOB
#define M11_Pin GPIO_PIN_11
#define M11_GPIO_Port GPIOB
#define M12_Pin GPIO_PIN_12
#define M12_GPIO_Port GPIOB
#define M13_Pin GPIO_PIN_13
#define M13_GPIO_Port GPIOB
#define M14_Pin GPIO_PIN_14
#define M14_GPIO_Port GPIOB
#define M15_Pin GPIO_PIN_15
#define M15_GPIO_Port GPIOB
#define SP_Pin GPIO_PIN_8
#define SP_GPIO_Port GPIOA
#define SW_Pin GPIO_PIN_10
#define SW_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define M3_Pin GPIO_PIN_3
#define M3_GPIO_Port GPIOB
#define M4_Pin GPIO_PIN_4
#define M4_GPIO_Port GPIOB
#define M5_Pin GPIO_PIN_5
#define M5_GPIO_Port GPIOB
#define M6_Pin GPIO_PIN_6
#define M6_GPIO_Port GPIOB
#define M7_Pin GPIO_PIN_7
#define M7_GPIO_Port GPIOB
#define M8_Pin GPIO_PIN_8
#define M8_GPIO_Port GPIOB
#define M9_Pin GPIO_PIN_9
#define M9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
