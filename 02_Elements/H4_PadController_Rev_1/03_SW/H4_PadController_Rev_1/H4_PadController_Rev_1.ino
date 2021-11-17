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
const int I2C_SCL			      = 3;
const int I2C_SCK			      = 3;
const int pLED              = 2;
const int pPAD_0            = A0;
const int pPAD_1            = A0;
const int pPAD_2            = A0;
const int pPAD_3            = A0;

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Control and Configuration values Definition-------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Constant Value Definition
const int LEDn                  = 6;
const int i2cDIR                = 8;

const bool OFF                  = 0;
const bool ON                   = 1;
const int DEFAULT_PADTHR        = 30;

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Custom Configuration Structs----------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Global Variables----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------


H4_LEDPad LED(pLED);
H4_AnalogPad PAD_0(pPAD_0, true, DEFAULT_PADTHR);
H4_Configuration  CONFIGURATION;
int activePad0 = 0;

void setup() {
  LED.init();
  PAD_0.init();
  CONFIGURATION.init();
  Serial.begin(115200);
  // i2C Initialization
  Wire.begin(i2cDIR);
  Wire.onReceive(receiveEvent); // register event
  // Marks the start of operations
}

/*
 * LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
 */
void loop() {
  PAD_0.readPad();
  if((!activePad0) && PAD_0.get_isPadActivated()){
    Serial.println("I GO UP!");
    LED.fadeColor_fromNucleo(BLACK_OFF, TURQOUISE_HIGH, 500);
    activePad0 = 1;
  }
  
  if((activePad0) && !PAD_0.get_isPadActivated()){
    Serial.println("I GO DOWN!");
    LED.fadeColor_fromNucleo(TURQOUISE_HIGH, BLACK_OFF, 500);
    activePad0 = 0;
  }
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  int x = Wire.read();    // receive byte as an integer
  //blinkLed();
}


/*
 * @function    void updateMeasureArray(int newValue, padObject *pad)
 * @description Updates an specific padObject with a new analogRead value
 * @param       padObject *pad    -> PAD to be updated
 *              int newValue      -> newValue to be stored
 * @return      none
 */
/*void updateMeasureArray(int newValue, padObject *pad){
  for(int i = 1; i < nMEASURES; i++){
    pad->lastMeasures[i] = pad->lastMeasures[i-1];
  }
  pad->lastMeasures[0] = newValue;
}*/

/*
 * @function    int calculateMeasureArray(padObject *pad)
 * @description Calculates the average measure of an specific padObject
 * @param       padObject *pad            -> PAD to be measured
 * @return      (averageValue/nMEASURES)  -> Average value
 */
/*int calculateMeasureArray(padObject *pad){
  int averageValue = 0;
  for(int i = 0; i < nMEASURES; i++){
    averageValue = (averageValue + pad->lastMeasures[i]);
  }
  return (averageValue/nMEASURES);
}*/
