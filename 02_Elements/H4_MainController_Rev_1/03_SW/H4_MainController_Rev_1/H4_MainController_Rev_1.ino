/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
 * @date   2020_10_27
 */

// Library Import
#include <Wire.h>
#include "H4_LEDPad.h"
#include "H4_AnalogPad.h"
#include "H4_Configuration.h"
#include <EEPROM.h>
// Modes
#define PAD_TESTING

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------System Pin Definition-----------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Pin Definition
const int pLED              = 2;
const int pPAD_0            = A0;
const int pPAD_1            = A0;
const int pPAD_2            = A0;
const int pPAD_3            = A0;

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Control and Configuration values Definition-------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Constant Value Definition
const int i2cDIR                = 8;

const bool OFF                  = 0;
const bool ON                   = 1;
const int DEFAULT_PADTHR        = 50;

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
const int SET_LEDACTIVITY       = 8;

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

int inMssg[10] = {0,0,0,0,0,0,0,0,0,0};
int color = 0;

void setup() {
  Serial.begin(115200);
  // i2C Initialization
  Wire.begin();
  Wire.onReceive(recieveRequestResponse); // register event

  Wire.beginTransmission(i2cDIR);
  Wire.write(SET_DEFAULT);
  Wire.endTransmission();
  delay(200);
  
}

/*
 * LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
 */
void loop() {
  Wire.beginTransmission(i2cDIR);
  Wire.write(GET_PAD);
  Wire.endTransmission();

  Wire.requestFrom(i2cDIR,9);
  recieveRequestResponse(0);

  if(inMssg[1] == 1){
    Serial.println(" PAD 1 PRESSED");
  }

  if(inMssg[5] == 1){
    Serial.println(" PAD 1 WAS PRESSED");
  }
}

void recieveRequestResponse(int HowMany){
  Serial.println("\nRecieve:");
  int i = 0;
  while(Wire.available()){ // loop through all but the last
    char in = Wire.read();    // receive byte as an integer
    inMssg[i] = in;
    Serial.print(inMssg[i]);
    i++;
  }
}
