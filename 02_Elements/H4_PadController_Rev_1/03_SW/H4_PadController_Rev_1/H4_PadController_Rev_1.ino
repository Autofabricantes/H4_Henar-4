/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
 * @date   2020_10_27
 */
 
// Library Import
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

// Modes
#define PAD_TESTING

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------System Pin Definition-----------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Pin Definition
const int I2C_SCL			= 3;
const int I2C_SCK			= 3;
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
  int muxChannel;
  bool padStatus        = OFF;
  int lastMeasures[nMEASURES];
  int padType             = 0;
  byte execValue           = 0;
};



// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Global Variables----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------
int pad[4]                       = {PAD_00, PAD_01, PAD_02, PAD_03};
int modeTrigger                  = OFF;
int generalThr                   = 0;
int padNumber                    = 0;

Adafruit_NeoPixel ledRGB = Adafruit_NeoPixel(1, LED, NEO_RGB + NEO_KHZ800);

void setup() {
  // WS2812B LED Initialization
  ledRGB.begin();
  ledRGB.show();
  // Marks the start of operations
  blinkLed();  
  blinkLed();
  blinkLed();
  blinkLed();
  blinkLed();
  // Sets initial Values -> Later these would be get from EEPROM
  generalThr = 150;
  padNumber  = 1;
}

/*
 * LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
 */
void loop() {
  for(int i = 0; i < nPADNOTE; i++){
    read_singlePad(&pad[i]);
  }
}

/*
 * @function    void read_singlePad(padObject *pad)
 * @description Reads the new value for the padObject and executes an action, if necessary
 * @param       padObject *pad      -> PAD to analyse
 * @return      none
 */
void read_singlePad(padObject *pad){
  set_muxGate(pad->muxChannel);
  int newRead = analogRead(MUX_0);
  int avgLevel = calculateMeasureArray(pad);
  switch(pad->padStatus){
    case ON:
      if((avgLevel - newRead) < generalThr){
        pad->padStatus = OFF;
        switch(pad->padType){
          case PADTYPE_NOTE:
            exec_padNote(OFF, 0, (pad->execValue + ((currentOctave + octaveCorrector[currentInstrument]) * 12)), 0);
            break;
          case PADTYPE_OCTAVE:
            // Nothing to do
            break;
          case PADTYPE_CHANNEL:
            // Nothing to do
            break;
          default:
            // Nothing to do
            break;
        }
      }
      break;
    default: // off
      updateMeasureArray(newRead, pad);
      if((calculateMeasureArray(pad) - newRead) > generalThr){
		    pad->padStatus = ON;
        switch(pad->padType){
          case PADTYPE_NOTE:
            exec_padNote(ON, 0, (pad->execValue + ((currentOctave + octaveCorrector[currentInstrument]) * 12)), map(((avgLevel - generalThr) - newRead), 0, (avgLevel - generalThr), MIN_MIDIVELOCITY, MAX_MIDIVELOCITY));
            break;
          case PADTYPE_OCTAVE:
            pad->execValue = exec_padOctave(ON);
            break;
          case PADTYPE_CHANNEL:
            pad->execValue = exec_padInstrument(ON);
            break;
          default:
            // Nothing to do
            break;
        }
        //blinkLed();
      }
      break;
  }
  delay(10);
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
  switch (ONOFF){
    case ON:
      //Serial << " NOTE: " << noteToPlay << "--ON----------------------------------------------------------------------" << endl;
      MIDI_TX(channelToPlay, NOTE_ON, noteToPlay, velocityToPlay);
      break;
    default: // OFF
      //Serial << " NOTE: " << noteToPlay << "--OFF---------------------------------------------------------------------" << endl;
      MIDI_TX(channelToPlay, NOTE_OFF, noteToPlay, 0);
      break;
  }
}

/*
 * @function    int exec_padOctave(byte ONOFF)
 * @description Executes actions of PADTYPE_OCTAVE
 * @param       byte ONOFF          -> ON or OFF enable
 * @return      int currentOctave   -> new Octave
 */
int exec_padOctave(byte ONOFF){
  switch (ONOFF){
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
  }
}

/*
 * @function    int exec_padInstrument(byte ONOFF)
 * @description Executes actions of PADTYPE_CHANNEL
 * @param       byte ONOFF          -> ON or OFF enable
 * @return      int currentInstrument  -> new Channel
 */
