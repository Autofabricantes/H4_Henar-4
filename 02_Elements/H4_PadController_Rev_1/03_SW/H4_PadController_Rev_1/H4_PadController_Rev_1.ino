/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
 * @date   2020_10_27
 */

// Library Import
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

// Modes
#define PAD_TESTING

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------System Pin Definition-----------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Pin Definition
const int I2C_SCL			      = 3;
const int I2C_SCK			      = 3;
const int LED               = 2;
const int PAD_00            = A0;
const int PAD_01            = A0;
const int PAD_02            = A0;
const int PAD_03            = A0;

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Control and Configuration values Definition-------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Constant Value Definition
const int LEDn                  = 6;
const int i2cDIR                = 8;

const bool OFF                  = 0;
const bool ON                   = 1;
const int NOTEDURATION          = 200;
const int nMEASURES             = 5;
const int nPADNOTE              = 8;
const byte MIN_MIDIVELOCITY     = 60;
const byte MAX_MIDIVELOCITY     = 127;

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Custom Configuration Structs----------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
struct padObject {
  int muxChannel        = 0;
  bool padStatus        = OFF;
  int lastMeasures[nMEASURES];
  byte execValue          = 0;
};

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Global Variables----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------
int pad[4]                       = {PAD_00, PAD_01, PAD_02, PAD_03};
int currentRGB[nLED][3]          = {{OFF, OFF, OFF},
                                   {OFF, OFF, OFF},
                                   {OFF, OFF, OFF},
                                   {OFF, OFF, OFF},
                                   {OFF, OFF, OFF},
                                   {OFF, OFF, OFF}};
int modeTrigger                  = OFF;
int generalThr                   = 0;
int padNumber                    = 0;

Adafruit_NeoPixel ledRGB = Adafruit_NeoPixel(6, LED, NEO_RGB + NEO_KHZ800);

void setup() {
  // WS2812B LED Initialization
  ledRGB.begin();
  ledRGB.show();
  blinkLed();
  // i2C Initialization
  Wire.begin(i2cDIR);
  Wire.onReceive(receiveEvent); // register event
  // Marks the start of operations
  blinkLed();
  // Sets initial Values -> Later these would be get from EEPROM
  generalThr = 150;
  padNumber  = 1;
}

/*
 * LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
 */
void loop() {
  //for(int i = 0; i < nPADNOTE; i++){
  //  read_singlePad(&padObject[i]);
  //}
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  int x = Wire.read();    // receive byte as an integer
  blinkLed();
}

/*
 * @function    void read_singlePad(padObject *pad)
 * @description Reads the new value for the padObject and executes an action, if necessary
 * @param       padObject *pad      -> PAD to analyse
 * @return      none
 */
void read_singlePad(padObject *pad){
  /*set_muxGate(pad->muxChannel);
  int newRead = analogRead(MUX_0);
  int avgLevel = calculateMeasureArray(pad);
  switch(pad->padStatus){
    case ON:
      if((avgLevel - newRead) < generalThr){
        pad->padStatus = OFF;
      }
      break;
    default: // off
      updateMeasureArray(newRead, pad);
      if((calculateMeasureArray(pad) - newRead) > generalThr){
		    pad->padStatus = ON;
      }
      break;
  }*/
  //delay(10);
}

/*
 * @function    void exec_padNote(byte ONOFF, byte channelToPlay, byte noteToPlay, byte velocityToPlay)
 * @description Executes actions of PADTYPE_NOTE
 * @param       byte ONOFF          -> ON or OFF enable
 *              byte channelToPlay  -> MIDI Channel
 *              byte noteToPlay     -> MIDI Note
 *              byte velocityToPlay -> MIDI Velocity
 * @return      none
 */
void exec_padNote(byte ONOFF, byte channelToPlay, byte noteToPlay, byte velocityToPlay){
  /*switch (ONOFF){
    case ON:
      //Serial << " NOTE: " << noteToPlay << "--ON----------------------------------------------------------------------" << endl;
      MIDI_TX(channelToPlay, NOTE_ON, noteToPlay, velocityToPlay);
      break;
    default: // OFF
      //Serial << " NOTE: " << noteToPlay << "--OFF---------------------------------------------------------------------" << endl;
      MIDI_TX(channelToPlay, NOTE_OFF, noteToPlay, 0);
      break;
  }*/
}

/*
 * @function    int exec_padOctave(byte ONOFF)
 * @description Executes actions of PADTYPE_OCTAVE
 * @param       byte ONOFF          -> ON or OFF enable
 * @return      int currentOctave   -> new Octave
 */
int exec_padOctave(byte ONOFF){
  /*switch (ONOFF){
    case ON:
      MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C8);
      if(currentOctave == 5){
        currentOctave = 4;
      }else{
        currentOctave = 5;
      }
      //Serial << " OCTAVE: " << currentOctave << "--ON----------------------------------------------------------------------" << endl;
      blinkLed();
      return currentOctave;
      break;
    default: // OFF
      // Nothing to do
      break;
  }*/
}

