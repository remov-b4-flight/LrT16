/**
 * @file	speaker.h
 * @brief	header file for speaker function.
 * @author	remov-b4-flight
 * @copyright	3-Clause BSD License
 */
#ifndef INC_SPEAKER_H_
#define INC_SPEAKER_H_

#include <stdint.h>

	SPEAKER_Initialize();
	SPEAKER_PlaySound(uint16_t freq,uint16_t duration);
	SPEAKER_Stop();

#endif /* INC_SPEAKER_H_ */
