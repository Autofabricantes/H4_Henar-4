/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .
 * @date   2021_10_31
 */
#include "Arduino.h"
#include "H4_LEDPad.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/**
 * @brief H4_LEDPad Constructor.
 * @param pinLed  Pin of the LED Stripe
 */
H4_LEDPad::H4_LEDPad(int pinLed)
{
  // Pin Configuration
  _pinLed = pinLed;
}

/**
 * @brief Initializes the LED Stripe and sets it to OFF
 */
void H4_LEDPad::init()
{
  // Pin Configuration
  _LED = Adafruit_NeoPixel(QLED, _pinLed, NEO_GRB      + NEO_KHZ800);
  _LED.begin();                    // INITIALIZE NeoPixel strip object
  _LED.clear();
  _LED.show();
}


/**
 * @brief Sets the whole Stripe Off
 */
void H4_LEDPad::setOff(){
  _LED.clear();
  _LED.show();
}

/**
 * @brief Sets the whole Stripe to the selected color
 * @param colorCode Color to be set
 */
void H4_LEDPad::setAllStripe(int colorCode){
  // Value Safety Control
  if((colorCode < MIN_COLORCODE) ||(colorCode > MAX_COLORCODE)){
    colorCode = BLACK_OFF;
  }
  // Processing
  for(int i = 0; i< QLED; i++){
    _LED.setPixelColor(i, gamma8[COL[colorCode][0]], gamma8[COL[colorCode][1]], gamma8[COL[colorCode][2]]);
    /*Serial.print("ColorCode: ");
    Serial.println(colorCode);
    Serial.print("R: ");
    Serial.println(COL[colorCode][0]);
    Serial.print("G: ");
    Serial.println(COL[colorCode][1]);
    Serial.print("B: ");
    Serial.println(COL[colorCode][2]);*/
  }
  _LED.show();
}

/**
 * @brief Makes a blink in the whole Stripe for a selected color por a set duration
 * @param colorCode Color to be set
 * @param blinkDuration Duration of the blink in ms
 */
void H4_LEDPad::blinkColor(int colorCode, int blinkDuration){
  // Value Safety Control
  if((colorCode < MIN_COLORCODE) ||(colorCode > MAX_COLORCODE)){
    colorCode = BLACK_OFF;
  }
  if((blinkDuration > MAX_DURATION) || (blinkDuration < MIN_DURATION)){
    blinkDuration = MIN_DURATION;
  }
  // Processing
  setAllStripe(colorCode);
  delay(blinkDuration * 10);
  setOff();
}

/**
 * @brief Makes a Radial fade from the center from initColorCode to endColorCode in fadeDuration time
 * @param initColorCode Initial Color in which fade starts
 * @param endColorCode  End Color in which fade ends
 * @param fadeDuration  Duration of the fade in ds
 */
