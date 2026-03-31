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
#define TIME_2700HZ 370

#define TIME_C7		477
#define TIME_C#7	451
#define TIME_D7		425
#define TIME_D#7	401
#define TIME_E7		379
#define TIME_F7		357
#define TIME_F#7	337
#define TIME_G7		318
#define TIME_G#7	301
#define TIME_A7		284
#define TIME_A#7	268
#define TIME_B7		253
#define TIME_C8		238

void SPEAKER_Initialize();
void SPEAKER_PlaySound(uint16_t freq,uint16_t duration);
void SPEAKER_Stop();

#endif /* INC_SPEAKER_H_ */
