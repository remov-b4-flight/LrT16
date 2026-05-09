/**
 *	@file		speaker.c
 *	@brief		speaker control functions
 *	@author		remov-b4-flight
 *	@copyright	3-Clause BSD License
*/
#include <stdbool.h>
#include "speaker.h"
#include "main.h"
extern TIM_HandleTypeDef htim1;
uint8_t SPEAKER_Timer;
bool SPEAKER_Timer_Enable;

void SPEAKER_Initialize() {
	SPEAKER_Timer_Enable = false;
	SPEAKER_Timer = SPEAKER_TIMER_STOP;
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}

void SPEAKER_PlaySound(uint16_t freq, uint8_t duration) {
	SPEAKER_Timer = duration;
	SPEAKER_Timer_Enable = true;
	//
	htim1.Instance->ARR = freq;
	htim1.Instance->CCR1 = (freq / 2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void SPEAKER_Stop() {
	SPEAKER_Timer_Enable = false;
	SPEAKER_Timer = SPEAKER_TIMER_STOP;
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}
