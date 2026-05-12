/**
 * @file		EmulateMIDI.c
 * @brief		MIDI Emulation of LrTMAX
 * @author		remov-b4-flight
 * @copyright	3-Clause BSD License
 */
#include "EmulateMIDI.h"
#include "queue.h"
#include "stm32f0xx_it.h"

extern	uint8_t	LrScene;
extern	USBD_HandleTypeDef *pInstance;
extern	bool isScene_Timeout;
extern	bool Soft_Timer_Update;
extern	SW_SCAN prev_sidesw_push;
//! keeps previous 'Note On' note number For sending 'Note Off' message.
uint8_t	prev_note;
//! If true, MIDI message previous sent is switch. If false, it's encoder
bool	isPrev_SwPush;
//! Indicate scene name appearing on OLED. need clearing buffers.
bool	isPrev_Scene;

// MIDI variables
//! MIDI CC message value for each channels.
uint8_t	MIDI_CC_Value[CC_CH_COUNT];
//! Queing received MIDI message;
QUEUE	midi_rx_que;

// keyboard variable
//! If true, ISR detected any Switch was pushed.
bool	isAnySwitchPushed;
//! If true ISR detected any Encoder was moved.
bool	isAnyEncoderMoved;
//! Switch pressed status set by timer scanning.
MTX_SCAN	MTX_Stat;
//! Encoder moved status set by timer scanning.
ENC_SCAN	ENC_Stat;
//! USB MIDI message structure for send
MIDI_MSG	MIDI_TxMessage;
//! Encoder move information
ENC_MOVE	enc_move;
//! LP(Long Press) timer
int8_t		LP_Timer;
bool		LP_Timer_Enable;

/**
 * @brief	Rise CC message value
 * @param	channel to modify
 */
static bool MIDI_CC_Inc(uint8_t channel) {
	if (MIDI_CC_Value[channel] < MIDI_CC_MAX ) {
		MIDI_CC_Value[channel]++;
		return true;
	} else {
		return false;
	}
}

/**
 * @brief	Fall CC message value
 * @param	channel to modify
 */
static bool MIDI_CC_Dec(uint8_t channel) {
	if (MIDI_CC_Value[channel] >= (MIDI_CC_MIN+1) ) {
		MIDI_CC_Value[channel]--;
		return true;
	} else {
		return false;
	}
}

/**
 * 	@brief	Initialize MIDI value memory, queue.
 */
void EmulateMIDI_Init() {
	LP_Timer = LP_TIM_NORM;
	LP_Timer_Enable = false;
	isPrev_SwPush = false;
	isPrev_Scene = false;
	memset(MIDI_CC_Value, MIDI_CC_INITIAL, CC_CH_COUNT);
	queue_init(&midi_rx_que);
}

/**
 *	@brief	Generate MIDI message and Send to host by User interaction.
 *	@pre	isAnySwitchPushed 	any Switches was pressed or not
 *	@pre	isAnyEncodersMoved 	any Encoders was moved or not
 *	@pre	ENCSW_Stat	current Switches/Encoders status
 */
