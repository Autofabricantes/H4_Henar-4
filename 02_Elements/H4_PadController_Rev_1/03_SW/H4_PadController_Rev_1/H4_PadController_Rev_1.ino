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
const int i2cDIR                = 8;

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
H4_AnalogPad PAD_0(pPAD_0, false, 0);
H4_AnalogPad PAD_1(pPAD_1, false, 0);
H4_AnalogPad PAD_2(pPAD_2, false, 0);
H4_AnalogPad PAD_3(pPAD_3, false, 0);
H4_Configuration  CONFIGURATION;
char requestBuffer[10] = {0,0,0,0,0,0,0,0,0,0};
int requestBufferLength = 10;

// PAD Flags
char PAD_ACTIVE_0 = 0;
char PAD_ACTIVE_1 = 0;
char PAD_ACTIVE_2 = 0;
char PAD_ACTIVE_3 = 0;
// 

int inMssg[10] = {0,0,0,0,0,0,0,0,0,0};
boolean recievedWireCmd = false;

// Led Activity Flags:
boolean performLedActivity    = false;
int ledActivity_id            = 0;
int ledActivity_primaryCode   = 0;
int ledActivity_secondaryCode = 0;
int ledActivity_duration      = 0;



void setup() {
  //CONFIGURATION.set_defaultConfiguration();
  //CONFIGURATION.saveConfiguration();
  CONFIGURATION.init();
  CONFIGURATION.loadConfiguration();
  
  PAD_0 = H4_AnalogPad(pPAD_0, CONFIGURATION.CONF.CH_On_0, CONFIGURATION.CONF.CH_Thr_0);
  PAD_1 = H4_AnalogPad(pPAD_1, CONFIGURATION.CONF.CH_On_1, CONFIGURATION.CONF.CH_Thr_1);
  PAD_2 = H4_AnalogPad(pPAD_2, CONFIGURATION.CONF.CH_On_2, CONFIGURATION.CONF.CH_Thr_2);
  PAD_3 = H4_AnalogPad(pPAD_3, CONFIGURATION.CONF.CH_On_3, CONFIGURATION.CONF.CH_Thr_3);
  
  LED.init();
  delay(1000);
  PAD_0.init();
  PAD_1.init();
  PAD_2.init();
  PAD_3.init();
  Serial.begin(115200);
  // i2C Initialization
  Wire.begin(CONFIGURATION.CONF.i2cDIR_Stored);
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  // Marks the start of operations
  //Pad Stabilization
  
  for(int i = 0; i > STABILIZATION_ITERATOR; i++){
    PAD_0.readPad();
    PAD_1.readPad();
    PAD_2.readPad();
    PAD_3.readPad();
  }
}

/*
 * LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
 */