int exec_padInstrument(byte ONOFF){
  switch (ONOFF){
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
  }
}

/*
 * @function    void init_allPadOctave()
 * @description Sets the parameters of all padOctave
 * @param       none
 * @return      none
 */
void init_allPadOctave(){
  padOctave.muxChannel = 6;               // TODO Fixed Value
  padOctave.execValue = currentOctave;
  padOctave.padType = PADTYPE_OCTAVE;
}

/*
 * @function    void init_allpadInstrument()
 * @description Sets the parameters of all padInstrument
 * @param       none
 * @return      none
 */
void init_allPadInstrument(){
  padInstrument.muxChannel = 7;           // TODO Fixed Value
  padInstrument.execValue = currentInstrument;
  padInstrument.padType = PADTYPE_CHANNEL;
}

/*
 * @function    void init_allPadNote(int scale)
 * @description Sets the parameters of all padNote
 * @param       int scale     -> scale to be set, based on SCALES[][]
 * @return      none
 */
void init_allPadNote(int scale){
	for(int i = 0; i < nPADNOTE; i++){
		padNote[i].muxChannel = padNote_muxChannel[i];
    padNote[i].padType = PADTYPE_NOTE;
	}
	set_allPadNote_scale(scale);
}

/*
 * @function    void set_allPadNote_scale(int scale)
 * @description Updates all the padNote to a new scale of notes
 * @param       int scale     -> scale to be set, based on SCALES[][]
 * @return      none
 */
void set_allPadNote_scale(int scale){
	MIDIOFF_FIX(currentInstrument, 0, 127);
	for(int i = 0; i < nPADNOTE; i++){
		padNote[i].execValue = SCALES[scale][i];
	}
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
    ledRGB.setPixelColor(0, FULL_G, FULL_R, FULL_B);
    ledRGB.show();
    delay(100);
    instrumentLed(currentInstrument + 1);
}

/*
 * @function    void instrumentLed(int level)
 * @description Sets a designated color in the WS2812B RGB LED controlled by constant values
 * @param       int level       -> LED Level code [0:5]
 * @return      none
 */
void instrumentLed(int level){
	switch(level){
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
	}
}

/*
 * @function    void set_muxGate(int muxChannel)
 * @description Sets a designated Multiplexer Gate
 * @param       int muxChannel      -> MUX Channel to set
 * @return      none
 */
void set_muxGate(int muxChannel){
  for (int h = 0; h < 4; h++) {
      digitalWrite(muxSel[h], ((muxChannel >> (3 - h)) & 0x01)); //MUX Setup pin for each MUX Pin
      //Serial << muxSel[h] << " VALUE: " << ((muxChannel >> (3 - h)) & 0x01) <<endl;
  }
  
  delay(10); // Delay por MUX Setup
}

/*
 * @function    void MIDI_TX(byte MIDICHANNEL, byte MESSAGE, byte PITCH, byte VELOCITY)
 * @description Sends a MIDI Command
 * @param       byte MIDICHANNEL    -> MIDI CHANNEL
 *              byte MESSAGE        -> MIDI Byte 1
 *              byte PITCH          -> MIDI Byte 2
 *              byte VELOCITY       -> MIDI Byte 3
 * @return      none
 */
void MIDI_TX(byte MIDICHANNEL, byte MESSAGE, byte PITCH, byte VELOCITY){
  //byte status1 = MESSAGE + MIDICHANNEL;
  midiEventPacket_t event = {(byte)(MESSAGE >> 4), (byte)(MESSAGE | (MIDICHANNEL)), PITCH, VELOCITY};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

/*
 * @function    void MIDIOFF_FIX(byte CHANNEL, int INIT_NOTE, int END_NOTE)
 * @description Sends a number of NOTE_OFF to silence a channel. Done due to the non acceptance of MIDI Command MIDI_GLOBAL_OFF
 * @param       byte CHANNEL    -> MIDI CHANNEL
 *              int INIT_NOTE   -> Initial Note to MUTE
 *              int END_NOTE    -> End Note to MUTE
 * @return      none
 */
void MIDIOFF_FIX(byte CHANNEL, int INIT_NOTE, int END_NOTE){
  for(int i= INIT_NOTE; i <= END_NOTE; i++){
    MIDI_TX(CHANNEL, NOTE_OFF, i, 0);
    delay(5);
  }
}
