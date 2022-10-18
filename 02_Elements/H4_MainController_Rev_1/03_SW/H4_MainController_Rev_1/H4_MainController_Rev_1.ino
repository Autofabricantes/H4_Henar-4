/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls a full system consisting of:
 *         18x PadNotes
 *         01x PadInstrument (Double Pad)
 *         01x PadScaler  (Double Pad)
 *         01x PadMode (Double Pad)
 *         01x PadOctave
 *         01x PadProNatural
 *         01x PadVoiceCommand
 *         
 *         Please I now it sounds confusing, probably you should check the documentation first.
 * @date   2022_09_29
 */

// Library Import
#include <Wire.h>
#include "H4_PadController.h"
#include "H4_MIDIController.h"
#include "H4_LEDSignal.h"
// Modes
#define PAD_TESTING
// LEDSIGNALLING controls the LED in the MAINBoard to blink everytime a MIDI Message is sent. It does not work very well yet...
//#define LEDSIGNALLING
// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------System Pin Definition-----------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
#define LEDSIGNAL     6
// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Control and Configuration values Definition-------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Constant Value Definition
const bool OFF                  = 0;
const bool ON                   = 1;
const byte qPadNotes             = 18;
const byte OCTAVE_MIN            = 4;
const byte OCTAVE_MAX            = 5;
const byte INSTRUMENT_MIN        = 0;
const byte INSTRUMENT_MAX        = 5;
const byte SCALE_MIN             = 0;
const byte SCALE_MAX             = 5;
const byte NATURAL              = 0;
const byte PRO                  = 1;
const byte MODE_MIN             = 0;
const byte MODE_MAX             = 11;

const byte PadInstrumentColor[6][2] = {{TURQOUISE_HIGH, WHITE_LOW},
                            {GREEN_HIGH, WHITE_LOW},
                            {BLUE_HIGH, WHITE_LOW},
                            {ORANGE_HIGH, WHITE_LOW},
                            {PURPLE_HIGH, WHITE_LOW},
                            {ZIEL_HIGH, WHITE_LOW}
};

const byte PadOctaverColor[2][2] = {{BLUE_LOW, WHITE_LOW},
                            {BLUE_HIGH, WHITE_LOW}};

const byte PadProNaturalColor[2][2] = {{GREEN_LOW, WHITE_LOW},
                                      {PURPLE_LOW, WHITE_LOW}
};

const byte PadScalerColor[2][2] = {{BLACK_OFF, BLACK_OFF},
                                  {YELLOW_LOW, WHITE_LOW}
};

const byte PadModeColor[2][2] = {{BLACK_OFF, BLACK_OFF},
                                   {RED_LOW, WHITE_LOW}
};

const byte PadVoiceDescriptionColor[2][2] = {{RED_LOW, WHITE_LOW},
                                   {ORANGE_LOW, WHITE_LOW}
};

byte controlMessageInstrument[INSTRUMENT_MAX + 1] = {NOTE_C2, NOTE_C2s, NOTE_D2, NOTE_D2s, NOTE_G1s, NOTE_A1};
byte controlMessageOctaver[OCTAVE_MAX - OCTAVE_MIN + 1] = {NOTE_B1,NOTE_A1s};
byte controlMessageScaler[SCALE_MAX + 1] = {NOTE_A2, NOTE_A2s, NOTE_B2, NOTE_E2, NOTE_F2, NOTE_F2s};
byte controlMessageMode[MODE_MAX + 1] = {NOTE_C0, NOTE_C0s, NOTE_D0, NOTE_D0s, NOTE_E0, NOTE_F0,NOTE_F0s, NOTE_G0, NOTE_G0s, NOTE_A0, NOTE_A0s, NOTE_B0};
byte controlMessageProNatural[PRO + 1] = {NOTE_E1, NOTE_F1};

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Custom Configuration Structs----------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Global Variables----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
int inMssg[10] = {0,0,0,0,0,0,0,0,0,0};

int PadProActivation[2][18] = {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                              {1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,1,0,1}};

