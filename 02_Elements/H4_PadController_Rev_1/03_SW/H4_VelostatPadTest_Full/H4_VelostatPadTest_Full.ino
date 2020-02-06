/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief This code controls a number Velostat Pad to be tested
 * @date 2020_01_28
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
const byte PADTYPE_CHANNEL   = 1;
const byte PADTYPE_OCTAVE       = 2;

// FIRST BYTE GENERAL
const byte NOTE_ON              = 144;
const byte NOTE_OFF             = 128;
const byte CONTROL              = 176;

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
const byte GEN_R_L2   =  20   	 ;const byte GEN_G_L2   =  30  		;const byte GEN_B_L2  =   0;        // RGB L2
const byte GEN_R_L3   =  30  	 ;const byte GEN_G_L3   =  20  		  ;const byte GEN_B_L3  =   10;       // RGB L3
const byte GEN_R_L4   =  40     ;const byte GEN_G_L4   = 10       ;const byte GEN_B_L4  =   20;       // RGB L4
const byte GEN_R_L5   =  50     ;const byte GEN_G_L5   =  0        ;const byte GEN_B_L5  =   30;      // RGB L5
const byte FULL_R     =  150     ;const byte FULL_G     =  150      ;const byte FULL_B    =   150;      // FULL WHITE

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
  bool padStatus = OFF;
  int lastMeasures[nMEASURES];
  byte padType = 0;             // For a padObject is a NOTE
  byte execValue = 0;             // For a padObject is a NOTE
};


// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Global Variables----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------
int padNote_muxChannel[nPADNOTE] =  {15,14,13,12,11,10,9,8};	// Pad Multeplexor Channel Definition
int muxSel[4] = {MUX_D, MUX_C, MUX_B, MUX_A};
int lastMeasures[nPADNOTE][nMEASURES];
int modeTrigger = OFF;
int generalThr = 0;
int currentChannel = 0;   // Channel is also the Instrument
int currentOctave  = 5;   // Starting at value NOTE_C5 60
// PAD Definition
padObject padNote[nPADNOTE];
padObject padChannel;
padObject padOctave;


Adafruit_NeoPixel ledRGB = Adafruit_NeoPixel(1, LED, NEO_RGB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  // WS2812B LED Initialization
  ledRGB.begin();
  ledRGB.show();
  // Pin Initialization
  //pinMode(MUX_0, INPUT);
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);
  pinMode(MUX_C, OUTPUT);
  pinMode(MUX_D, OUTPUT);
  // padNote initialization
  init_allPadNote(0);
  init_allPadOctave();
  init_allPadChannel();
  // Marks the start of operations
  blinkLed();  
  blinkLed();
  blinkLed();
  blinkLed();
  blinkLed();
}

void loop() {
  generalThr = 100;
  for(int i = 0; i < 1; i++){
    read_singlePad(&padNote[i]);
  }
  /*read_singlePad(&padOctave);
  read_singlePad(&padChannel);*/

  Serial << " padNote[i]: " << padNote[0].padType << "--ON----------------------------------------------------------------------" << endl;
  //Serial << " padChannel: " << padChannel.padType << "--ON----------------------------------------------------------------------" << endl;
}

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
            exec_padNote(OFF, currentChannel, (pad->execValue + (currentOctave * 12)), 0);
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
        Serial << " pad->padType: " << pad->padType << "--ON----------------------------------------------------------------------" << endl;
        switch(pad->padType){
          case PADTYPE_NOTE:
            exec_padNote(ON, currentChannel, (pad->execValue + (currentOctave * 12)), map(((avgLevel - generalThr) - newRead), 0, (avgLevel - generalThr), MIN_MIDIVELOCITY, MAX_MIDIVELOCITY));
            break;
          case PADTYPE_OCTAVE:
            pad->execValue = exec_padOctave(ON);
            break;
          case PADTYPE_CHANNEL:
            pad->execValue = exec_padChannel(ON);
            break;
          default:
            // Nothing to do
            break;
        }
        blinkLed();
      }
      break;
  }
  delay(10);
}

