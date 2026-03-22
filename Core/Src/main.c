/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file		main.c
  * @brief		Main program body
  * @author		remov-b4-flight
  * @copyright	3-Clause BSD License
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "LrCommon.h"
#include "midi.h"
#include "led.h"
#include "bitcount.h"
#include "usbd_midi_if.h"
#include "EmulateMIDI.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_hal_dma.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//extern	uint8_t	LED_Scene[SCENE_COUNT][LED_COUNT];
extern	uint8_t	LEDColor[LED_COUNT];
extern	uint8_t	LEDTimer[LED_COUNT];
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim14;
DMA_HandleTypeDef hdma_tim3_ch1_trig;

/* USER CODE BEGIN PV */

//! STM32 TIM3 instance handle
TIM_HandleTypeDef htim3;
//! LrTMAX USB connection state
uint8_t	LrState;
//! LrTMAX Scene index
uint8_t	LrScene;
//! In key scanning whether Line selected to read for key matrix.
uint8_t	ENCSW_Line;
// OLED variables
//! Flag set by timer ISR, It makes 'off' OLES contents.
bool 	Msg_Timer_Update;
//! Timer counter ticked by TIM7.
int32_t	Msg_Timer_Count;
//! If true Msg_Timer counting is enabled.
bool	Msg_Timer_Enable;
//! If true, Screen is cleared in main() that is determined on timer interrupt.
bool	Msg_Off_Flag;
//! If true, Screen is flashed by [] at main() function.
static	bool	isMsgFlash;
//! If true, frame_buffer[] contents flashes the screen.
static	bool	isRender;

//! LED variables
//! If true, LEDs are flashed by LEDColor[] array.
bool	isLEDsendpulse;
//! Flag is set by timer ISR, It makes LED_Timer[] count up in main()
bool	LED_Timer_Update;

//! Scene time out
bool	isScene_Timeout;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM14_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void Jump2SystemMemory();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 *	@brief	Stop encoder scan
 */
static inline void Stop_All_Encoders() {
	HAL_TIM_Base_Stop_IT(&htim2);
}

/**
 * @brief	Start encoder scan by timer
 */