/*
 * @function    int exec_padInstrument(byte ONOFF)
 * @description Executes actions of PADTYPE_CHANNEL
 * @param       byte ONOFF          -> ON or OFF enable
 * @return      int currentInstrument  -> new Channel
 */
int exec_padInstrument(byte ONOFF){
  /*switch (ONOFF){
    case ON:
      MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C8);
      if(currentInstrument < 5){
        currentInstrument++;
      }else{
        currentInstrument = 0;
      }
      //Serial << " CHANNEL: " << currentInstrument << "--ON----------------------------------------------------------------------" << endl;
      MIDI_TX(0, INSTRUMENT, currentInstrument, 0);
      blinkLed();
      return currentInstrument;
      break;
    default: // OFF
      // Nothing to do
      break;
  }*/
}


/*
 * @function    void updateMeasureArray(int newValue, padObject *pad)
 * @description Updates an specific padObject with a new analogRead value
 * @param       padObject *pad    -> PAD to be updated
 *              int newValue      -> newValue to be stored
 * @return      none
 */
void updateMeasureArray(int newValue, padObject *pad){
  for(int i = 1; i < nMEASURES; i++){
    pad->lastMeasures[i] = pad->lastMeasures[i-1];
  }
  pad->lastMeasures[0] = newValue;
}

/*
 * @function    int calculateMeasureArray(padObject *pad)
 * @description Calculates the average measure of an specific padObject
 * @param       padObject *pad            -> PAD to be measured
 * @return      (averageValue/nMEASURES)  -> Average value
 */
int calculateMeasureArray(padObject *pad){
  int averageValue = 0;
  for(int i = 0; i < nMEASURES; i++){
    averageValue = (averageValue + pad->lastMeasures[i]);
  }
  return (averageValue/nMEASURES);
}

/*
 * @function    void blinkLed()
 * @description Blinks the LED in white for 100ms and returns to the previos color
 * @param       none
 * @return      none
 */
void blinkLed(){
    setallLed_noStep(255,255,255);
    delay(500);
    setallLed_noStep(0,0,0);
    delay(500);
}

/*
 * @function    void setallLed_noStep(int R, int G, int B)
 * @description Sets all the LEDs in stripe to selected RGB
 * @param       int R       -> LED R [0:255]
 * @param       int G       -> LED G [0:255]
 * @param       int B       -> LED B [0:255]
 * @return      none
 */
void setallLed_noStep(int R, int G, int B){
    if(R > 255){R = 255;}
    if(G > 255){G = 255;}
    if(B > 255){B = 255;}
    for(int i=0; i<LEDn; i++){
      ledRGB.setPixelColor(i, G, R, B);
      currentRGB[i][0]=R;
      currentRGB[i][2]=G;
      currentRGB[i][3]=B;
      ledRGB.show();
    }
}

/*
 * @function    void setallLed_Step()
 * @description Sets all the LEDs in stripe to selected RGB as asequence
 * @param       int R       -> LED R [0:255]
 * @param       int G       -> LED G [0:255]
 * @param       int B       -> LED B [0:255]
 * @return      none
 */
void setallLed_Step(int R, int G, int B){
    if(R > 255){R = 255;}
    if(G > 255){G = 255;}
    if(B > 255){B = 255;}
    for(int i=0; i<LEDn; i++){
      ledRGB.setPixelColor(i, G, R, B);
      currentRGB[i][0]=R;
      currentRGB[i][2]=G;
      currentRGB[i][3]=B;
      ledRGB.show();
    }
}

/*
 * @function    void instrumentLed(int level)
 * @description Sets a designated color in the WS2812B RGB LED controlled by constant values
 * @param       int level       -> LED Level code [0:5]
 * @return      none
 */
void instrumentLed(int level){
	/*switch(level){
		case 0:	//	All OFF
			ledRGB.setPixelColor(0, 0, 0, 0);
			ledRGB.show();
			break;
		case 1:
			ledRGB.setPixelColor(0, GEN_G_L0, GEN_R_L0, GEN_B_L0);
			ledRGB.show();
			break;
		case 2:
			ledRGB.setPixelColor(0, GEN_G_L1, GEN_R_L1, GEN_B_L1);
			ledRGB.show();
			break;
		case 3:
			ledRGB.setPixelColor(0, GEN_G_L2, GEN_R_L2, GEN_B_L2);
			ledRGB.show();
			break;
    case 4:
      ledRGB.setPixelColor(0, GEN_G_L3, GEN_R_L3, GEN_B_L3);
      ledRGB.show();
      break;
    case 5:
      ledRGB.setPixelColor(0, GEN_G_L4, GEN_R_L4, GEN_B_L4);
      ledRGB.show();
      break;
		default:
			ledRGB.setPixelColor(0, GEN_G_L5, GEN_R_L5, GEN_B_L5);
			ledRGB.show();
			break;
	}*/
}
