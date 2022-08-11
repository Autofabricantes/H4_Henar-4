/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .
 * @date   2021_10_31
 */
#ifndef H4_PADCONTROLLER_H
#define H4_PADCONTROLLER_H

#include "Arduino.h"
//#include "H4_WireController.h"

const int SIGNATURE_ADDR = 0;  // EEPROM Signature Address
// Signature Value
const int SIGNATURE = 90;      // Signature 0x5A 0x01011010 -> Very hard to be random

// Color codes definition
#define  BLACK_OFF           0
#define  WHITE_BRIGHT        1
#define  WHITE_LOW           2
#define  RED_LOW             3
#define  RED_HIGH            4
#define  GREEN_LOW           5
#define  GREEN_HIGH          6
#define  BLUE_LOW            7
#define  BLUE_HIGH           8
#define  ORANGE_LOW          9
#define  ORANGE_HIGH         10
#define  PURPLE_LOW          11
#define  PURPLE_HIGH         12
#define  CYAN_LOW            13
#define  CYAN_HIGH           14
#define  YELLOW_LOW          15
#define  YELLOW_HIGH         16
#define  TURQOUISE_LOW       17
#define  TURQOUISE_HIGH      18
#define  ZIEL_LOW            19
#define  ZIEL_HIGH           20

const int INIT_ADDR = 1;  // EEPROM Init Address
// Default values for all the configuration parameters
const uint8_t DEFAULT_i2cDIR_Stored = 8;  // DEFAULT
const int DEFAULT_offColor = BLACK_OFF;      // BLACK_OFF
const int DEFAULT_primaryColor = TURQOUISE_HIGH;      // TURQOUISE_HIGH
const int DEFAULT_secondaryColor = WHITE_LOW;     // WHITE_LOW
const int DEFAULT_disconnectedColor = 4;  // RED_BRIGHT
const int DEFAULT_connectedColor = 8;     // BLUE_HIGH
const int DEFAULT_blinkDuration = 30;    // MS
const int DEFAULT_fadeDuration = 30;     // MS
const bool DEFAULT_CH_On_0 = 1;           // ON
const bool DEFAULT_CH_On_1 = 0;           // OFF
const bool DEFAULT_CH_On_2 = 0;           // OFF
const bool DEFAULT_CH_On_3 = 0;           // OFF
const int DEFAULT_CH_Thr_0 = 80;          // AVERAGE
const int DEFAULT_CH_Thr_1 = 80;          // AVERAGE
const int DEFAULT_CH_Thr_2 = 80;          // AVERAGE
const int DEFAULT_CH_Thr_3 = 80;          // AVERAGE

const bool CH_OFF           = 0;           // ON
const bool CH_ON            = 1;           // OFF
const byte CH_THR            = 40;          // AVERAGE
const int DISCONNECTEDCOLOR = RED_HIGH;
const int CONNECTEDCOLOR    = BLUE_HIGH;
const int OFFCOLOR          = BLACK_OFF; 
const int BLINKDURATION     = 30;     // MS
const int FADEDURATION      = 30;     // MS

// Pad Control values
const int NO_EVENT = 2;
const int EVENT_TO_ON = 1;
const int EVENT_TO_OFF = 0;

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

// I2C Messages
const int getChMSSG[1] = {GET_PAD}; 
//extern H4_WireController WireController;


class H4_PadController
{
  public:
  
    H4_PadController(int controllerId, int i2cDir);
    void recieveRequestResponse(int HowMany);
    int get_padEvent(int CH_ID);
    void set_padConfiguration(byte primaryColor, byte secondaryColor, byte disconnectedColor, byte connectedColor, byte blinkDuration, byte fadeDuration, bool CH_On_0, bool CH_On_1, bool CH_On_2, bool CH_On_3, byte CH_Thr_0, byte CH_Thr_1, byte CH_Thr_2, byte CH_Thr_3);
    void set_padInstruction(byte channelCode, byte instructionCode_0, byte instructionCode_1, byte pitchCode, byte velocityCode_0, byte velocityCode_1);
    void set_padLedActivity(byte ledActivity_id, byte ledActivity_primaryCode, byte ledActivity_secondaryCode, byte ledActivity_duration);
    byte controllerId;
    bool padPressed[4] = {0,0,0,0};

    // Structure to easily manage all the parameters
    struct padInstruction{
      byte channelCode;
      byte instructionCode_0;
      byte instructionCode_1;
      byte pitchCode;
      byte velocityCode_0;
      byte velocityCode_1;
    };
    
    struct padState{
      bool CH_0_ON;
      bool CH_1_ON;
      bool CH_2_ON;
      bool CH_3_ON;
      bool CH_0_NOW;
      bool CH_1_NOW;
      bool CH_2_NOW;
      bool CH_3_NOW;
      bool CH_0_EVENT;
      bool CH_1_EVENT;
      bool CH_2_EVENT;
      bool CH_3_EVENT;
    };
    
    struct padConfiguration{
      uint8_t i2cDIR;
      byte primaryColor;
      byte secondaryColor;
      byte disconnectedColor;
      byte connectedColor;
      byte blinkDuration;
      byte fadeDuration;
      bool CH_On_0;
      bool CH_On_1;
      bool CH_On_2;
      bool CH_On_3;
      byte CH_Thr_0;
      byte CH_Thr_1;
      byte CH_Thr_2;
      byte CH_Thr_3;
    };

    padInstruction INSTRUCTION;
    padState STATE;
    padConfiguration CONF;
  private:
    byte inMssgLocal[10] = {0,0,0,0,0,0,0,0,0,0};
    void recieveResponse(int HowMany);
    //storedConfiguration _localConfiguration
};

#endif
