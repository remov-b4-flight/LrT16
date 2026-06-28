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
//! @defgroup Define LrTMAX version.
#define USBD_DEVICE_VER	0x0099
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
//! Build option if you need not use scene timeouts, un-comment this.
//#define NO_SCENE_TO	1
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Start_LongTimer(uint32_t tick);
void Msg_Print();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM_PERIOD_NPIX 279
#define Lr_PRODUCT "LrT16"
#define PWM_HI 38
#define TIM_PERIOD_RGBLED 24576
#define PWM_LO 15
#define PWM_PERIOD 59
#define Lr_PID 0xB747
#define TIM_PERIOD_ENC 1600
#define TIM_PRESC_100uS 4799
#define Lr_VENDOR "Ruffles Inc."
#define TIM_PERIOD_SPEAKER 370
#define TIM_PRESC_1uS 47
#define TIM_PERIOD_MATRIX 7999
#define TIM_SPEAKER_PULSE_WIDTH 185
#define TIM_PERIOD_OLED 32767
#define E6B_Pin GPIO_PIN_13
#define E6B_GPIO_Port GPIOC
#define E7A_Pin GPIO_PIN_14
#define E7A_GPIO_Port GPIOC
#define E7B_Pin GPIO_PIN_15
#define E7B_GPIO_Port GPIOC
#define L0_Pin GPIO_PIN_0
#define L0_GPIO_Port GPIOF
#define L1_Pin GPIO_PIN_1
#define L1_GPIO_Port GPIOF
#define E0A_Pin GPIO_PIN_0
#define E0A_GPIO_Port GPIOC
#define E0B_Pin GPIO_PIN_1
#define E0B_GPIO_Port GPIOC
#define E1A_Pin GPIO_PIN_2
#define E1A_GPIO_Port GPIOC
#define E1B_Pin GPIO_PIN_3
#define E1B_GPIO_Port GPIOC
#define M0_Pin GPIO_PIN_0
#define M0_GPIO_Port GPIOA
#define M1_Pin GPIO_PIN_1
#define M1_GPIO_Port GPIOA
#define M2_Pin GPIO_PIN_2
#define M2_GPIO_Port GPIOA
#define M3_Pin GPIO_PIN_3
#define M3_GPIO_Port GPIOA
#define SW18_Pin GPIO_PIN_4
#define SW18_GPIO_Port GPIOA
#define SW17_Pin GPIO_PIN_5
#define SW17_GPIO_Port GPIOA
#define LED_DON_Pin GPIO_PIN_6
#define LED_DON_GPIO_Port GPIOA
#define E2A_Pin GPIO_PIN_4
#define E2A_GPIO_Port GPIOC
#define E2B_Pin GPIO_PIN_5
#define E2B_GPIO_Port GPIOC
#define E8A_Pin GPIO_PIN_0
#define E8A_GPIO_Port GPIOB
#define E8B_Pin GPIO_PIN_1
#define E8B_GPIO_Port GPIOB
#define E9A_Pin GPIO_PIN_2
#define E9A_GPIO_Port GPIOB
#define E13A_Pin GPIO_PIN_10
#define E13A_GPIO_Port GPIOB
#define E13B_Pin GPIO_PIN_11
#define E13B_GPIO_Port GPIOB
#define E14A_Pin GPIO_PIN_12
#define E14A_GPIO_Port GPIOB
#define E14B_Pin GPIO_PIN_13
#define E14B_GPIO_Port GPIOB
#define E15A_Pin GPIO_PIN_14
#define E15A_GPIO_Port GPIOB
#define E15B_Pin GPIO_PIN_15
#define E15B_GPIO_Port GPIOB
#define E3A_Pin GPIO_PIN_6
#define E3A_GPIO_Port GPIOC
#define E3B_Pin GPIO_PIN_7
#define E3B_GPIO_Port GPIOC
#define E4A_Pin GPIO_PIN_8
#define E4A_GPIO_Port GPIOC
#define E4B_Pin GPIO_PIN_9
#define E4B_GPIO_Port GPIOC
#define SP_Pin GPIO_PIN_8
#define SP_GPIO_Port GPIOA
#define L3_Pin GPIO_PIN_9
#define L3_GPIO_Port GPIOA
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define E5A_Pin GPIO_PIN_10
#define E5A_GPIO_Port GPIOC
#define E5B_Pin GPIO_PIN_11
#define E5B_GPIO_Port GPIOC
#define E6A_Pin GPIO_PIN_12
#define E6A_GPIO_Port GPIOC
#define L2_Pin GPIO_PIN_2
#define L2_GPIO_Port GPIOD
#define E9B_Pin GPIO_PIN_3
#define E9B_GPIO_Port GPIOB
#define E10A_Pin GPIO_PIN_4
#define E10A_GPIO_Port GPIOB
#define E10B_Pin GPIO_PIN_5
#define E10B_GPIO_Port GPIOB
#define E11A_Pin GPIO_PIN_6
#define E11A_GPIO_Port GPIOB
#define E11B_Pin GPIO_PIN_7
#define E11B_GPIO_Port GPIOB
#define E12A_Pin GPIO_PIN_8
#define E12A_GPIO_Port GPIOB
#define E12B_Pin GPIO_PIN_9
#define E12B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

//! LED TIM3 definitions
#define LED_TIM_RETRY_WAIT	21		//Transfer period for I2C
//! ON boot check bit patter for enter DFU
#define BOOT_DFU_MASK	0x0030
#define LED_PULSE_1S	42
#define LED_PULSE_2S	83
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
