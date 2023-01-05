
/* Lume1-FF-6AF 6A Buck + FET with UDR Driver config options
 * - Anduril 2 Firmware by Selene Scriven (R624)
 * - 25 Oct 2022
 * - Attiny1616
 * - For use with FireFlyLite E12R, E07X, PL09mu, etc
 * 
 * For more information: 
 * - www.loneoceans.com/labs/
 * - Manual: http://toykeeper.net/torches/fsm/anduril2/anduril-manual.txt
 */

#define MODEL_NUMBER "0481"  // lume1-ff-6af with attiny1616
#include "hwdef-lume1-1616-fet-6buck.h"

//***************************************
//**       THERMAL SETTINGS            **
//***************************************

// set this light for 50C thermal ceiling
#undef DEFAULT_THERM_CEIL
#define DEFAULT_THERM_CEIL 55
//#define MAX_THERM_CEIL 70 // default maximum value

// drop from turbo at therm_ceil+TURBO_TEMP_EXTRA
#define TURBO_TEMP_EXTRA 7 // in celsius

// stop panicking at ~3A (not sure of this numbers yet since it depends on the host..)
#define THERM_FASTER_LEVEL 125			   	 // About 8.9W
#define MIN_THERM_STEPDOWN 88			          // About 2.2W
#define THERM_NEXT_WARNING_THRESHOLD 16		 // accumulate less error before adjusting (default 24)
#define THERM_RESPONSE_MAGNITUDE 64  	   	 // bigger adjustments (default 64)

// easier access to thermal config mode, similar to Emisar, Noctigon
#define USE_TENCLICK_THERMAL_CONFIG

// optional, makes initial turbo step-down faster so first peak isn't as hot
// FET mode can run very very hot, so be extra careful
//#define THERM_HARD_TURBO_DROP

//***************************************
//**      RAMPS AND OPERATIONS         **
//***************************************

// Uncomment to get Max Turbo on 2C from Off
//#define USE_2C_MAX_TURBO

// enable 2 click turbo
#define DEFAULT_2C_STYLE 1

// set this light to use stepped ramp by default (instead of smooth)
#undef RAMP_STYLE
#define RAMP_STYLE 1

// the entire ramp is regulated; don't blink halfway up
#ifdef BLINK_AT_RAMP_MIDDLE
#undef BLINK_AT_RAMP_MIDDLE
#endif

// We're abusing the Dynamic PWM functionality to set the VREF instead of PWM TOP (from GChart's work)
// We don't want the Gradual functionality to mess with the PWM_TOP value.
#ifdef USE_SET_LEVEL_GRADUALLY
#undef USE_SET_LEVEL_GRADUALLY
#endif

// Enable usage of multiple power paths (right now 3 paths)
#define USE_MULTIPLE_POWER_PATH

// RAMP TABLE FOR BEHAVIOR USING LEVELS 1-149 as BUCK, 150 as FET

#define RAMP_LENGTH 150
#define USE_DYN_PWM

// NOTE --> RAMP TABLES MUST CORRESPOND TO THESE VALUES!
#define LED_PATH1_PIN_LEVEL_MIN 1
#define LED_PATH2_PIN_LEVEL_MIN 15
#define LED_PATH3_PIN_LEVEL_MIN 48
#define LED_FET_PIN_LEVEL_MIN 150

#define PWM_CHANNELS 2
#define PWM_BITS 8 
#define PMW2_TOP 255

// =========== UDR x^4 curves ===========

// for 255 DAC UDR - with FET
#define PWM1_LEVELS 1, 1, 2, 2, 3, 7, 14, 25, 40, 61, 89, 126, 174, 234, 2, 3, 4, 5, 6, 7, 9, 11, 13, 15, 18, 22, 25, 29, 34, 39, 44, 50, 57, 64, 72, 80, 90, 100, 111, 123, 136, 149, 164, 180, 197, 215, 235, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 13, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 31, 32, 33, 35, 37, 38, 40, 42, 43, 45, 47, 49, 51, 53, 56, 58, 60, 62, 65, 67, 70, 73, 75, 78, 81, 84, 87, 90, 93, 96, 100, 103, 107, 110, 114, 118, 122, 126, 130, 134, 138, 143, 147, 152, 157, 161, 166, 171, 176, 182, 187, 193, 198, 204, 210, 216, 222, 228, 235, 241, 248, 255, 0
// for 255 PWM FET
#define PWM2_LEVELS 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255