// Pad I2C Direction definition
H4_PadController PadNotes[qPadNotes] = {H4_PadController(0,14),
                                 H4_PadController(1,37),
                                 H4_PadController(2,29),
                                 H4_PadController(3,27),
                                 H4_PadController(4,36),
                                 H4_PadController(5,26),
                                 H4_PadController(6,43),
                                 H4_PadController(7,25),
                                 H4_PadController(8,24),
                                 H4_PadController(9,35),
                                 H4_PadController(10,23),
                                 H4_PadController(11,32),
                                 H4_PadController(12,22),
                                 H4_PadController(13,31),
                                 H4_PadController(14,21),
                                 H4_PadController(15,20),
                                 H4_PadController(16,34),
                                 H4_PadController(17,42)};
H4_PadController PadOctaver     = H4_PadController(0,10);
H4_PadController PadInstrument  = H4_PadController(0,39);
H4_PadController PadScaler      = H4_PadController(0,41);
H4_PadController PadMode        = H4_PadController(0,40);
H4_PadController PadProNatural  = H4_PadController(0,18);
H4_PadController PadVoiceDescription = H4_PadController(0,12);

H4_MIDIController MIDI          = H4_MIDIController();
#ifdef  LEDSIGNALLING
  H4_LEDSignal LEDSignal          = H4_LEDSignal(LEDSIGNAL);
#endif

int octaveCorrector[6]              = {0, 1, 0, 0, 0, 1};
byte currentInstrument              = INSTRUMENT_MIN;   // Channel is also the Instrument
byte currentScale                   = SCALE_MIN;   // Channel is also the Instrument
int currentOctave                   = OCTAVE_MIN;   // Starting at value NOTE_C5 60
byte currentMode                    = MODE_MIN;
byte currentProNatural              = NATURAL;
byte currentVoiceDescription  = ON;

int iterator = 0;


// Define functions to trick the compiler
void read_octaverPad();
void read_voiceDescriptionPad();
void read_instrumentPad();
void read_modePad();
void read_scalerPad();
void read_proNaturalPad();
void set_allPadNote(int scaleId, int instrumentId, boolean proNaturalMode);
void set_PadInstrument(int instrumentId);
void set_PadMode(int proNatural);
void set_PadScaler(int proNatural);
void set_PadOctaver();
void set_PadVoiceDescription(bool activeVoiceDescription);
void set_PadProNatural(int proNatural);
void swipeColor_allPads();
void individualSwipeColor_SinglePad(H4_PadController *padToPerform);
void individualSwipeColor_allPads();


void setup() {
  // i2C Initialization
  Wire.begin();
  Wire.onReceive(recieveRequestResponse); // register event
  Serial.begin(115200);
  delay(1500);  // Small delay for all elements to power up
#ifdef  LEDSIGNALLING
  LEDSignal.init();
  LEDSignal.blinkColorAndStay(WHITE_BRIGHT, WHITE_LOW, MIN_DURATION);
#endif
  padInitialization(currentProNatural);
  // Only to Initialice new ones
  //while(1){setNewI2cDirFromDefault(42);};
}

/*
 * LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
 */
void loop() {
  read_allPads();
  read_octaverPad();
  read_voiceDescriptionPad();
  read_instrumentPad();
  read_modePad();
  read_scalerPad();
  read_proNaturalPad();
}

/**
 * @brief Groups all the H4_PadController initializations
 * @param proNatural current proNatural Mode
 */
void padInitialization(int proNatural){
  // Stablish the default values except ProNatural
  currentInstrument           = INSTRUMENT_MIN;   // Channel is also the Instrument
  currentScale                = SCALE_MIN;   // Channel is also the Instrument
  currentOctave               = OCTAVE_MIN;   // Starting at value NOTE_C5 60
  currentMode                 = MODE_MIN;
  // Initialize all Pads
  set_PadInstrument(currentInstrument);
  set_PadScaler(proNatural);
  set_PadMode(proNatural);
  set_PadOctaver();
  set_PadProNatural(currentProNatural);
  set_PadVoiceDescription(currentVoiceDescription);
  set_allPadNote(currentScale, currentInstrument, proNatural);
}

/**
 * @brief Function to recieve the incoming I2C Message and store it into a input buffer (inMssg)
 * @param HowMany Unused
 */
void recieveRequestResponse(int HowMany){
  ////Serial.println("\nRecieve1:");
  int i = 0;
  while(Wire.available()){ // loop through all but the last
    char in = Wire.read();    // receive byte as an integer
    inMssg[i] = in;
    //Serial.print(inMssg[i]);
    i++;
  }
}

