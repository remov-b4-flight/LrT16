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
#define SPEAKER_TIMER_0R2S	8
#define SPEAKER_TIMER_0R5S	21
#define SPEAKER_TIMER_1S	42
#define FREQ_2700HZ	370

#define FREQ_C7		477
#define FREQ_CS7	451
#define FREQ_D7		425
#define FREQ_DS7	401
#define FREQE7		379
#define FREQ_F7		357
#define FREQ_FS7	337
#define FREQ_G7		318
#define FREQ_GS7	301
#define FREQ_A7		284
#define FREQ_AS7	268
#define FREQ_B7		253
#define FREQ_C8		238

void SPEAKER_Initialize();
void SPEAKER_PlaySound(uint16_t freq,uint8_t duration);
void SPEAKER_Stop();

#endif /* INC_SPEAKER_H_ */
