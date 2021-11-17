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
  //_localConfiguration = {DEFAULT_i2cDIR_Stored, DEFAULT_primaryColor, DEFAULT_secondaryColor, DEFAULT_disconnectedColor, DEFAULT_connectedColor, DEFAULT_CH_On_0, DEFAULT_CH_On_1, DEFAULT_CH_On_2, DEFAULT_CH_On_3, DEFAULT_CH_Thr_0, DEFAULT_CH_Thr_1, DEFAULT_CH_Thr_2, DEFAULT_CH_Thr_3};
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
  _localConfiguration.i2cDIR_Stored     = EEPROM.read(INIT_ADDR + 0);
  _localConfiguration.primaryColor      = EEPROM.read(INIT_ADDR + 1);
  _localConfiguration.secondaryColor    = EEPROM.read(INIT_ADDR + 2);
  _localConfiguration.disconnectedColor = EEPROM.read(INIT_ADDR + 3);
  _localConfiguration.connectedColor    = EEPROM.read(INIT_ADDR + 4);
  _localConfiguration.CH_On_0           = EEPROM.read(INIT_ADDR + 5);
  _localConfiguration.CH_On_1           = EEPROM.read(INIT_ADDR + 6);
  _localConfiguration.CH_On_2           = EEPROM.read(INIT_ADDR + 7);
  _localConfiguration.CH_On_3           = EEPROM.read(INIT_ADDR + 8);
  _localConfiguration.CH_Thr_0          = EEPROM.read(INIT_ADDR + 9);
  _localConfiguration.CH_Thr_1          = EEPROM.read(INIT_ADDR + 10);
  _localConfiguration.CH_Thr_2          = EEPROM.read(INIT_ADDR + 11);
  _localConfiguration.CH_Thr_3          = EEPROM.read(INIT_ADDR + 12);
}

void H4_Configuration::saveConfiguration(){
  EEPROM.write(INIT_ADDR + 0, _localConfiguration.i2cDIR_Stored);
  EEPROM.write(INIT_ADDR + 1, _localConfiguration.primaryColor);
  EEPROM.write(INIT_ADDR + 2, _localConfiguration.secondaryColor);
  EEPROM.write(INIT_ADDR + 3, _localConfiguration.disconnectedColor);
  EEPROM.write(INIT_ADDR + 4, _localConfiguration.connectedColor);
  EEPROM.write(INIT_ADDR + 5, _localConfiguration.CH_On_0);
  EEPROM.write(INIT_ADDR + 6, _localConfiguration.CH_On_1);
  EEPROM.write(INIT_ADDR + 7, _localConfiguration.CH_On_2);
  EEPROM.write(INIT_ADDR + 8, _localConfiguration.CH_On_3);
  EEPROM.write(INIT_ADDR + 9, _localConfiguration.CH_Thr_0);
  EEPROM.write(INIT_ADDR + 10, _localConfiguration.CH_Thr_1);
  EEPROM.write(INIT_ADDR + 11, _localConfiguration.CH_Thr_2);
  EEPROM.write(INIT_ADDR + 12, _localConfiguration.CH_Thr_3);
}

void H4_Configuration::set_defaultConfiguration(){
  _localConfiguration.i2cDIR_Stored     = DEFAULT_i2cDIR_Stored;
  _localConfiguration.primaryColor      = DEFAULT_primaryColor;
  _localConfiguration.secondaryColor    = DEFAULT_secondaryColor;
  _localConfiguration.disconnectedColor = DEFAULT_disconnectedColor;
  _localConfiguration.connectedColor    = DEFAULT_connectedColor;
  _localConfiguration.CH_On_0           = DEFAULT_CH_On_0;
  _localConfiguration.CH_On_1           = DEFAULT_CH_On_1;
  _localConfiguration.CH_On_2           = DEFAULT_CH_On_2;
  _localConfiguration.CH_On_3           = DEFAULT_CH_On_3;
  _localConfiguration.CH_Thr_0          = DEFAULT_CH_Thr_0;
  _localConfiguration.CH_Thr_1          = DEFAULT_CH_Thr_1;
  _localConfiguration.CH_Thr_2          = DEFAULT_CH_Thr_2;
  _localConfiguration.CH_Thr_3          = DEFAULT_CH_Thr_3;
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