/**
 * @brief I2C Command to H4_PadController to change the default I2C Direction (0x08) and store a new one in its EEPROM. It is mainly used during early configuration.
 * @param newDir  New I2C Direction
 */
void setNewI2cDirFromDefault(int newDir){
  change_i2cDir(8, newDir);
}

/**
 * @brief I2C Command to H4_PADController to change the I2C Direction and store a new one in its EEPROM. It is mainly used during early configuration.
 * @param oldDir  Old I2C Direction
 * @param newDir  New I2C Direction
 */
void change_i2cDir(int oldDir, int newDir){
  /*//Serial.println(oldDir);
  //Serial.println(newDir);*/
  int changei2c[2] = {SET_I2CDIR, newDir}; 
  Wire.beginTransmission(oldDir); 
  for(int i = 0; i < 2; i++){
     Wire.write(changei2c[i]);
     //Serial.println(changei2c[i]);
  }
  Wire.endTransmission();
}

/**
 * @brief Makes an I2C Polling of all the Note_PAD in PadNotes. If a ON/OFF Step is detected, launches the according MIDI Message
 */
void read_allPads(){
  int padResult = 0;
  for(int i = 0; i < qPadNotes; i++){
      padResult = PadNotes[i].get_padEvent(0);
      if(padResult == EVENT_TO_ON){
        if(currentProNatural == NATURAL){
          MIDI.MIDI_TX((byte)PadNotes[i].INSTRUCTION.channelCode, (byte)PadNotes[i].INSTRUCTION.instructionCode_0, (byte)(PadNotes[i].INSTRUCTION.pitchCode + ((currentOctave + octaveCorrector[currentInstrument]) * 12)), (byte)PadNotes[i].INSTRUCTION.velocityCode_0);
        }else{ // PRO MODE
          MIDI.MIDI_TX((byte)PadNotes[i].INSTRUCTION.channelCode, (byte)PadNotes[i].INSTRUCTION.instructionCode_0, ((byte)(PadNotes[i].INSTRUCTION.pitchCode + ((currentOctave + octaveCorrector[currentInstrument]) * 12)) + currentMode), (byte)PadNotes[i].INSTRUCTION.velocityCode_0);
        }

#ifdef  LEDSIGNALLING
        LEDSignal.blinkColorAndStay(PadNotes[i].CONF.primaryColor, WHITE_LOW, MIN_DURATION);
#endif   
      }else if(padResult == EVENT_TO_OFF){
        if(currentProNatural == NATURAL){
          MIDI.MIDI_TX((byte)PadNotes[i].INSTRUCTION.channelCode, (byte)PadNotes[i].INSTRUCTION.instructionCode_1, (byte)(PadNotes[i].INSTRUCTION.pitchCode + ((currentOctave + octaveCorrector[currentInstrument]) * 12)), (byte)PadNotes[i].INSTRUCTION.velocityCode_1);
        }else{ // PRO MODE
          MIDI.MIDI_TX((byte)PadNotes[i].INSTRUCTION.channelCode, (byte)PadNotes[i].INSTRUCTION.instructionCode_1, ((byte)(PadNotes[i].INSTRUCTION.pitchCode + ((currentOctave + octaveCorrector[currentInstrument]) * 12)) + currentMode), (byte)PadNotes[i].INSTRUCTION.velocityCode_1);
        }
#ifdef  LEDSIGNALLING
        LEDSignal.blinkColorAndStay(PadNotes[i].CONF.primaryColor, WHITE_LOW, MIN_DURATION);
#endif
      }
  }
}

/**
 * @brief Sets all the PadNotes to the according configuration regarding scale, instrument, and ProNatural mode
 */
