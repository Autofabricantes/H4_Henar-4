/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .
 * @date   2021_10_31
 */
#include "Arduino.h"
#include "H4_Configuration.h"
#include <EEPROM.h>
/**
 * @brief H4_Configuration Constructor.
 * @param pinLed  Pin of the LED Stripe
 */
H4_Configuration::H4_Configuration()
{
  set_defaultConfiguration();
}

/**
 * @brief Initializes the LED Stripe and sets it to OFF
 */
void H4_Configuration::init()
{
  //CONF = {DEFAULT_i2cDIR_Stored, DEFAULT_primaryColor, DEFAULT_secondaryColor, DEFAULT_disconnectedColor, DEFAULT_connectedColor, DEFAULT_CH_On_0, DEFAULT_CH_On_1, DEFAULT_CH_On_2, DEFAULT_CH_On_3, DEFAULT_CH_Thr_0, DEFAULT_CH_Thr_1, DEFAULT_CH_Thr_2, DEFAULT_CH_Thr_3};
  if(EEPROM.read(SIGNATURE_ADDR) == SIGNATURE){ // If EEPROM is not corrupted, and things are going nice
    loadConfiguration();
  }else{  // If EEPROM is corrupted or it is it's first use
    set_defaultConfiguration();
    saveConfiguration();
    EEPROM.write(SIGNATURE_ADDR, SIGNATURE); 
    // Here it should probably restart
  }
}

void H4_Configuration::loadConfiguration(){
  CONF.i2cDIR_Stored     = EEPROM.read(INIT_ADDR + 0);
  CONF.primaryColor      = EEPROM.read(INIT_ADDR + 1);
  CONF.secondaryColor    = EEPROM.read(INIT_ADDR + 2);
  CONF.disconnectedColor = EEPROM.read(INIT_ADDR + 3);
  CONF.connectedColor    = EEPROM.read(INIT_ADDR + 4);
  CONF.blinkDuration     = EEPROM.read(INIT_ADDR + 5);
  CONF.fadeDuration      = EEPROM.read(INIT_ADDR + 6);
  CONF.CH_On_0           = EEPROM.read(INIT_ADDR + 7);
  CONF.CH_On_1           = EEPROM.read(INIT_ADDR + 8);
  CONF.CH_On_2           = EEPROM.read(INIT_ADDR + 9);
  CONF.CH_On_3           = EEPROM.read(INIT_ADDR + 10);
  CONF.CH_Thr_0          = EEPROM.read(INIT_ADDR + 11);
  CONF.CH_Thr_1          = EEPROM.read(INIT_ADDR + 12);
  CONF.CH_Thr_2          = EEPROM.read(INIT_ADDR + 13);
  CONF.CH_Thr_3          = EEPROM.read(INIT_ADDR + 14);
}

void H4_Configuration::saveConfiguration(){
  EEPROM.write(INIT_ADDR + 0, CONF.i2cDIR_Stored);
  EEPROM.write(INIT_ADDR + 1, CONF.primaryColor);
  EEPROM.write(INIT_ADDR + 2, CONF.secondaryColor);
  EEPROM.write(INIT_ADDR + 3, CONF.disconnectedColor);
  EEPROM.write(INIT_ADDR + 4, CONF.connectedColor);
  EEPROM.write(INIT_ADDR + 5, CONF.blinkDuration);
  EEPROM.write(INIT_ADDR + 6, CONF.fadeDuration);
  EEPROM.write(INIT_ADDR + 7, CONF.CH_On_0);
  EEPROM.write(INIT_ADDR + 8, CONF.CH_On_1);
  EEPROM.write(INIT_ADDR + 9, CONF.CH_On_2);
  EEPROM.write(INIT_ADDR + 10, CONF.CH_On_3);
  EEPROM.write(INIT_ADDR + 11, CONF.CH_Thr_0);
  EEPROM.write(INIT_ADDR + 12, CONF.CH_Thr_1);
  EEPROM.write(INIT_ADDR + 13, CONF.CH_Thr_2);
  EEPROM.write(INIT_ADDR + 14, CONF.CH_Thr_3);
}

void H4_Configuration::set_defaultConfiguration(){
  CONF.i2cDIR_Stored     = DEFAULT_i2cDIR_Stored;
  CONF.primaryColor      = DEFAULT_primaryColor;
  CONF.secondaryColor    = DEFAULT_secondaryColor;
  CONF.disconnectedColor = DEFAULT_disconnectedColor;
  CONF.connectedColor    = DEFAULT_connectedColor;
  CONF.blinkDuration     = DEFAULT_blinkDuration;
  CONF.fadeDuration      = DEFAULT_fadeDuration;
  CONF.CH_On_0           = DEFAULT_CH_On_0;
  CONF.CH_On_1           = DEFAULT_CH_On_1;
  CONF.CH_On_2           = DEFAULT_CH_On_2;
  CONF.CH_On_3           = DEFAULT_CH_On_3;
  CONF.CH_Thr_0          = DEFAULT_CH_Thr_0;
  CONF.CH_Thr_1          = DEFAULT_CH_Thr_1;
  CONF.CH_Thr_2          = DEFAULT_CH_Thr_2;
  CONF.CH_Thr_3          = DEFAULT_CH_Thr_3;
}

/*
 * const uint8_t DEFAULT_i2cDIR_Stored = 8;  // DEFAULT
const int DEFAULT_primaryColor = 14;      // CYAN_HIGH
const int DEFAULT_secondaryColor = 2;     // WHITE_LOW
const int DEFAULT_disconnectedColor = 4;  // RED_BRIGHT
const int DEFAULT_connectedColor = 8;     // BLUE_HIGH
const bool DEFAULT_CH_On_0 = 1;           // ON
const bool DEFAULT_CH_On_1 = 0;           // OFF
const bool DEFAULT_CH_On_2 = 0;           // OFF
const bool DEFAULT_CH_On_3 = 0;           // OFF
const int DEFAULT_CH_Thr_0 = 20;          // AVERAGE
const int DEFAULT_CH_Thr_1 = 20;          // AVERAGE
const int DEFAULT_CH_Thr_2 = 20;          // AVERAGE
const int DEFAULT_CH_Thr_3 = 20;          // AVERAGE
 */
