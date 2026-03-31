/**
 * @file	speaker.h
 * @brief	header file for speaker function.
 * @author	remov-b4-flight
 * @copyright	3-Clause BSD License
 */
#ifndef INC_SPEAKER_H_
#define INC_SPEAKER_H_

#include <stdint.h>

#define SPEAKER_TIMER_CONTINUOUS 0xff
#define FREQ_2700HZ 370
#define FREQ_F7 357
#define FREQ_E7 379
#define FREQ_D7 425

void SPEAKER_Initialize();
void SPEAKER_PlaySound(uint16_t freq,uint16_t duration);
void SPEAKER_Stop();

#endif /* INC_SPEAKER_H_ */
