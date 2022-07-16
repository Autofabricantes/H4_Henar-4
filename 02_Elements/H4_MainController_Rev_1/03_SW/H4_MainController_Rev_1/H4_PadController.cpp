/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .
 * @date   2022_02_13
 */
 // Library Import
#include "Arduino.h"
#include <Wire.h>
#include "H4_PadController.h"
//#include "H4_WireController.h"
//#include "Wire.h"
/**
 * @brief H4_PadController Constructor.
 */
H4_PadController::H4_PadController(int controllerId, int i2cDir)
{
  CONF.i2cDIR = i2cDir;
  controllerId = controllerId;
  //Wire.begin();
  //set_defaultConfiguration();
}

void H4_PadController::set_padInstruction(byte channelCode, byte instructionCode_0, byte instructionCode_1, byte pitchCode, byte velocityCode_0, byte velocityCode_1){
  INSTRUCTION.channelCode = channelCode;
  INSTRUCTION.instructionCode_0 = instructionCode_0;
  INSTRUCTION.instructionCode_1 = instructionCode_1;
  INSTRUCTION.pitchCode = pitchCode;
  INSTRUCTION.velocityCode_0 = velocityCode_0;
  INSTRUCTION.velocityCode_1 = velocityCode_1;
}

void H4_PadController::set_padConfiguration(int primaryColor, int secondaryColor, int disconnectedColor, int connectedColor, int blinkDuration, int fadeDuration, bool CH_On_0, bool CH_On_1, bool CH_On_2, bool CH_On_3, int CH_Thr_0, int CH_Thr_1, int CH_Thr_2, int CH_Thr_3){
  // Update local parameters
  CONF.primaryColor = primaryColor;
  CONF.secondaryColor = secondaryColor;
  CONF.disconnectedColor = disconnectedColor;
  CONF.connectedColor = connectedColor;
  CONF.blinkDuration = blinkDuration;
  CONF.fadeDuration = fadeDuration;
  CONF.CH_On_0 = CH_On_0;
  CONF.CH_On_1 = CH_On_1;
  CONF.CH_On_2 = CH_On_2;
  CONF.CH_On_3 = CH_On_3;
  CONF.CH_Thr_0 = CH_Thr_0;
  CONF.CH_Thr_1 = CH_Thr_1;
  CONF.CH_Thr_2 = CH_Thr_2;
  CONF.CH_Thr_3 = CH_Thr_3;
  // Send Configuration messages
  int setColorMSSG[7] = {SET_COLOR, primaryColor, secondaryColor, disconnectedColor, connectedColor, blinkDuration, fadeDuration};
  Wire.beginTransmission(CONF.i2cDIR); 
  for(int i = 0; i < 7; i++){
    Wire.write(setColorMSSG[i]);
  }
  Wire.endTransmission();
  int setChMSSG[9] = {SET_CH, CH_On_0, CH_On_1, CH_On_2, CH_On_3, CH_Thr_0, CH_Thr_1, CH_Thr_2, CH_Thr_3};
  
  Wire.beginTransmission(CONF.i2cDIR); 
  for(int i = 0; i < 9; i++){
    Wire.write(setChMSSG[i]);
  }
  Wire.endTransmission();
};

void H4_PadController::set_padLedActivity(int ledActivity_id, int ledActivity_primaryCode, int ledActivity_secondaryCode, int ledActivity_duration){
  // Send Configuration messages
  int setLedActivityMSSG[5] = {SET_LEDACTIVITY, ledActivity_id, ledActivity_primaryCode, ledActivity_secondaryCode, ledActivity_duration};
  Wire.beginTransmission(CONF.i2cDIR); 
  for(int i = 0; i < 5; i++){
    Wire.write(setLedActivityMSSG[i]);
  }
  Wire.endTransmission();
};

int H4_PadController::get_padEvent(int CH_ID){
  
  Wire.beginTransmission(CONF.i2cDIR); 
  Wire.write(getChMSSG[0]);
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(CONF.i2cDIR,9 );
  recieveResponse(0);

  int padNow = 1 + CH_ID;
  int padEvent = 5 + CH_ID;
  
  if((!padPressed)&&(inMssgLocal[padNow] == 1)){
    padPressed = true;
    return EVENT_TO_ON;
  }else if((padPressed)&&(inMssgLocal[padNow] == 0)){
    padPressed = false;
    return EVENT_TO_OFF;
  }else{
    return NO_EVENT;
  }
}


void H4_PadController::recieveResponse(int HowMany){
  int i = 0;
  //Serial.print("\n I2C RECIEVE for I2C ");
  //Serial.print(CONF.i2cDIR);
  //Serial.print(" : ");
  while(Wire.available()){ // loop through all but the last
    char in = Wire.read();    // receive byte as an integer
    inMssgLocal[i] = in;
    //Serial.print(inMssgLocal[i]);
    i++;
  }
}
