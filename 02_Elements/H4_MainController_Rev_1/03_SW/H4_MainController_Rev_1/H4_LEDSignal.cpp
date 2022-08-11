/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .
 * @date   2022_08_08
 */
#include "Arduino.h"
#include "H4_LEDSignal.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

/**
 * @brief H4_LEDSignal Constructor.
 * @param pinLed  Pin of the LED Stripe
 */
H4_LEDSignal::H4_LEDSignal(int pinLed)
{
  // Pin Configuration
  _pinLed = pinLed;
}

/**
 * @brief Initializes the LED Stripe and sets it to OFF
 */
void H4_LEDSignal::init()
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
void H4_LEDSignal::setOff(){
  _LED.clear();
  _LED.show();
}

/**
 * @brief Sets the whole Stripe to the selected color
 * @param colorCode Color to be set
 */
void H4_LEDSignal::setAllStripe(int colorCode){
  // Value Safety Control
  if((colorCode < MIN_COLORCODE) ||(colorCode > MAX_COLORCODE)){
    colorCode = BLACK_OFF;
  }
  // Processing
  for(int i = 0; i< QLED; i++){
    _LED.setPixelColor(i, gamma8[COL[colorCode][0]], gamma8[COL[colorCode][1]], gamma8[COL[colorCode][2]]);
  }
  _LED.show();
}

/**
 * @brief Makes a blink in the whole Stripe for a selected color por a set duration and then goes for a permanent color
 * @param colorCode Color to be set
 * @param colorBlinkCode Color to blink
 * @param blinkDuration Duration of the blink in ms
 */
void H4_LEDSignal::blinkColorAndStay(int colorBlinkCode, int colorCode, int blinkDuration){
  // Value Safety Control
  if((colorCode < MIN_COLORCODE) ||(colorCode > MAX_COLORCODE)){
    colorCode = BLACK_OFF;
  }
  if((blinkDuration > MAX_DURATION) || (blinkDuration < MIN_DURATION)){
    blinkDuration = MIN_DURATION;
  }
  // Processing
  setAllStripe(colorBlinkCode);
  delay(blinkDuration * 10);
  setAllStripe(colorCode);
  setOff();
}