void set_allPadNote(int scaleId, int instrumentId, boolean proNaturalMode){
   // Initialize values
   int primaryColorToSet;
   int secondaryColorToSet;
   int scaleToSet;
   boolean ch1ActivationToSet;
   for(int i = 0; i < qPadNotes; i++){
    if(proNaturalMode == NATURAL){
      // Natural Mode
      primaryColorToSet = PadInstrumentColor[instrumentId][0];
      secondaryColorToSet = PadInstrumentColor[instrumentId][1];
      scaleToSet = NATURAALSCALE;
      ch1ActivationToSet = PadProActivation[proNaturalMode][i];
    }else{
      // Pro Mode
      primaryColorToSet = PadInstrumentColor[instrumentId][0];
      secondaryColorToSet = PadInstrumentColor[instrumentId][1];
      scaleToSet = 1 + scaleId;
      ch1ActivationToSet = PadProActivation[proNaturalMode][i];
      if(!PadProActivation[proNaturalMode][i]){
        // Not active Pad
        primaryColorToSet = OFFCOLOR;
        secondaryColorToSet = OFFCOLOR;
        
      }
    }
       //Serial.println(i);
       // Set the led with the defined configuration      
       PadNotes[i].set_padConfiguration(primaryColorToSet,secondaryColorToSet,
                                  DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                  BLINKDURATION,FADEDURATION,
                                  PadProActivation[proNaturalMode][i],CH_OFF,CH_OFF,CH_OFF,
                                  CH_THR,CH_THR,CH_THR,CH_THR);
       PadNotes[i].set_padInstruction(instrumentId, NOTE_ON, NOTE_OFF, SCALES[scaleToSet][i], MAX_MIDIVELOCITY, 0);
    }
    swipeColor_allPads();   // Makes the visual changes to the new configuration
}

/**
 * @brief Sets the PadInstrument according to the current instrumentId
 * @param instrumentId  current instrument ID
 */
void set_PadInstrument(int instrumentId){
  int primaryColorToSet = PadInstrumentColor[instrumentId][0];
  int secondaryColorToSet = PadInstrumentColor[instrumentId][1];
  PadInstrument.set_padConfiguration(primaryColorToSet,secondaryColorToSet,
                                  DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                  BLINKDURATION,FADEDURATION,
                                  CH_ON,CH_OFF,CH_OFF,CH_ON,
                                  CH_THR,CH_THR,CH_THR,CH_THR);
  individualSwipeColor_SinglePad(&PadInstrument);
}

/**
 * @brief Sets the PadMode according to the current PadMode (global) and proNatural Mode
 * @param proNatural  current proNatural Mode
 */
