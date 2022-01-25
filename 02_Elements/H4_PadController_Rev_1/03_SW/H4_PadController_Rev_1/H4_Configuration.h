/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .
 * @date   2021_10_31
 */
#ifndef H4_ConfIGURATION_H
#define H4_ConfIGURATION_H

#include "Arduino.h"
#include <EEPROM.h>

const int SIGNATURE_ADDR = 0;  // EEPROM Signature Address
// Signature Value
const int SIGNATURE = 90;      // Signature 0x5A 0x01011010 -> Very hard to be random


const int INIT_ADDR = 1;  // EEPROM Init Address
// Default values for all the configuration parameters
const uint8_t DEFAULT_i2cDIR_Stored = 8;  // DEFAULT
const int DEFAULT_primaryColor = 14;      // CYAN_HIGH
const int DEFAULT_secondaryColor = 2;     // WHITE_LOW
const int DEFAULT_disconnectedColor = 4;  // RED_BRIGHT
const int DEFAULT_connectedColor = 8;     // BLUE_HIGH
const int DEFAULT_blinkDuration = 30;     // MS
const int DEFAULT_fadeDuration = 30;      // MS
const bool DEFAULT_CH_On_0 = 1;           // ON
const bool DEFAULT_CH_On_1 = 0;           // OFF
const bool DEFAULT_CH_On_2 = 0;           // OFF
const bool DEFAULT_CH_On_3 = 0;           // OFF
const int DEFAULT_CH_Thr_0 = 100;          // AVERAGE
const int DEFAULT_CH_Thr_1 = 100;          // AVERAGE
const int DEFAULT_CH_Thr_2 = 100;          // AVERAGE
const int DEFAULT_CH_Thr_3 = 100;          // AVERAGE


class H4_Configuration
{
  public:
    H4_Configuration();
    void init();
    void loadConfiguration();
    void saveConfiguration();
    void set_defaultConfiguration();
    void set_defaultI2cDirConfiguration();
    // Structure to easily manage all the parameters
    struct storedConfiguration{
        uint8_t i2cDIR_Stored;
        int primaryColor;
        int secondaryColor;
        int disconnectedColor;
        int connectedColor;
        int blinkDuration;
        int fadeDuration;
        bool CH_On_0;
        bool CH_On_1;
        bool CH_On_2;
        bool CH_On_3;
        int CH_Thr_0;
        int CH_Thr_1;
        int CH_Thr_2;
        int CH_Thr_3;
    };

    storedConfiguration CONF;

    /*uint8_t get_i2cDIR_Stored();
    int get_primaryColor();
    int get_secondaryColor();
    int get_disconnectedColor();
    int get_connectedColor();
    bool get_CH_On_0();
    bool get_CH_On_1();
    bool get_CH_On_2();
    bool get_CH_On_3();
    int get_CH_Thr_0();
    int get_CH_Thr_1();
    int get_CH_Thr_2();
    int get_CH_Thr_3();

    void set_i2cDIR_Stored(uint8_t i2cDIR_Stored);
    void set_primaryColor(int primaryColor);
    void set_secondaryColor(int secondaryColor);
    void set_disconnectedColor(int disconnectedColor);
    void set_connectedColor(int connectedColor);
    void set_CH_On_0(bool CH_On_0);
    void set_CH_On_1(bool CH_On_1);
    void set_CH_On_2(bool CH_On_2);
    void set_CH_On_3(bool CH_On_3);
    void set_CH_Thr_0(int CH_Thr_0);
    void set_CH_Thr_1(int CH_Thr_1);
    void set_CH_Thr_2(int CH_Thr_2);
    void set_CH_Thr_3(int CH_Thr_3);*/

   

    
  private:
    //storedConfiguration _localConfiguration
};

#endif
