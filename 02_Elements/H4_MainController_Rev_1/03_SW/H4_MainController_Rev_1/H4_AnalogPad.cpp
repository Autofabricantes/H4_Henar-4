/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
 * @date   2021_11_01
 */
#include "Arduino.h"
#include "H4_AnalogPad.h"

/**
 * @brief H4_AnalogPad Constructor.
 * @param pPAD     Pin of the Analog Pad
 * @param PAD_Enable   Enable of the Analog Pad
 * @param PAD_Thr  Activation Threshold of the Analog Pad
 */
H4_AnalogPad::H4_AnalogPad(int pPAD, bool PAD_Enable, int PAD_Thr)
{
  _pPAD = pPAD;
  _PAD_Enable = PAD_Enable;
  _PAD_Thr = PAD_Thr;
}

/**
 * @brief Initializes the Pad Controller
 */
void H4_AnalogPad::init()
{
  pinMode(_pPAD, INPUT);
  fillMeasureArray();
}

/*
 *  @brief  Calculates the average of the last nMEASURES
 */
int H4_AnalogPad::calculateAverage(){
  int newTotal = 0;
  for(int i = 0; i < nMEASURES; i++){
    newTotal += _measures[i];
  }
  //Serial.print("New PAD Average = ");
  //Serial.println(newTotal/nMEASURES);
  return newTotal/nMEASURES; // newAverage
}

/**
 * @brief Sets a new value for _PAD_Enable
 * @param newPadEnable  new value for _PAD_Enable
 */
void H4_AnalogPad::set_padEnable(int newPadEnable){
  _PAD_Enable = newPadEnable;
}

/**
 * @brief Sets a new value for _PAD_Thr
 * @param newPadThre  new value for _PAD_Thr
 */
void H4_AnalogPad::set_padThr(int newPadThr){
  _PAD_Thr = newPadThr;
}

/**
 * @brief Gets current value for _PAD_Thr
 */
int H4_AnalogPad::get_padThr(){
  return _PAD_Thr;
}

/**
 * @brief Gets current value for _isActive
 */
bool H4_AnalogPad::get_isPadActivated(){
  return _isActive;
}

/**
 * @brief Gets current value for _wasActive
 */
bool H4_AnalogPad::get_wasPadActivated(){
  return _wasActive;
}

/**
 * @brief Resets the value for _wasActive
 */
void H4_AnalogPad::clear_wasPadActivated(){
  _wasActive = false;
}

/**
 * @brief makes a new read for the Pad and sets a new value for _WasActive if active
 */
void H4_AnalogPad::readPad(){
  int newMeasure = analogRead(_pPAD);
  int newAverage = calculateAverage();

  //Serial.print("newMeasure: ");
  //Serial.println(newMeasure);

  // as a frequency filter, detects if the deviation is higher than _PAD_Thr
  if((newMeasure < newAverage)&&((newAverage-newMeasure)>_PAD_Thr)){
    ////Serial.println("PAD ACTIVE");
    _wasActive = true;
    _isActive = true;
    fillMeasureArray();
  }
  else if((newMeasure > newAverage)&&((newMeasure-newAverage)>_PAD_Thr)){
    ////Serial.println("PAD DEACTIVE");
    _isActive = false;
    fillMeasureArray();
  }
  // Adds last measure to the measure array

  for(int i = 1; i < nMEASURES; i++){
    _measures[nMEASURES - i] = _measures[nMEASURES - i - 1];
  }
  _measures[0] = newMeasure;
  
  // Prints all data
  ////Serial.println("Full Measure Array");
  //for(int j = 0; j < nMEASURES; j++){
  //  //Serial.print(_measures[j]);
  //  //Serial.print("\t");
  //}
  ////Serial.println(" ");
}

/**
 * @brief fills the measure array with new measures
 */
void H4_AnalogPad::fillMeasureArray(){
  for(int i = 1; i < nMEASURES; i++){
    _measures[i] = analogRead(_pPAD);
  }
}