void set_PadMode(int proNatural){
  PadMode.set_padConfiguration(PadModeColor[proNatural][0],
                                    PadModeColor[proNatural][1],
                                    DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                    BLINKDURATION,FADEDURATION,
                                    proNatural,
                                    CH_OFF,CH_OFF,
                                    proNatural,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
   individualSwipeColor_SinglePad(&PadMode);
}

/**
 * @brief Sets the PadScaler according to the current PadScale (global) and proNatural Mode
 * @param proNatural  current proNatural Mode
 */
void set_PadScaler(int proNatural){
  PadScaler.set_padConfiguration(PadScalerColor[proNatural][0],
                                    PadScalerColor[proNatural][1],
                                    DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                    BLINKDURATION,FADEDURATION,
                                    proNatural,
                                    CH_OFF,CH_OFF,
                                    proNatural,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
   individualSwipeColor_SinglePad(&PadScaler);
}

/**
 * @brief Sets the PadOctaver according to the current PadOctaver (global) and proNatural Mode
 * @param proNatural  current proNatural Mode
 */
void set_PadOctaver(){
  PadOctaver.set_padConfiguration(PadOctaverColor[0][0],
                                    PadOctaverColor[0][1],
                                    DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                    BLINKDURATION,FADEDURATION,
                                    CH_ON,
                                    CH_OFF,CH_OFF,CH_OFF,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
   individualSwipeColor_SinglePad(&PadOctaver);
}

/**
 * @brief Sets the PadVoiceDescription according to the current activeVoiceDescription Mode
 * @param activeVoiceDescription  current activeVoiceDescription Mode
 */
void set_PadVoiceDescription(bool activeVoiceDescription){
  PadVoiceDescription.set_padConfiguration(PadVoiceDescriptionColor[activeVoiceDescription][0],
                                    PadVoiceDescriptionColor[activeVoiceDescription][1],
                                    DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                    BLINKDURATION,FADEDURATION,
                                    CH_ON,
                                    CH_OFF,CH_OFF,CH_OFF,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
   individualSwipeColor_SinglePad(&PadVoiceDescription);
}

/**
 * @brief Sets the PadProNatural according to the current PadProNatural Mode
 * @param proNatural  current proNatural Mode
 */
void set_PadProNatural(int proNatural){
  PadProNatural.set_padConfiguration(PadProNaturalColor[proNatural][0],
                                    PadProNaturalColor[proNatural][1],
                                    DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                    BLINKDURATION,FADEDURATION,
                                    CH_ON,
                                    CH_OFF,CH_OFF,CH_OFF,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
   individualSwipeColor_SinglePad(&PadProNatural);
}

/**
 * @brief Makes a color swipe of all the PadNotes from beginning to end
 */
void swipeColor_allPads(){
  // Turn them all to ON
  for(int i = 0; i < qPadNotes; i++){
    PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.fadeDuration);
    delay(50);
  }
  // Turn them all to OFF
  for(int i = 0; i < qPadNotes; i++){
    PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.fadeDuration);
    delay(50);
  }
}

/**
 * @brief Makes a color swipe of a single Pad according to its stored parameters
 */
void individualSwipeColor_SinglePad(H4_PadController *padToPerform){
   padToPerform->set_padLedActivity(LED_ID_FADEFROMNUCLEO, padToPerform->CONF.secondaryColor, padToPerform->CONF.primaryColor, padToPerform->CONF.fadeDuration);
   delay(50);
   padToPerform->set_padLedActivity(LED_ID_FADEFROMNUCLEO, padToPerform->CONF.primaryColor, padToPerform->CONF.secondaryColor, padToPerform->CONF.fadeDuration);
}

/**
 * @brief Makes a color swipe of all the PadNotes from beginning to end
 */
void individualSwipeColor_allPads(){
  // Turn them all to ON
  for(int i = 0; i < qPadNotes; i++){
    PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.fadeDuration);
    delay(50 + PadNotes[i].CONF.fadeDuration);
    PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.fadeDuration);
  }
}

/*
 * @brief Makes a read of the PadOctaver and performs actions if needed.
 */
void read_octaverPad(){
  int padResult = 0;
  padResult = PadOctaver.get_padEvent(0);
  if(padResult == EVENT_TO_ON){
    if(currentOctave == OCTAVE_MIN){
      currentOctave = OCTAVE_MAX;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageOctaver[currentOctave - OCTAVE_MIN], 127);
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C3, NOTE_C10);
    }else if(currentOctave == OCTAVE_MAX){
      currentOctave = OCTAVE_MIN;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageOctaver[currentOctave - OCTAVE_MIN], 127);
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C3, NOTE_C10);
    }
  }else{
    // Do nothing
  }
}

/*
 * @brief Makes a read of the PadInstrument and performs actions if needed. Circular.
 */
void read_instrumentPad(){
  int padUp = 0;
  int padDown = 0;
  padUp = PadInstrument.get_padEvent(3);
  padDown = PadInstrument.get_padEvent(0);
  if(padUp == EVENT_TO_ON){
    if(currentInstrument < INSTRUMENT_MAX){
      currentInstrument++;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageInstrument[currentInstrument], 127);
      set_PadInstrument(currentInstrument);
      MIDI.MIDIOFF_FIX((currentInstrument-1), NOTE_C3, NOTE_C10);
      set_allPadNote(currentScale, currentInstrument, currentProNatural);
    }else if(currentInstrument == INSTRUMENT_MAX){
      currentInstrument=INSTRUMENT_MIN;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageInstrument[currentInstrument], 127);
      set_PadInstrument(currentInstrument);
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C3, NOTE_C10);
      set_allPadNote(currentScale, currentInstrument, currentProNatural);
    }
  }
  if(padDown == EVENT_TO_ON){
    if(currentInstrument > INSTRUMENT_MIN){
      currentInstrument--;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageInstrument[currentInstrument], 127);
      set_PadInstrument(currentInstrument);
      MIDI.MIDIOFF_FIX((currentInstrument+1), NOTE_C3, NOTE_C10);
      set_allPadNote(currentScale, currentInstrument, currentProNatural);
    }else if(currentInstrument == INSTRUMENT_MIN){
      currentInstrument=INSTRUMENT_MAX;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageInstrument[currentInstrument], 127);
      set_PadInstrument(currentInstrument);
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C3, NOTE_C10);
      set_allPadNote(currentScale, currentInstrument, currentProNatural);
    }
  }
}

