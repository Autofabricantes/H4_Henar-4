 /*
   @author Pablo dMM (Pablodmm.isp@gmail.com)
   @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
   @date   2020_10_27
*/

// Library Import
#include "H4_LEDPad.h"
#include "H4_Configuration.h"
// Modes
#define PAD_TESTING
#define FIXED_I2C_DIR
// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------System Pin Definition-----------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Pin Definition Arduino Megsa
/*  const int I2C_SCL			      = 3;
  const int I2C_SCK			      = 3;
  const int pLED              = 2;
  const int pPAD_0            = A0;
  const int pPAD_1            = A0;
  const int pPAD_2            = A0;
  const int pPAD_3            = A0;*/

#define SDA    21 // GPIO21
#define SCL    22 // GPIO22
// Pin Definition ESP32

const int I2C_SCL           = 22;
const int I2C_SDA           = 21;
const int pLED              = 33;
const int pPAD_0            = 18;
const int pPAD_1            = 5;
const int pPAD_2            = 4;
const int pPAD_3            = 2;



// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Control and Configuration values Definition-------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Constant Value Definition
const int i2cDIR                = 8;

const byte FixedI2cDIR          = 120;

const bool OFF                  = 0;
const bool ON                   = 1;
const int STABILIZATION_ITERATOR = 200;

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Custom Configuration Structs----------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------

// I2C Values
const int SET_I2CDIR            = 1;
const int SET_COLOR             = 2;
const int GET_COLOR             = 3;
const int SET_CH                = 4;
const int GET_CH                = 5;
const int GET_PAD               = 6;
const int SET_DEFAULT           = 7;
const int SET_DEFAULT_NOI2CDIR  = 8;
const int SET_LEDACTIVITY       = 9;
const int SET_RESET             = 10;

// LED Activity IDs
const int LED_ID_ALLSTRIPE      = 0;
const int LED_ID_SETOFF         = 1;
const int LED_ID_BLINKCOLOR     = 2;
const int LED_ID_FADEFROMNUCLEO = 3;
const int LED_ID_FADEANDOFFCOLOR_FROMNUCLEO = 4;

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Global Variables----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------


H4_LEDPad LED(pLED);
H4_Configuration CONFIGURATION;
// Led Activity Flags:
boolean performLedActivity    = false;
int ledActivity_id            = 0;
int ledActivity_primaryCode   = 0;
int ledActivity_secondaryCode = 0;
int ledActivity_duration      = 0;

void(* resetFunc) (void) = 0;

void setup() {
  Serial.begin(115200);
  LED.init();
  delay(500);


}

/*
   LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
*/
void loop() {
  Serial.println("AYY");
  ledActivity(LED_ID_FADEFROMNUCLEO, WHITE_LOW, WHITE_LOW, 50);
  delay(500);
  delay(200);
}

void ledActivity(int id, int primaryCode, int secondaryCode, int duration) {
  performLedActivity = false; // Clear activation flag
  switch (id) {
    case LED_ID_ALLSTRIPE:
      LED.setAllStripe(primaryCode);
      break;
    case LED_ID_SETOFF:
      LED.setOff();
      break;
    case LED_ID_BLINKCOLOR:
      LED.blinkColor(primaryCode, duration);
      break;
    case LED_ID_FADEFROMNUCLEO:
      LED.fadeColor_fromNucleo(primaryCode, secondaryCode, duration);
      break;
    case LED_ID_FADEANDOFFCOLOR_FROMNUCLEO:
      LED.fadeandOffColor_fromNucleo(primaryCode, secondaryCode, duration);
      break;
    default:
      break;
  }
}
