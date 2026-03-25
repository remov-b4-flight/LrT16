/*
 * @file	LrCommon.h
 * @brief	Common definitions on LrTMAX
 * @author	remov-b4-flight
 * @copyright	3-Clause BSD License
*/

#ifndef INC_LRCOMMON_H_
#define INC_LRCOMMON_H_
#include <stdint.h>

//! @defgroup LrTMAX Hardware definition
#define SW_COUNT		8
#define ENC_COUNT		16
#define ENC_SW_COUNT	(SW_COUNT + ENC_COUNT)

//! Type for Encoders
typedef union enc_scan_t {
    uint32_t lo;
    struct enc_bits_t {
		uint8_t enc0:2;	//! < Rotary encoder0
		uint8_t enc1:2;	//! < Rotary encoder1
		uint8_t enc2:2;	//! < Rotary encoder2
		uint8_t enc3:2;	//! < Rotary encoder3
		uint8_t enc4:2;	//! < Rotary encoder4
		uint8_t enc5:2;	//! < Rotary encoder5
		uint8_t enc6:2;	//! < Rotary encoder6
		uint8_t enc7:2;	//! < Rotary encoder7
		uint8_t enc8:2;	//! < Rotary encoder8
		uint8_t enc9:2;	//! < Rotary encoder9
		uint8_t enc10:2;	//! < Rotary encoder10
		uint8_t enc11:2;	//! < Rotary encoder11
		uint8_t enc12:2;	//! < Rotary encoder12
		uint8_t enc13:2;	//! < Rotary encoder13
		uint8_t enc14:2;	//! < Rotary encoder14
		uint8_t enc15:2;	//! < Rotary encoder15
    } nb;
} ENC_SCAN;

//! Type for switch matrix
typedef union mtrx_scan_t {
    uint64_t ll;
    struct ks_bits_t {
		uint16_t n0;	//! < Switch Line0
		uint16_t n1;	//! < Switch Line1
		uint16_t n2;	//! < Switch Line2
		uint16_t b0:1;	//! < un-matrixed switch undo
		uint16_t b1:1;	//! < un-matrixed switch scene
		uint16_t dummy:14;
    } sh;
    struct cc_bits16_t {
    	uint32_t n01;
    	uint16_t n2;
		uint16_t b:2;	//! < un-matrixed switch undo
		uint16_t dummy:14;
    } mix;
    struct cc_bits32_t {
    	uint32_t n01;
    	uint32_t n2b:18; // combinated n2*b
		uint16_t dummy:14;
    } m2;
} MTX_SCAN;

//! Encoder movement expression
typedef union enc_m_t {
	uint8_t	wd;
	struct enc_mm_t {
		uint8_t	move:2;	//! < ENC_MOVE_CW/ENC_MOVE_CCW
		uint8_t axis:4;	//! < axis
		uint8_t	dummy:2;
	} bits;
} ENC_MOVE;

//! LrTMAX State definition
enum lr_state_t {
	LR_RESET,			//! < Power on reset
	LR_USB_NOLINK,		//! < No USB connection idling.
	LR_USB_LINKUP,		//! < USB just connected.
	LR_USB_LINKED,		//! < operate USB function
	LR_USB_LINK_LOST,	//! < USB just disconnected.
	LR_USB_DFU,			//! < Launch bootloader & not return.
};

//! switch-encoder matrix lines
enum {
	L0 = 0,
	L1,
	L2,
};

//! Encoder definitions
enum lr_enc_t {
	Lr_ENC0 = 0,
	Lr_ENC1,
	Lr_ENC2,
	Lr_ENC3,
	Lr_ENC4,
	Lr_ENC5,
	Lr_ENC6,
	Lr_ENC7,
	Lr_ENC8,
	Lr_ENC9,
	Lr_ENC10,
	Lr_ENC11,
	Lr_ENC12,
	Lr_ENC13,
	Lr_ENC14,
	Lr_ENC15,
};

//! Scene definition in MIDI
enum lr_scene_t {
	Lr_SCENE0 = 0,
	Lr_SCENE1 = 1,
	Lr_SCENE2 = 2,
	Lr_SCENE3 = 3,
};
//! Key matrix states
enum lr_matrix_t {
	Lr_MATRIX_STOP = 0,
	Lr_MATRIX_START = 1,
};
//! number of scene
#define SCENE_COUNT		4
//! prof. define structure (in key_define.c)
#define DEFINES_PER_SCENE	( ENC_SW_COUNT + (ENC_COUNT * 2) )
//! Assigned notes per scene.
#define NOTES_PER_SCENE	16
//! Define key that designated for scene change.
#define SCENE_BIT		49
//! Scene timeout (1 hour)
#ifdef DEBUG
#define SCENE_TIMEOUT (90*1000*1000 / (TIM_PERIOD_MATRIX + 1))
#else
#define SCENE_TIMEOUT (1200*1000*1000 / (TIM_PERIOD_MATRIX + 1))
#endif
//!CC channel offset for Scene0-ENC0
#define CC_CH_OFFSET	0
#define NOTE_OFFSET		0 //(CC_CH_OFFSET - 8)
//!Assigned CC channel count per scene
#define CC_CH_PER_SCENE	16
#define CH_SCENE_MASK	(CC_CH_PER_SCENE - 1)

//! @defgroup LED timer definitions
#define LED_TIM_NORM	21		//500ms (1 tick=24ms)
#define LED_TIM_SHORT	4		//100ms
#define LED_TIM_HALF	11		//250ms
#define LED_TIM_LONG	42		//1.0Sec
#define LED_TIM_CONNECT	64		//1.5Sec

//! @defgroup Screen timer definitions
#define MSG_TIMER_DEFAULT	47		//1.5Sec (1 tick=32.7ms)
#define MSG_TIMER_NOLINK	31		//1Sec (OLED update in USB not connected)
#define MSG_TIMER_CONNECT	122		//4Sec (USB connected)

#endif /* INC_LRCOMMON_H_ */