/*
 * @brief Makes a read of the PadScaler and performs actions if needed. Non Circular
 */
void read_scalerPad(){
  int padUp = 0;
  int padDown = 0;
  padUp = PadScaler.get_padEvent(3);
  padDown = PadScaler.get_padEvent(0);
  if(padUp == EVENT_TO_ON){
    if(currentScale < SCALE_MAX){
      currentScale++;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageScaler[currentScale], 127);
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C3, NOTE_C10);
      set_allPadNote(currentScale, currentInstrument, currentProNatural);
    }
  }else if(padDown == EVENT_TO_ON){
    if(currentScale > SCALE_MIN){
      currentScale--;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageScaler[currentScale], 127);
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C3, NOTE_C10);
      set_allPadNote(currentScale, currentInstrument, currentProNatural);
    }
  }
}

/*
 * @brief Makes a read of the PadMode and performs actions if needed. Non Circular
 */
void read_modePad(){
  int padUp = 0;
  int padDown = 0;
  padUp = PadMode.get_padEvent(3);
  padDown = PadMode.get_padEvent(0);
  if(padUp == EVENT_TO_ON){
    if(currentMode < MODE_MAX){
        currentMode++;
        sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageMode[currentMode], 127);
      }
  }else if(padDown == EVENT_TO_ON){
    if(currentMode > MODE_MIN){
        currentMode--;
        sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageMode[currentMode], 127);
      }
  }
}

/*
 * @brief Makes a read of the PadProNatural and performs actions if needed.
 */
void read_proNaturalPad(){
  int padResult = 0;
  padResult = PadProNatural.get_padEvent(0);
  if(padResult == EVENT_TO_ON){
    if(currentProNatural == NATURAL){
      currentProNatural = PRO;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageProNatural[currentProNatural], 127);
      padInitialization(currentProNatural);
    }else if(currentProNatural == PRO){
      currentProNatural = NATURAL;
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageProNatural[currentProNatural], 127);
      padInitialization(currentProNatural);
    }
  }else{
    // Do nothing
  }
}

/*
 * @brief Makes a read of the PadVoiceDescription and performs actions if needed.
 */
void read_voiceDescriptionPad(){
  int padResult = 0;
  padResult = PadVoiceDescription.get_padEvent(0);
  if(padResult == EVENT_TO_ON){
    if(currentVoiceDescription == OFF){
      currentVoiceDescription = ON;
      set_PadVoiceDescription(currentVoiceDescription);
      voiceSituationReport();
    }else if(currentVoiceDescription == ON){
      currentVoiceDescription = OFF;
      set_PadVoiceDescription(currentVoiceDescription);
    }
  }else{
    // Do nothing
  }
}

/*
 * @brief Launches a succesion of MIDI voice command detailling the current situation of the system
 */
void voiceSituationReport(){
   sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageProNatural[currentProNatural], 127);
   delay(1500);
   sendVoiceCommand(CH_CONTROL, NOTE_ON, NOTE_G2, 127);   // "Instrumentos"
   delay(1200);
   sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageInstrument[currentInstrument], 127);
   delay(1500);
   sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageOctaver[currentOctave - OCTAVE_MIN], 127);
   if(currentProNatural == PRO){
      delay(1500);
      sendVoiceCommand(CH_CONTROL, NOTE_ON, NOTE_G2s, 127);   // "Escalas"
      delay(1200);
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageScaler[currentScale], 127);
      delay(1500);
      sendVoiceCommand(CH_CONTROL, NOTE_ON, NOTE_C1, 127);   // "Tono"
      delay(1200);
      sendVoiceCommand(CH_CONTROL, NOTE_ON, controlMessageMode[currentMode], 127);
   }
}

/*
 * @brief Launches a MIDI voice command if currentVoiceDescription is ON
 */
void sendVoiceCommand(byte MIDICHANNEL, byte MESSAGE, byte PITCH, byte VELOCITY){
  if(currentVoiceDescription){
    MIDI.MIDI_TX(MIDICHANNEL, MESSAGE, PITCH, VELOCITY);
  }
}
