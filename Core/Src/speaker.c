/**
 *	@file		speaker.c
 *	@brief		speaker control functions
 *	@author		remov-b4-flight
 *	@copyright	3-Clause BSD License
*/
#include "speaker.h"
#include "main.h"
extern TIM_HandleTypeDef htim1;
uint8_t SPEAKER_Timer;

void SPEAKER_Initialize() {
	SPEAKER_Timer = SPEAKER_TIMER_CONTINUOUS;
	HAL_TIM_Base_Stop(&htim1);
	HAL_GPIO_WritePin(SP_GPIO_Port, SP_Pin, GPIO_PIN_RESET);
}

void SPEAKER_PlaySound(uint16_t freq, uint16_t duration){
	htim1.Init.Period = freq;
	HAL_TIM_Base_Start(&htim1);
}

void SPEAKER_Stop() {
	HAL_TIM_Base_Stop(&htim1);
	HAL_GPIO_WritePin(SP_GPIO_Port, SP_Pin, GPIO_PIN_RESET);
}