void exec_padNote(byte ONOFF, byte channelToPlay, byte noteToPlay, byte velocityToPlay){
  switch (ONOFF){
    case ON:
      Serial << " NOTE: " << noteToPlay << "--ON----------------------------------------------------------------------" << endl;
      //MIDI_TX(channelToPlay, NOTE_ON, noteToPlay, velocityToPlay);
      break;
    default: // OFF
      Serial << " NOTE: " << noteToPlay << "--OFF---------------------------------------------------------------------" << endl;
      //MIDI_TX(channelToPlay, NOTE_OFF, noteToPlay, 0);
      break;
  }
}

int exec_padOctave(byte ONOFF){
  switch (ONOFF){
    case ON:
      //MIDIOFF_FIX(currentChannel, 0, 127);
      if(currentOctave == 5){
        currentOctave = 4;
      }else{
        currentOctave = 5;
      }
      Serial << " CHANNEL: " << currentOctave << "--ON----------------------------------------------------------------------" << endl;
      return currentOctave;
      break;
    default: // OFF
      // Nothing to do
      break;
  }
}

int exec_padChannel(byte ONOFF){
  switch (ONOFF){
    case ON:
      //MIDIOFF_FIX(currentChannel, 0, 127);
      if(currentChannel < 5){
        currentChannel++;
      }else{
        currentChannel = 0;
      }
      Serial << " CHANNEL: " << currentChannel << "--ON----------------------------------------------------------------------" << endl;
      return currentChannel;
      
      break;
    default: // OFF
      // Nothing to do
      break;
  }
}

void init_allPadOctave(){
  padOctave.muxChannel = 1;               // TODO Fixed Value
  padOctave.execValue = currentOctave;
  padOctave.padType = PADTYPE_OCTAVE;
}

void init_allPadChannel(){
  padChannel.muxChannel = 0;           // TODO Fixed Value
  padChannel.execValue = currentChannel;
  padChannel.padType = PADTYPE_CHANNEL;
}

void init_allPadNote(int scale){
	for(int i = 0; i < nPADNOTE; i++){
		padNote[i].muxChannel = padNote_muxChannel[i];
    padNote[i].padType = PADTYPE_NOTE;
	}
	set_allPadNote_scale(scale);
}

void set_allPadNote_scale(int scale){
	//MIDIOFF_FIX(currentChannel, 0, 127);
	for(int i = 0; i < nPADNOTE; i++){
		padNote[i].execValue = SCALES[scale][i];
	}
}

int updateMeasureArray(int newValue, padObject *pad){
  //lastMeasures[0] = newValue;
  set_muxGate(pad->muxChannel);
  for(int i = 0; i < nMEASURES; i++){
    pad->lastMeasures[i + 1] = pad->lastMeasures[i];
  }
  pad->lastMeasures[0] = newValue;
}

int calculateMeasureArray(padObject *pad){
  int averageValue = 0;
  for(int i = 0; i < nMEASURES; i++){
    averageValue = (averageValue + pad->lastMeasures[i]);
  }
  return (averageValue/nMEASURES);
}

void blinkLed(){
    ledRGB.setPixelColor(0, FULL_G, FULL_R, FULL_B);
    ledRGB.show();
    delay(100);
    instrumentLed(currentChannel + 1);
}

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

void set_muxGate(int muxChannel){
  for (int h = 0; h < 4; h++) {
      digitalWrite(muxSel[h], ((muxChannel >> (3 - h)) & 0x01)); //MUX Setup pin for each MUX Pin
      //Serial << muxSel[h] << " VALUE: " << ((muxChannel >> (3 - h)) & 0x01) <<endl;
  }
  
  delay(15); // Delay por MUX Setup
}

void MIDI_TX(byte MIDICHANNEL, byte MESSAGE, byte PITCH, byte VELOCITY) {
  //byte status1 = MESSAGE + MIDICHANNEL;
  midiEventPacket_t event = {(byte)(MESSAGE >> 4), (byte)(MESSAGE | (MIDICHANNEL)), PITCH, VELOCITY};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void MIDIOFF_FIX(byte CHANNEL, int INIT_NOTE, int END_NOTE){ // This is a very rude and nasty sustitution to a General Note Off CMD
  for(int i= INIT_NOTE; i <= END_NOTE; i++){
    //MIDI_TX(CHANNEL, NOTE_OFF, i, 0);
    delay(10);
  }
}
