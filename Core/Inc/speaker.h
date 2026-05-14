/**
 * @file	speaker.h
 * @brief	header file for speaker function.
 * @author	remov-b4-flight
 * @copyright	3-Clause BSD License
 */
#ifndef INC_SPEAKER_H_
#define INC_SPEAKER_H_

#include <stdint.h>

#define SPEAKER_TIMER_STOP	0
#define SPEAKER_TIMER_0R1S	4
#define SPEAKER_TIMER_0R2S	8
#define SPEAKER_TIMER_0R5S	21
#define SPEAKER_TIMER_1S	42
#define SPEAKER_TIMER_CONTINUOUS 0xff

#define FREQ_D7		425
#define FREQ_DS7	401
#define FREQ_2700HZ	370
#define FREQ_E7		379
#define FREQ_F7		357
#define FREQ_G7		318

void SPEAKER_Initialize();
void SPEAKER_PlaySound(uint16_t freq,uint8_t duration);
void SPEAKER_Stop();

#endif /* INC_SPEAKER_H_ */