void loop() {
  if(performLedActivity){
    ledActivity(ledActivity_id, ledActivity_primaryCode, ledActivity_secondaryCode, ledActivity_duration);
  }
  
  if(recievedWireCmd){
    CONFIGURATION.saveConfiguration();
    recievedWireCmd = false;
  }
  // READ PAD 0
  //LED.fadeColor_fromNucleo(CONFIGURATION.CONF.secondaryColor, CONFIGURATION.CONF.primaryColor, CONFIGURATION.CONF.fadeDuration);
  if(CONFIGURATION.CONF.CH_On_0){
    PAD_0.readPad();
    if((!PAD_ACTIVE_0) && PAD_0.get_isPadActivated()){
      Serial.println("0-> I GO UP!");
      PAD_ACTIVE_0 = 1;
      LED.fadeColor_fromNucleo(CONFIGURATION.CONF.secondaryColor, CONFIGURATION.CONF.primaryColor, CONFIGURATION.CONF.fadeDuration);
      
    }
    
    if((PAD_ACTIVE_0) && !PAD_0.get_isPadActivated()){
      Serial.println("0-> I GO DOWN!");
      PAD_ACTIVE_0 = 0;
      LED.fadeColor_fromNucleo(CONFIGURATION.CONF.primaryColor, CONFIGURATION.CONF.secondaryColor, CONFIGURATION.CONF.fadeDuration);
      
    }
  }
  // READ PAD 1
  if(CONFIGURATION.CONF.CH_On_1){
    PAD_1.readPad();
    if((!PAD_ACTIVE_1) && PAD_1.get_isPadActivated()){
      Serial.println("1-> I GO UP!");
      PAD_ACTIVE_1 = 1;
      LED.fadeColor_fromNucleo(CONFIGURATION.CONF.secondaryColor, CONFIGURATION.CONF.primaryColor, CONFIGURATION.CONF.fadeDuration);
      
    }
    
    if((PAD_ACTIVE_1) && !PAD_1.get_isPadActivated()){
      Serial.println("1-> I GO DOWN!");
      PAD_ACTIVE_1 = 0;
      LED.fadeColor_fromNucleo(CONFIGURATION.CONF.primaryColor, CONFIGURATION.CONF.secondaryColor, CONFIGURATION.CONF.fadeDuration);
      
    }
  }
  // READ PAD 2
  if(CONFIGURATION.CONF.CH_On_2){
    PAD_2.readPad();
    if((!PAD_ACTIVE_2) && PAD_2.get_isPadActivated()){
      Serial.println("2-> I GO UP!");
      PAD_ACTIVE_2 = 1;
      LED.fadeColor_fromNucleo(CONFIGURATION.CONF.secondaryColor, CONFIGURATION.CONF.primaryColor, CONFIGURATION.CONF.fadeDuration * 10);
      
    }
    
    if((PAD_ACTIVE_2) && !PAD_2.get_isPadActivated()){
      Serial.println("2-> I GO DOWN!");
      PAD_ACTIVE_2 = 0;
      LED.fadeColor_fromNucleo(CONFIGURATION.CONF.primaryColor, CONFIGURATION.CONF.secondaryColor, CONFIGURATION.CONF.fadeDuration * 10);
    }
  }
  // READ PAD 3
  if(CONFIGURATION.CONF.CH_On_3){
    PAD_3.readPad();
    if((!PAD_ACTIVE_3) && PAD_3.get_isPadActivated()){
      Serial.println("3-> I GO UP!");
      PAD_ACTIVE_3 = 1;
      LED.fadeColor_fromNucleo(CONFIGURATION.CONF.secondaryColor, CONFIGURATION.CONF.primaryColor, CONFIGURATION.CONF.fadeDuration * 10);
      
    }
    
    if((PAD_ACTIVE_3) && !PAD_3.get_isPadActivated()){
      Serial.println("3-> I GO DOWN!");
      PAD_ACTIVE_3 = 0;
      LED.fadeColor_fromNucleo(CONFIGURATION.CONF.primaryColor, CONFIGURATION.CONF.secondaryColor, CONFIGURATION.CONF.fadeDuration * 10);
    }
  }
  delay(5);
}