static inline void Start_All_Encoders() {
	ENC_Init();
	HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief Start OLED off timer
*/
void Start_MsgTimer(uint32_t tick){
	Msg_Off_Flag = false;
	Msg_Timer_Count = tick;
	Msg_Timer_Enable = true;
}
/**
 * @brief	raise flag to message flash
 */
inline void Msg_Print() {
	isMsgFlash = true;
}

/**
 * @brief start/stop matrix L0-L3 control
 * @param control Lr_MATRIX_START / Lr_MATRIX_STOP
 */
static void Matrix_Control(uint8_t control) {
	if (control == Lr_MATRIX_START) {
//		MTRX_Init();
	}

	HAL_GPIO_WritePin(L0_GPIO_Port, L0_Pin, (control == Lr_MATRIX_START)? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L1_GPIO_Port, L1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L2_GPIO_Port, L2_Pin, GPIO_PIN_RESET);
	ENCSW_Line = L0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	//! Indicates 1st Msg_Timer timeout has occurred from power on reset.
	bool Msg_1st_timeout = true;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	Msg_Off_Flag = false;
	Msg_Timer_Enable = false;
	Msg_Timer_Count = MSG_TIMER_DEFAULT;
	isMsgFlash = false;
	isRender = true;

	LrState = LR_RESET;
	LrScene = Lr_SCENE0;

	isLEDsendpulse = false;
	Msg_Timer_Update = false;
	LED_Timer_Update = false;
	isScene_Timeout = false;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM1_Init();
  MX_TIM14_Init();
/* USER CODE BEGIN MX_USB_Devive_Init LrTMAX*/
//  MX_USB_DEVICE_Init(); must be delayed.
/* USER CODE END MX_USB_Devive_Init LrTMAX*/
  MX_TIM6_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	// Set all LEDs to 'OFF'
	LED_Initialize();
	// Stop All Encoders until USB link up
	Stop_All_Encoders();
	//Initialize Switch matrix
	HAL_GPIO_WritePin(L0_GPIO_Port, L0_Pin, GPIO_PIN_SET);	// Initialize L0-3.

	// Initialize series of WS2812C
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR6_0;	// Pull up PA6 (WS2812C-2020 workaround)
	GPIOA->ODR |= GPIO_PIN_6;				//'RESET' state
	//AF -> GPIO
	GPIOA->MODER &= ~(GPIO_MODER_MODER6_1);
	GPIOA->MODER |=	GPIO_MODER_MODER6_0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint32_t	nc_count = 0;

	HAL_TIM_Base_Start_IT(&htim6);		//Start LED timer.
	Start_MsgTimer(MSG_TIMER_DEFAULT);

	// Check SW1 and SW3 is pushed at Power On
	if (0) { // MTD: define key combination
		LrState = LR_USB_DFU;
	} else {
		// MX_USB_DEVICE_Init() must be delayed until here for launch DFU.
		MX_USB_DEVICE_Init();
		LrState = LR_USB_NOLINK;
	}

	// Initialize CC Value table
	EmulateMIDI_Init();

	// LED Initialize
	LED_SetScene(LrScene);

	// Main loop
	while (1) {
		if (LrState == LR_USB_LINKUP) {
			// USB device configured by host

			Matrix_Control(Lr_MATRIX_START);	// Initialize L0-3.
			HAL_TIM_Base_Start_IT(&htim1);		// Start Switch matrix timer.
			Start_All_Encoders();				// Start rotary encoder.

			// Connection banner
			Start_MsgTimer(MSG_TIMER_CONNECT);
//			memcpy(LEDColor, LED_Scene[LrScene], LED_COUNT);
			LED_SetPulse(LED_IDX_ENC0, LED_PINK, LED_TIM_CONNECT);
			LrState = LR_USB_LINKED;

		} else if (LrState == LR_USB_LINKED) {
			// Operate as MIDI Instruments.
			EmulateMIDI();
		} else if (LrState == LR_USB_LINK_LOST) {
			Stop_All_Encoders();

			HAL_TIM_Base_Stop(&htim1);
			Matrix_Control(Lr_MATRIX_STOP);		// Stop L0-L3

			LED_TestPattern();
			Msg_1st_timeout = false;
			Start_MsgTimer(MSG_TIMER_DEFAULT);
			nc_count = 0;
			LrState = LR_USB_NOLINK;

		} else if (LrState == LR_USB_NOLINK) {
			// USB can't be configured or disconnected by host.
			if (Msg_Off_Flag == true) {
				if (Msg_1st_timeout == true) {
					LrState = LR_USB_LINK_LOST;
				} else { // 2nd or more

					Msg_Print();

					// Restart OLED timer.
					Start_MsgTimer(MSG_TIMER_NOLINK);

					// Rotate LED colors
					uint8_t	tempcolor = LEDColor[7];
					LEDColor[7] = LEDColor[6];
					LEDColor[6] = LEDColor[5];
					LEDColor[5] = LEDColor[4];
					LEDColor[4] = LEDColor[3];
					LEDColor[3] = LEDColor[2];
					LEDColor[2] = LEDColor[1];
					LEDColor[1] = LEDColor[0];
					LEDColor[0] = tempcolor;

					isLEDsendpulse = true;
				}
			}// Msg_Off_Flag
		} else if (LrState == LR_USB_DFU) {
			if (Msg_Off_Flag == true) {
				if (nc_count == 0) {
					// Show DFU banner
					Msg_Print();
					nc_count++;
				} else if(nc_count == 1) {
					// Show LED pattern
					LED_TestPattern();
					nc_count++;
				} else if (nc_count <= 2) {
					LED_Initialize();
					// Jump to BOOTLOADER
					Jump2SystemMemory();
				}
				Start_MsgTimer(MSG_TIMER_NOLINK/2);
			}

		}// LrState

		// LED Timer
		if (LED_Timer_Update == true) { // 24ms interval
			for (uint8_t i = 0; i < LED_COUNT; i++) {
				if (LEDTimer[i] != LED_TIMER_CONSTANT && --LEDTimer[i] == 0) {
//					LED_SetPulse(i, LED_Scene[LrScene][i], LED_TIMER_CONSTANT);
				}
			}
			LED_Timer_Update = false;
			continue;
		}

		// Flashing LEDs
		if (isLEDsendpulse == true) {
			if (LED_SendPulse() == true) {
				isLEDsendpulse = false;
			} else {
				HAL_Delay(LED_TIM_RETRY_WAIT);	// i2c is busy, retry with interval
			}
			continue;
		}

		HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = TIM_PRESC_1uS;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 5400;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 2700;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = TIM_PRESC_1uS;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = TIM_PERIOD_ENC;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = PWM_PERIOD;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = TIM_PRESC_1uS;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = TIM_PERIOD_RGBLED;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = TIM_PRESC_1uS;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = TIM_PERIOD_NPIX;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */
  htim14.Instance->CR1 |= TIM_CR1_OPM;
  /* USER CODE END TIM14_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_5_6_7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, L0_Pin|L1_Pin|L2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, M4_Pin|M5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : L0_Pin L1_Pin L2_Pin */
  GPIO_InitStruct.Pin = L0_Pin|L1_Pin|L2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SW2_Pin SW_Pin */
  GPIO_InitStruct.Pin = SW2_Pin|SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : M0_Pin M1_Pin M2_Pin M10_Pin
                           M11_Pin M12_Pin M13_Pin M14_Pin
                           M15_Pin M3_Pin M6_Pin M7_Pin
                           M8_Pin M9_Pin */
  GPIO_InitStruct.Pin = M0_Pin|M1_Pin|M2_Pin|M10_Pin
                          |M11_Pin|M12_Pin|M13_Pin|M14_Pin
                          |M15_Pin|M3_Pin|M6_Pin|M7_Pin
                          |M8_Pin|M9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : M4_Pin M5_Pin */
  GPIO_InitStruct.Pin = M4_Pin|M5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
