/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
 * @date   2020_02_08
 */
 
// Library Import
#include <Streaming.h>
#include <MIDIUSB.h>
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
const int MUX_0				= A0;
const int LED         = 2;
const int MUX_A				= 3;
const int MUX_B				= 4;
const int MUX_C				= 5;
const int MUX_D				= 6;

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Control and Configuration values Definition-------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Constant Value Definition
const int LEDn                  = 4;
//const int LEDBoometer[LEDn]     = {LED_G, LED_Y, LED_R};

const int Buzzn                 = 5;
const int BuzzBoometer[Buzzn]   = {33, 65, 131, 262, 523};

const bool OFF                  = 0;
const bool ON                   = 1;
const int NOTEDURATION          = 200;
const int nMEASURES             = 5;
const int nPADNOTE              = 8;
const byte MIN_MIDIVELOCITY     = 60;
const byte MAX_MIDIVELOCITY     = 127;

// PAD TYPES
const byte PADTYPE_NOTE         = 0;
const byte PADTYPE_CHANNEL      = 1;
const byte PADTYPE_OCTAVE       = 2;

// FIRST BYTE GENERAL
const byte NOTE_ON              = 144;
const byte NOTE_OFF             = 128;
const byte CONTROL              = 176;      
const byte INSTRUMENT           = 192; 

// SECOND BYTE GENERAL 
const byte CTRL_VOL             =   7;
const byte CTRL_MOD             =   1;
const byte CTRL_EFCT1           =  12;
const byte CTRL_EFCT2           =  13;
const byte CTRL_NOTEOFF         = 123;

// SECOND BYTE NOTE VALUES ------------------------------------------------------------------------------------------------------------------------------------
// OCTAVE 00              || OCTAVE 01               || OCTAVE 02               || OCTAVE 03              || OCTAVE 04               || OCTAVE 05
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
const byte NOTE_C0  =  0  ;const byte NOTE_C1  = 12  ;const byte NOTE_C2  = 24  ;const byte NOTE_C3  = 36  ;const byte NOTE_C4  = 48  ;const byte NOTE_C5  = 60;
const byte NOTE_C0s =  1  ;const byte NOTE_C1s = 13  ;const byte NOTE_C2s = 25  ;const byte NOTE_C3s = 37  ;const byte NOTE_C4s = 49  ;const byte NOTE_C5s = 61;
const byte NOTE_D0  =  2  ;const byte NOTE_D1  = 14  ;const byte NOTE_D2  = 26  ;const byte NOTE_D3  = 38  ;const byte NOTE_D4  = 50  ;const byte NOTE_D5  = 62;
const byte NOTE_D0s =  3  ;const byte NOTE_D1s = 15  ;const byte NOTE_D2s = 27  ;const byte NOTE_D3s = 39  ;const byte NOTE_D4s = 51  ;const byte NOTE_D5s = 63;
const byte NOTE_E0  =  4  ;const byte NOTE_E1  = 16  ;const byte NOTE_E2  = 28  ;const byte NOTE_E3  = 40  ;const byte NOTE_E4  = 52  ;const byte NOTE_E5  = 64;
const byte NOTE_F0  =  5  ;const byte NOTE_F1  = 17  ;const byte NOTE_F2  = 29  ;const byte NOTE_F3  = 41  ;const byte NOTE_F4  = 53  ;const byte NOTE_F5  = 65;
const byte NOTE_F0s =  6  ;const byte NOTE_F1s = 18  ;const byte NOTE_F2s = 30  ;const byte NOTE_F3s = 42  ;const byte NOTE_F4s = 54  ;const byte NOTE_F5s = 66;
const byte NOTE_G0  =  7  ;const byte NOTE_G1  = 19  ;const byte NOTE_G2  = 31  ;const byte NOTE_G3  = 43  ;const byte NOTE_G4  = 55  ;const byte NOTE_G5  = 67;
const byte NOTE_G0s =  8  ;const byte NOTE_G1s = 20  ;const byte NOTE_G2s = 32  ;const byte NOTE_G3s = 44  ;const byte NOTE_G4s = 56  ;const byte NOTE_G5s = 68;
const byte NOTE_A0  =  9  ;const byte NOTE_A1  = 21  ;const byte NOTE_A2  = 33  ;const byte NOTE_A3  = 45  ;const byte NOTE_A4  = 57  ;const byte NOTE_A5  = 69;
const byte NOTE_A0s = 10  ;const byte NOTE_A1s = 22  ;const byte NOTE_A2s = 34  ;const byte NOTE_A3s = 46  ;const byte NOTE_A4s = 58  ;const byte NOTE_A5s = 70;
const byte NOTE_B0  = 11  ;const byte NOTE_B1  = 23  ;const byte NOTE_B2  = 35  ;const byte NOTE_B3  = 47  ;const byte NOTE_B4  = 59  ;const byte NOTE_B5  = 71;