void ledActivity(int id, int primaryCode, int secondaryCode, int duration){
  performLedActivity = false; // Clear activation flag
  switch(id){
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

/*
 * @function    void receiveEvent(int howMany)
 * @description Processes the event of a Recieve I2C Message
 * @param       int howMany    -> Not Sure what this is
 * @return      none
 */
void receiveEvent(int howMany)
{
  Serial.println("\nRecieve:");
  int i = 0;
  while(Wire.available()){ // loop through all but the last
    int in = Wire.read();    // receive byte as an integer
    inMssg[i] = in;
    Serial.print(inMssg[i]);
    Serial.print(" ");
    i++;
  }
  //recievedEvent = true;
  processMsg(inMssg);
  //Wire.write(requestBuffer, requestBufferLength);
}

/*
 * @function    void requestEvent()
 * @description Processes the event of a Reqeust I2C Message
 * @param       none
 * @return      none
 */
void requestEvent()
{
    Serial.println("Request");
    Wire.write(requestBuffer, requestBufferLength);
}

/*
 * @function    int processMsg(int inMssg[])
 * @description Processes the incomming I2C Message and prepares the Buffer por a following i2C Request
 * @param       int inMssg[]    -> Incomming message
 * @return      none
 */
void processMsg(int inMssg[]){
  recievedWireCmd = true;
  switch(inMssg[0]){
    // Set the I2CDIR
    case SET_I2CDIR:
      Serial.println("\nSET_I2CDIR");
      CONFIGURATION.set_defaultConfiguration();
      CONFIGURATION.CONF.i2cDIR_Stored = inMssg[1];
      //CONFIGURATION.saveConfiguration();
      requestBuffer[0] = SET_I2CDIR;
      requestBuffer[1] = CONFIGURATION.CONF.i2cDIR_Stored;
      requestBufferLength = 2;
      break;
    // Color LED Control
    case SET_COLOR:
      Serial.println("\nSET_COLOR");
      CONFIGURATION.CONF.primaryColor       = inMssg[1];
      CONFIGURATION.CONF.secondaryColor     = inMssg[2];
      CONFIGURATION.CONF.disconnectedColor  = inMssg[3];
      CONFIGURATION.CONF.connectedColor     = inMssg[4];
      CONFIGURATION.CONF.blinkDuration      = inMssg[5];
      CONFIGURATION.CONF.fadeDuration       = inMssg[6];
      //CONFIGURATION.saveConfiguration();
      // Prepare Request Buffer 
      requestBuffer[0] = SET_COLOR;
      requestBuffer[1] = CONFIGURATION.CONF.primaryColor;
      requestBuffer[2] = CONFIGURATION.CONF.secondaryColor;
      requestBuffer[3] = CONFIGURATION.CONF.disconnectedColor;
      requestBuffer[4] = CONFIGURATION.CONF.connectedColor;
      requestBuffer[5] = CONFIGURATION.CONF.blinkDuration;
      requestBuffer[6] = CONFIGURATION.CONF.fadeDuration;
      requestBufferLength = 7;
      break;
    case GET_COLOR:
      Serial.println("\nGET_COLOR");
      requestBuffer[0] = GET_COLOR;
      requestBuffer[1] = CONFIGURATION.CONF.primaryColor;
      requestBuffer[2] = CONFIGURATION.CONF.secondaryColor;
      requestBuffer[3] = CONFIGURATION.CONF.disconnectedColor;
      requestBuffer[4] = CONFIGURATION.CONF.connectedColor;
      requestBuffer[5] = CONFIGURATION.CONF.blinkDuration;
      requestBuffer[6] = CONFIGURATION.CONF.fadeDuration;
      requestBufferLength = 7;
      break;
    // Channel ON/OFF Control
    case SET_CH:
      Serial.println("\nSET_CH");
      CONFIGURATION.CONF.CH_On_0 = inMssg[1];
      CONFIGURATION.CONF.CH_On_1 = inMssg[2];
      CONFIGURATION.CONF.CH_On_2 = inMssg[3];
      CONFIGURATION.CONF.CH_On_3 = inMssg[4];
      CONFIGURATION.CONF.CH_Thr_0 = inMssg[5];
      CONFIGURATION.CONF.CH_Thr_1 = inMssg[6];
      CONFIGURATION.CONF.CH_Thr_2 = inMssg[7];
      CONFIGURATION.CONF.CH_Thr_3 = inMssg[8];
      //CONFIGURATION.saveConfiguration();
      // Update Values
      PAD_0 = H4_AnalogPad(pPAD_0, CONFIGURATION.CONF.CH_On_0, CONFIGURATION.CONF.CH_Thr_0);
      PAD_1 = H4_AnalogPad(pPAD_1, CONFIGURATION.CONF.CH_On_1, CONFIGURATION.CONF.CH_Thr_1);
      PAD_2 = H4_AnalogPad(pPAD_2, CONFIGURATION.CONF.CH_On_2, CONFIGURATION.CONF.CH_Thr_2);
      PAD_3 = H4_AnalogPad(pPAD_3, CONFIGURATION.CONF.CH_On_3, CONFIGURATION.CONF.CH_Thr_3);
      // Prepare Responses
      requestBuffer[0] = SET_CH;
      requestBuffer[1] = CONFIGURATION.CONF.CH_On_0;
      requestBuffer[2] = CONFIGURATION.CONF.CH_On_1;
      requestBuffer[3] = CONFIGURATION.CONF.CH_On_2;
      requestBuffer[4] = CONFIGURATION.CONF.CH_On_3;
      requestBuffer[5] = CONFIGURATION.CONF.CH_Thr_0;
      requestBuffer[6] = CONFIGURATION.CONF.CH_Thr_1;
      requestBuffer[7] = CONFIGURATION.CONF.CH_Thr_2;
      requestBuffer[8] = CONFIGURATION.CONF.CH_Thr_3;
      requestBufferLength = 9;
      break;
    case GET_CH:
      Serial.println("\nGET_CH");
      requestBuffer[0] = GET_CH;
      requestBuffer[1] = CONFIGURATION.CONF.CH_On_0;
      requestBuffer[2] = CONFIGURATION.CONF.CH_On_1;
      requestBuffer[3] = CONFIGURATION.CONF.CH_On_2;
      requestBuffer[4] = CONFIGURATION.CONF.CH_On_3;
      requestBuffer[5] = CONFIGURATION.CONF.CH_Thr_0;
      requestBuffer[6] = CONFIGURATION.CONF.CH_Thr_1;
      requestBuffer[7] = CONFIGURATION.CONF.CH_Thr_2;
      requestBuffer[8] = CONFIGURATION.CONF.CH_Thr_3;
      requestBufferLength = 9;
      break;
    // Read Pad Result
    case GET_PAD:
      Serial.println("\nGET_PAD");
      requestBuffer[0] = GET_PAD;
      requestBuffer[1] = PAD_ACTIVE_0;
      requestBuffer[2] = PAD_ACTIVE_1;
      requestBuffer[3] = PAD_ACTIVE_2;
      requestBuffer[4] = PAD_ACTIVE_3;
      requestBuffer[5] = PAD_0.get_wasPadActivated();
      requestBuffer[6] = PAD_1.get_wasPadActivated();
      requestBuffer[7] = PAD_2.get_wasPadActivated();
      requestBuffer[8] = PAD_3.get_wasPadActivated();
      // Resets the values of the PAD_WAS_ACTIVE
      PAD_0.clear_wasPadActivated();
      PAD_1.clear_wasPadActivated();
      PAD_2.clear_wasPadActivated();
      PAD_3.clear_wasPadActivated();
      requestBufferLength = 9;
      break;
    case SET_DEFAULT:
      Serial.println("\nSET_DEFAULT");
      CONFIGURATION.set_defaultConfiguration();
      PAD_0 = H4_AnalogPad(pPAD_0, CONFIGURATION.CONF.CH_On_0, CONFIGURATION.CONF.CH_Thr_0);
      PAD_1 = H4_AnalogPad(pPAD_1, CONFIGURATION.CONF.CH_On_1, CONFIGURATION.CONF.CH_Thr_1);
      PAD_2 = H4_AnalogPad(pPAD_2, CONFIGURATION.CONF.CH_On_2, CONFIGURATION.CONF.CH_Thr_2);
      PAD_3 = H4_AnalogPad(pPAD_3, CONFIGURATION.CONF.CH_On_3, CONFIGURATION.CONF.CH_Thr_3);
      //CONFIGURATION.saveConfiguration();
      break;
    case SET_LEDACTIVITY:
      Serial.println("\nSET_LEDACTIVITY");
      performLedActivity = true;
      ledActivity_id = inMssg[1];
      ledActivity_primaryCode = inMssg[2];
      ledActivity_secondaryCode = inMssg[3];
      ledActivity_duration = inMssg[4];
      break;
    default:
      break;
  }
}
