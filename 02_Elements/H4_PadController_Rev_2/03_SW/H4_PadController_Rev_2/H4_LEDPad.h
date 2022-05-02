/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
 * @date   2021_10_31
 */
#ifndef H4_LEDPAD_H
#define H4_LEDPAD_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Board Requirements
#define  QLED				         6

// Color codes definition
#define  BLACK_OFF           0
#define  WHITE_BRIGHT        1
#define  WHITE_LOW           2
#define  RED_LOW             3
#define  RED_HIGH            4
#define  GREEN_LOW           5
#define  GREEN_HIGH          6
#define  BLUE_LOW            7
#define  BLUE_HIGH           8
#define  ORANGE_LOW          9
#define  ORANGE_HIGH         10
#define  PURPLE_LOW          11
#define  PURPLE_HIGH         12
#define  CYAN_LOW            13
#define  CYAN_HIGH           14
#define  YELLOW_LOW          15
#define  YELLOW_HIGH         16
#define  TURQOUISE_LOW       17
#define  TURQOUISE_HIGH      18
#define  ZIEL_LOW            19
#define  ZIEL_HIGH           20

// Other usefull constants
#define  NSTEPS             20
#define  MAX_COLORCODE      20
#define  MIN_COLORCODE      0
#define  MAX_DURATION       255
#define  MIN_DURATION       2
#define  RADIAL_OFFSET_1    4
#define  RADIAL_OFFSET_2    8


class H4_LEDPad
{
  public:
    H4_LEDPad(int pinEn);
    void init();
    void setColor(int colorCode);
    void setAllStripe(int colorCode);
    void setOff();
    void blinkColor(int colorCode, int blinkDuration);
    void fadeColor_fromNucleo(int initColorCode, int endColorCode, int fadeDuration);
    void fadeandOffColor_fromNucleo(int initColorCode, int endColorCode, int fadeDuration);
  private:
    Adafruit_NeoPixel _LED;
    int _pinLed;
    // Gamma corrector table for WS2812B.
    // Obtained from: https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
    uint8_t gamma8[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
    int COL[21][3] =    {{ 0,    0,    0 },      // BLACK_OFF
                        { 255,  255,  255 },    // WHITE_BRIGHT
                        { 100,   100,   100 },     // WHITE_LOW
                        { 100,  0,    0 },      // RED_LOW
                        { 255,  0,    0 },      // RED_HIGH
                        { 0,    100,  0 },      // GREEN_LOW
                        { 0,    255,  0 },      // GREEN_HIGH
                        { 0,    0,    100 },    // BLUE_LOW
                        { 0,    0,    255 },    // BLUE_HIGH
                        { 255,  128,  0 },      // ORANGE_LOW
                        { 100,  50,   0 },      // ORANGE_HIGH
                        { 50,   0,    50 },     // PURPLE_LOW
                        { 255,  0,    255 },    // PURPLE_HIGH
                        { 0,    50,   50 },     // CYAN_LOW
                        { 0,    255,  255 },    // CYAN_HIGH
                        { 255,  255,  51 },     // YELLOW_LOW
                        { 50,   50,   0 },      // YELLOW_HIGH
                        { 0,    100,  50 },     // TURQOUISE_LOW
                        { 51,   255,  153 },    // TURQOUISE_HIGH
                        { 0,    50,   100 },    // ZIEL_LOW
                        { 51,   153,  255 }};   // ZIEL_HIGH
};

#endif