// SECOND BYTE NOTE VALUES -----------------------------------------------------------------------------------------------------------
// OCTAVE 06              || OCTAVE 07               || OCTAVE 08               || OCTAVE 09               || OCTAVE 10           
//------------------------------------------------------------------------------------------------------------------------------------
const byte NOTE_C6  = 72  ;const byte NOTE_C7  = 84  ;const byte NOTE_C8  = 96  ;const byte NOTE_C9  =108  ;const byte NOTE_C10  =120;
const byte NOTE_C6s = 73  ;const byte NOTE_C7s = 85  ;const byte NOTE_C8s = 97  ;const byte NOTE_C9s =109  ;const byte NOTE_C10s =121;
const byte NOTE_D6  = 74  ;const byte NOTE_D7  = 86  ;const byte NOTE_D8  = 98  ;const byte NOTE_D9  =110  ;const byte NOTE_D10  =122;
const byte NOTE_D6s = 75  ;const byte NOTE_D7s = 87  ;const byte NOTE_D8s = 99  ;const byte NOTE_D9s =111  ;const byte NOTE_D10s =123;
const byte NOTE_E6  = 76  ;const byte NOTE_E7  = 88  ;const byte NOTE_E8  =100  ;const byte NOTE_E9  =112  ;const byte NOTE_E10  =124;
const byte NOTE_F6  = 77  ;const byte NOTE_F7  = 89  ;const byte NOTE_F8  =101  ;const byte NOTE_F9  =113  ;const byte NOTE_F10  =125;
const byte NOTE_F6s = 78  ;const byte NOTE_F7s = 90  ;const byte NOTE_F8s =102  ;const byte NOTE_F9s =114  ;const byte NOTE_F10s =126;
const byte NOTE_G6  = 79  ;const byte NOTE_G7  = 91  ;const byte NOTE_G8  =103  ;const byte NOTE_G9  =115;
const byte NOTE_G6s = 80  ;const byte NOTE_G7s = 92  ;const byte NOTE_G8s =104  ;const byte NOTE_G9s =116;
const byte NOTE_A6  = 81  ;const byte NOTE_A7  = 93  ;const byte NOTE_A8  =105  ;const byte NOTE_A9  =117;
const byte NOTE_A6s = 82  ;const byte NOTE_A7s = 94  ;const byte NOTE_A8s =106  ;const byte NOTE_A9s =118;
const byte NOTE_B6  = 83  ;const byte NOTE_B7  = 95  ;const byte NOTE_B8  =107  ;const byte NOTE_B9  =119;

