/*
 * @file		EmulateMIDI.h
 * @brief		Definitions	for EmulateMIDI() function
 * @author		remov-b4-flight
 * @copyright	3-Clause BSD License
 */

#ifndef INC_EMULATEMIDI_H_
#define INC_EMULATEMIDI_H_
#include "main.h"
#include "midi.h"
#include "usbd_midi_if.h"
#include "bitcount.h"
#include "led.h"
#include "speaker.h"

//! define switch combination to invoke reset
#define RESET_SW_PATTERN	0x3	// in cc_bits.mix.b [Undo]+[Scene]
#define	SCENE_MSK	0x3

//! @defgroup number of cc channels that LrTMAX controls.
#define CC_COUNT_INUSE	(ENC_COUNT * SCENE_COUNT)
#define CC_MAX_INUSE	(CC_CH_OFFSET + CC_COUNT_INUSE)
#define CC_MIN_INUSE	CC_CH_OFFSET

//! Received from MIDI channel-value pair
typedef union ch_val_t {
	uint16_t	wd;
	struct cval_t {
		uint8_t	ch;	//! < MIDI channel
		uint8_t	val;//! < MIDI value
	} by;
}CH_VAL;

//! Bit position for 1st encoder
#define PROF_ENC1ST	16

void EmulateMIDI();
void EmulateMIDI_Init();

#endif /* INC_EMULATEMIDI_H_ */
