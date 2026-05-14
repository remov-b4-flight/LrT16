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

/**
 * @brief Initialize speaker
 */
inline void SPEAKER_Initialize() {
	SPEAKER_Stop();
}

/**
 * @brief Play single tone.
 * @param frequency (defined in speaker.h)
 * @param duration (24ms unit, defined in speaker.h)
 */
void SPEAKER_PlaySound(uint16_t freq, uint8_t duration) {
	SPEAKER_Timer = duration;
	SPEAKER_Timer_Enable = true;
	//
	htim1.Instance->ARR = freq;
	htim1.Instance->CCR1 = (freq / 2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

/**
 * @brief Stop sound from speaker
 */
void SPEAKER_Stop() {
	SPEAKER_Timer_Enable = false;
	SPEAKER_Timer = SPEAKER_TIMER_STOP;
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
}