//------------------------------------------------------------------------------------------------------------------------------------------
// COLOR RED [0:255]          || COLOR GREEN [0:255]         || COLOR BLUE [0:255]           || Comments
//------------------------------------------------------------------------------------------------------------------------------------------
const byte GEN_R_L0   =  0    	 ;const byte GEN_G_L0   =  50  		;const byte GEN_B_L0  =   40;      // RGB L0
const byte GEN_R_L1   =  10    	 ;const byte GEN_G_L1   =  40  		;const byte GEN_B_L1  =   50;      // RGB L1
const byte GEN_R_L2   =  20   	 ;const byte GEN_G_L2   =  30  		;const byte GEN_B_L2  =   0;       // RGB L2
const byte GEN_R_L3   =  30  	   ;const byte GEN_G_L3   =  20  		;const byte GEN_B_L3  =   10;      // RGB L3
const byte GEN_R_L4   =  40      ;const byte GEN_G_L4   =  10     ;const byte GEN_B_L4  =   20;      // RGB L4
const byte GEN_R_L5   =  50      ;const byte GEN_G_L5   =  0      ;const byte GEN_B_L5  =   30;      // RGB L5
const byte FULL_R     =  150     ;const byte FULL_G     =  150    ;const byte FULL_B    =   150;     // FULL WHITE

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Musical Parameters--------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
byte SCALES[6][nPADNOTE] = {{NOTE_C0,  NOTE_D0,   NOTE_E0,   NOTE_F0,   NOTE_G0,   NOTE_A0,   NOTE_B0,  NOTE_C1},   // Escala Mayor             I-II-III-IV-V-VI-VII
                            {NOTE_C0,  NOTE_D0,   NOTE_D0s,  NOTE_F0,   NOTE_G0,   NOTE_A0,   NOTE_A0s, NOTE_C1},   // Escala Menor Natural     I-II-IIIb-IV-V-VI-VIIb
                            {NOTE_C0,  NOTE_E0,   NOTE_G0,   NOTE_B0,   NOTE_C1,   NOTE_E1,   NOTE_G1,  NOTE_C1},   // Escala Arpegio Maj7      I-III-V-VII-I'-III'-V'
                            {NOTE_C0,  NOTE_D0s,  NOTE_G0,   NOTE_A0s,  NOTE_C1,   NOTE_D1s,  NOTE_G1,  NOTE_C1},   // Escala Arpegio Men7      I-IIb-V-VIIb-I'-IIIb'-V'
                            {NOTE_C0,  NOTE_D0,   NOTE_E0,   NOTE_G0,   NOTE_A0,   NOTE_C1,   NOTE_D1,  NOTE_C1},   // Escala Pentatonico Mayor I-II-III-V-VI-I'-II'
                            {NOTE_D0s, NOTE_F0,   NOTE_F0,   NOTE_G0,   NOTE_A0s,  NOTE_C1,   NOTE_D1s, NOTE_D1s}	  // Escala Pentatonico Menor I-IIIb-IV-VIIb-I'-IIIb'
};  

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
int padNote_muxChannel[nPADNOTE] =  {8,9,10,11,12,13,14,15};	// Pad Multeplexor Channel Definition
int muxSel[4]                    = {MUX_D, MUX_C, MUX_B, MUX_A};
int modeTrigger                  = OFF;
int generalThr                   = 0;
int octaveCorrector[6]           = {0, 1, 0, 0, 0, 1};
byte currentInstrument           = 0;   // Channel is also the Instrument
int currentOctave                = 5;   // Starting at value NOTE_C5 60
// PAD Definition
padObject padNote[nPADNOTE];
padObject padInstrument;
padObject padOctave;

Adafruit_NeoPixel ledRGB = Adafruit_NeoPixel(1, LED, NEO_RGB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  // WS2812B LED Initialization
  ledRGB.begin();
  ledRGB.show();
  // Pin Initialization
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);
  pinMode(MUX_D, OUTPUT);
  // padNote initialization
  init_allPadNote(0);
  init_allPadOctave();
  init_allPadInstrument();
  // Marks the start of operations
  blinkLed();  
  blinkLed();
  blinkLed();
  blinkLed();
  blinkLed();
}

/*
 * LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
 */
void loop() {
  generalThr = 150;
  for(int i = 0; i < nPADNOTE; i++){
    read_singlePad(&padNote[i]);
  }
  //read_singlePad(&padOctave);
  //read_singlePad(&padInstrument);
}

void readPAD

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
