/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .
 * @date   2021_10_31
 */
#include "Arduino.h"
#include "H4_Configuration.h"

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
  set_defaultConfiguration();
  set_defaultI2cDirConfiguration();
}



void H4_Configuration::set_defaultI2cDirConfiguration(){
  CONF.i2cDIR_Stored     = DEFAULT_i2cDIR_Stored;
  //saveConfiguration();
}

void H4_Configuration::set_defaultConfiguration(){
  //CONF.i2cDIR_Stored     = DEFAULT_i2cDIR_Stored;
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
  //saveConfiguration();
}
