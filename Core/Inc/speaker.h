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
#define FREQ_2700HZ	370

#define FRFEQ_C7	477
#define FRFEQ_CS7	451
#define FRFEQ_D7	425
#define FRFEQ_DS7	401
#define FRFEQ_E7	379
#define FRFEQ_F7	357
#define FRFEQ_FS7	337
#define FRFEQ_G7	318
#define FRFEQ_GS7	301
#define FRFEQ_A7	284
#define FRFEQ_AS7	268
#define FRFEQ_B7	253
#define FRFEQ_C8	238

void SPEAKER_Initialize();
void SPEAKER_PlaySound(uint16_t freq,uint16_t duration);
void SPEAKER_Stop();

#endif /* INC_SPEAKER_H_ */