// for 255 DAC UDR - 149 Levels, no fet
//#define PWM1_LEVELS 1, 1, 2, 2, 3, 7, 14, 25, 40, 61, 89, 126, 174, 234, 2, 3, 4, 5, 6, 7, 9, 11, 13, 15, 18, 22, 25, 29, 34, 39, 44, 50, 57, 64, 72, 80, 90, 100, 111, 123, 136, 149, 164, 180, 197, 215, 235, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 13, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 31, 32, 33, 35, 37, 38, 40, 42, 43, 45, 47, 49, 51, 53, 56, 58, 60, 62, 65, 67, 70, 73, 75, 78, 81, 84, 87, 90, 93, 96, 100, 103, 107, 110, 114, 118, 122, 126, 130, 134, 138, 143, 147, 152, 157, 161, 166, 171, 176, 182, 187, 193, 198, 204, 210, 216, 222, 228, 235, 241, 248, 255
// for 255 PWM FET
//#define PWM2_LEVELS 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

#define PWM_TOPS 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18

#ifdef USE_DYNAMIC_UNDERCLOCKING
#undef USE_DYNAMIC_UNDERCLOCKING
#endif

//#define MAX_1x7135 34
//#define HALFSPEED_LEVEL 14
//#define QUARTERSPEED_LEVEL 6

#define DEFAULT_LEVEL 61  // About ~0.5W
#define MAX_1x7135 88     // About ~2.2W

#define RAMP_SMOOTH_FLOOR 8
#define RAMP_SMOOTH_CEIL 149  // Level 150 is when Buck is off and FET is ON 100% (seems to be 5.00A)

#define RAMP_DISCRETE_FLOOR 8
#define RAMP_DISCRETE_CEIL RAMP_SMOOTH_CEIL
#define RAMP_DISCRETE_STEPS 5

#define SIMPLE_UI_FLOOR 13
#define SIMPLE_UI_CEIL 142  // 142 for 5A max
#define SIMPLE_UI_STEPS 5

// Turn on Buck from level 1 to 149, but not 150
// Level 150 is when Buck is off and FET is ON 100%
#define LED_ENABLE_PIN_LEVEL_MIN 1
#define LED_ENABLE_PIN_LEVEL_MAX 149

//***************************************
//**       AUX LEDs and MISC           **
//***************************************

// this light has three aux LED channels: R, G, B
#define USE_AUX_RGB_LEDS
#define AUXLED_RGB_DIFFERENT_PORTS 

// it has no independent LED in the button
//#define USE_BUTTON_LED

// off mode: low (1)
// lockout: blinking (3)
//#define INDICATOR_LED_DEFAULT_MODE ((3<<2) + 1)

// the button lights up
//#define USE_INDICATOR_LED

// the button is visible while main LEDs are on
//#define USE_INDICATOR_LED_WHILE_RAMPING

// the aux LEDs are front-facing, so turn them off while main LEDs are on
// TODO: the whole "indicator LED" thing needs to be refactored into
//       "aux LED(s)" and "button LED(s)" since they work a bit differently
#ifdef USE_INDICATOR_LED_WHILE_RAMPING
#undef USE_INDICATOR_LED_WHILE_RAMPING
#endif

#define RGB_LED_OFF_DEFAULT 0x18  // low, voltage
#define RGB_LED_LOCKOUT_DEFAULT 0x37  // blinking, rainbow

// enable blinking aux LEDs
#define TICK_DURING_STANDBY
#define STANDBY_TICK_SPEED 3  // every 0.128 s
//#define STANDBY_TICK_SPEED 4  // every 0.256 s
//#define STANDBY_TICK_SPEED 5  // every 0.512 s

#define USE_SOS_MODE
#define USE_SOS_MODE_IN_BLINKY_GROUP

#define USE_BEACONTOWER_MODE

// party strobe on-time
#define PARTY_STROBE_ONTIME 4

// attiny1616 has enough space to smooth out voltage readings
#define USE_VOLTAGE_LOWPASS

// can't reset the normal way because power is connected before the button
#define USE_SOFT_FACTORY_RESET

// ======================================