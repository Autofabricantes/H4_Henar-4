/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
 * @date   2021_11_01
 */
#ifndef H4_ANALOGPAD_H
#define H4_ANALOGPAD_H

#include "Arduino.h"

// Board Requirements
#define  nMEASURES           10

// Other usefull constants
#define  NSTEPS             20



class H4_AnalogPad
{
  public:
    H4_AnalogPad(int pPAD, bool PAD_Enable, int PAD_Thr);
    void init();
    void readPad();
    void set_padEnable(int newPadEnable);
    void set_padThr(int newThr);
    int get_padThr();
    bool get_isPadActivated();
    bool get_wasPadActivated();
    void clear_wasPadActivated();
    void fillMeasureArray();
    int calculateAverage();
  private:
    int _measures[nMEASURES] = {0,0,0,0,0,0,0,0,0,0};                                
    int _pPAD;
    bool _PAD_Enable;
    int _PAD_Thr;
    int _wasActive;
    int _isActive;
};

#endif