void EmulateMIDI() {
	bool isSendMIDIMessage = false;
	bool isLEDflash = false;
	uint8_t	bitpos;
	if (queue_isempty(&midi_rx_que) != true) { // Check MIDI CC message received.
		CH_VAL rx;
		uint8_t cc_scene = 0;
		do {
			rx.wd = queue_dequeue(&midi_rx_que);
			cc_scene = (rx.by.ch / CC_CH_PER_SCENE) & CH_SCENE_MASK;
			if ( CC_MIN_INUSE <= rx.by.ch && rx.by.ch < CC_MAX_INUSE ) {
				MIDI_CC_Value[rx.by.ch] = rx.by.val;
			}
		} while  (queue_isempty(&midi_rx_que) != true);
		if (isPrev_Scene == true) {
			isPrev_Scene = false;
		}
		Start_LongTimer(MSG_TIMER_DEFAULT);
	} else if (isAnySwitchPushed == true) {
		bitpos = ntz16(MTX_Stat.line.n2.u16.side_sw.u16);

		if ( MTX_Stat.line.n2.u32 != 0) { //Check Matrix switches/encoders
			//Send 'Note On' message from switches/encoders matrix.
			uint8_t	note;
			if (MTX_Stat.line.n2.u16.side_sw.bits.sw17 == 1
				&& MTX_Stat.line.n2.u16.side_sw.bits.sw18 == 1) { // RESET command
				HAL_NVIC_SystemReset();
			} else if (MTX_Stat.line.n2.u16.side_sw.bits.sw18 == 1) {
				note = NOTE_SCENE_SW; //is [SCENE] switch pressed?
			   	//Move to next Scene.
				if ( (++LrScene) > SCENE_COUNT  ) {
					LrScene = Lr_SCENE1;
				} else if ( isScene_Timeout == true ) {
					LrScene = Lr_SCENE1;
					isScene_Timeout = false;
				}
				SPEAKER_PlaySound(FREQ_D7, SPEAKER_TIMER_0R2S);
				LED_SetScene(LrScene);
				isPrev_Scene = true;

				isSendMIDIMessage = true;
			} else if (MTX_Stat.line.n2.u16.side_sw.bits.sw17 == 1) {
				MTX_Stat.line.n2.u16.side_sw.bits.sw17 = 0;
				// run LP_Timer for determine long push
				LP_Timer = LP_TIM_NORM;
				LP_Timer_Enable = true;
			} else if (MTX_Stat.line.n2.u16.side_sw.bits.sw17lp == 1) {
				MTX_Stat.line.n2.u16.side_sw.bits.sw17lp = 0;
				note = NOTE_FUNC_LP;
				SPEAKER_PlaySound(FREQ_G7, SPEAKER_TIMER_0R1S);
				isSendMIDIMessage = true;
			} else if (MTX_Stat.line.n2.u16.side_sw.bits.sw17sp == 1) {
				MTX_Stat.line.n2.u16.side_sw.bits.sw17sp = 0;
				note = NOTE_FUNC_SW;
				SPEAKER_PlaySound(FREQ_E7, SPEAKER_TIMER_0R1S);
				isSendMIDIMessage = true;
			} else {
				note = (LrScene * NOTES_PER_SCENE) + bitpos;
				SPEAKER_PlaySound(FREQ_E7, SPEAKER_TIMER_0R1S);
				isSendMIDIMessage = true;
			}

			//Print Message to OLED & LED
			Start_LongTimer(MSG_TIMER_DEFAULT);

			if (isSendMIDIMessage == true) {
				//Set 'Note On' message
				MIDI_TxMessage.header = MIDI_NT_ON;
				MIDI_TxMessage.status = MIDI_NT_ON_S;
				MIDI_TxMessage.channel = note;
				MIDI_TxMessage.value = MIDI_NT_ON_VELOCITY;

				prev_note = note;
				isPrev_SwPush = true;
				isLEDflash = true;
			}
		} else if (isPrev_SwPush == true) {// Is switch/encoder push released?
			//Set 'Note Off' message.
			MIDI_TxMessage.header = MIDI_NT_OFF;
			MIDI_TxMessage.status = MIDI_NT_OFF_S;
			MIDI_TxMessage.channel = prev_note;
			MIDI_TxMessage.value = MIDI_NT_OFF_VELO;

			isSendMIDIMessage = true;
			isPrev_SwPush = false;
		}
		isAnySwitchPushed = false;
	} else if ( isAnyEncoderMoved == true) { //check encoder movements
		uint8_t	axis = enc_move.bits.axis;
		uint8_t channel = (LrScene * CC_CH_PER_SCENE) + axis;

		bitpos = PROF_ENC1ST + (axis * 2);

		if (enc_move.bits.move == ENC_MOVE_CW) {
			isSendMIDIMessage = MIDI_CC_Inc(channel);
		} else if (enc_move.bits.move == ENC_MOVE_CCW) {
			isSendMIDIMessage = MIDI_CC_Dec(channel);
			bitpos += 1; // for LED_SetPulse()
		} else {
			goto rot_stopped_exits;
		}
		isLEDflash = isSendMIDIMessage;

		MIDI_TxMessage.header = MIDI_CC_HEADER;
		MIDI_TxMessage.status = MIDI_CC_STATUS;
		MIDI_TxMessage.channel = channel;
		MIDI_TxMessage.value = MIDI_CC_Value[channel];

		isPrev_SwPush = false;

		//Print Message to OLED & LEDs.
		Start_LongTimer(MSG_TIMER_DEFAULT);


rot_stopped_exits:
		isAnyEncoderMoved = false;
	}
	//Send MIDI message
	if (isSendMIDIMessage == true) {
		//Send MIDI message via USB.
		if (USBD_LL_Transmit(pInstance, MIDI_IN_EP, (uint8_t *)&MIDI_TxMessage, MIDI_MESSAGE_LENGTH) == USBD_OK) {
			isSendMIDIMessage = false;
		}
	}

	if (Soft_Timer_Update == true) { // 24ms interval
		if (LP_Timer_Enable == true && --LP_Timer == 0) {
			// shortcut for sw17 release under LPTimer running
			uint16_t s17 = (SSW_GPIO_Port->IDR & SW17_Pin);
			if (s17 != 0 ) { // released ?
				MTX_Stat.line.n2.u16.side_sw.bits.sw17sp = 1;
			} else  {
				MTX_Stat.line.n2.u16.side_sw.bits.sw17lp = 1;
			}
			isAnySwitchPushed = true;
			LP_Timer_Enable = false;
			LP_Timer = 0;
		}
	}

	// Flash LED.
	if (isLEDflash == true) {
		uint8_t value;
		uint8_t color0;
		uint8_t	color1;
		if (MIDI_TxMessage.header == MIDI_NT_ON) {
			value = MIDI_TxMessage.channel;
			color0 = LED_DARK;
			color1 = LED_ORANGE;
		} else {
			value = MIDI_TxMessage.value;
			color0 = LED_DARK;
			color1 = LED_GREEN;
		}
		LED_SetValue(value, color0, color1, MIDI_CC_BITS, LED_TIM_NORM);
	}

}