void H4_LEDPad::fadeColor_fromNucleo(int initColorCode, int endColorCode, int fadeDuration){
  // Value Safety Control
  if((initColorCode < MIN_COLORCODE) ||(initColorCode > MAX_COLORCODE)){
    initColorCode = BLACK_OFF;
  }
  if((endColorCode < MIN_COLORCODE) ||(endColorCode > MAX_COLORCODE)){
    endColorCode = BLACK_OFF;
  }
  if((fadeDuration > MAX_DURATION) || (fadeDuration < MIN_DURATION)){
    fadeDuration = MIN_DURATION;
  }
  // Processing
  int fadeInterval = fadeDuration * 10 / NSTEPS; // in ms
  int fadeR_R0 = (COL[endColorCode][0] - COL[initColorCode][0]) / NSTEPS; // in ms
  int fadeG_R0 = (COL[endColorCode][1] - COL[initColorCode][1]) / NSTEPS; // in ms
  int fadeB_R0 = (COL[endColorCode][2] - COL[initColorCode][2]) / NSTEPS; // in ms

  int fadeR_R1 = (COL[endColorCode][0] - COL[initColorCode][0]) / (NSTEPS + RADIAL_OFFSET_1); // in ms
  int fadeG_R1 = (COL[endColorCode][1] - COL[initColorCode][1]) / (NSTEPS + RADIAL_OFFSET_1); // in ms
  int fadeB_R1 = (COL[endColorCode][2] - COL[initColorCode][2]) / (NSTEPS + RADIAL_OFFSET_1); // in ms

  int fadeR_R2 = (COL[endColorCode][0] - COL[initColorCode][0]) / (NSTEPS + RADIAL_OFFSET_2); // in ms
  int fadeG_R2 = (COL[endColorCode][1] - COL[initColorCode][1]) / (NSTEPS + RADIAL_OFFSET_2); // in ms
  int fadeB_R2 = (COL[endColorCode][2] - COL[initColorCode][2]) / (NSTEPS + RADIAL_OFFSET_2); // in ms
  setAllStripe(initColorCode);
  for(int i = 0; i < NSTEPS; i++){
    // radial 2
     _LED.setPixelColor(0, gamma8[COL[initColorCode][0] + (i+1)*fadeR_R2], gamma8[COL[initColorCode][1] + (i+1)*fadeG_R2], gamma8[COL[initColorCode][2] + (i+1)*fadeB_R2]);
     _LED.setPixelColor(5, gamma8[COL[initColorCode][0] + (i+1)*fadeR_R2], gamma8[COL[initColorCode][1] + (i+1)*fadeG_R2], gamma8[COL[initColorCode][2] + (i+1)*fadeB_R2]);
     // radial 0
     _LED.setPixelColor(2, gamma8[COL[initColorCode][0] + (i+1)*fadeR_R0], gamma8[COL[initColorCode][1] + (i+1)*fadeG_R0], gamma8[COL[initColorCode][2] + (i+1)*fadeB_R0]);
     _LED.setPixelColor(3, gamma8[COL[initColorCode][0] + (i+1)*fadeR_R0], gamma8[COL[initColorCode][1] + (i+1)*fadeG_R0], gamma8[COL[initColorCode][2] + (i+1)*fadeB_R0]);
     // radial 1
     _LED.setPixelColor(1, gamma8[COL[initColorCode][0] + (i+1)*fadeR_R1], gamma8[COL[initColorCode][1] + (i+1)*fadeG_R1], gamma8[COL[initColorCode][2] + (i+1)*fadeB_R1]);
     _LED.setPixelColor(4, gamma8[COL[initColorCode][0] + (i+1)*fadeR_R1], gamma8[COL[initColorCode][1] + (i+1)*fadeG_R1], gamma8[COL[initColorCode][2] + (i+1)*fadeB_R1]);
     _LED.show();
     delay(fadeInterval);

  }
}

/**
 * @brief Makes a Radial fade from the center from initColorCode to endColorCode and then back to initColorCode in fadeDuration time
 * @param initColorCode Initial Color in which fade starts
 * @param endColorCode  End Color in which fade ends
 * @param fadeDuration  Duration of the fade
 */
void H4_LEDPad::fadeandOffColor_fromNucleo(int initColorCode, int endColorCode, int fadeDuration){
  // Value Safety Control
  if((initColorCode < MIN_COLORCODE) ||(initColorCode > MAX_COLORCODE)){
    initColorCode = BLACK_OFF;
  }
  if((endColorCode < MIN_COLORCODE) ||(endColorCode > MAX_COLORCODE)){
    endColorCode = BLACK_OFF;
  }
  if((fadeDuration > MAX_DURATION) || (fadeDuration < MIN_DURATION)){
    fadeDuration = MIN_DURATION;
  }
  // Processing
  fadeColor_fromNucleo(initColorCode, endColorCode, fadeDuration/2);
  fadeColor_fromNucleo(endColorCode, initColorCode, fadeDuration/2);
  setOff();
}
