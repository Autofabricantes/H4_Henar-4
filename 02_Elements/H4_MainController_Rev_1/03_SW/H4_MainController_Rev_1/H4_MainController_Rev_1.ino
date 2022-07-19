/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .This code controls 8 padNote, 1 padInstrument and 1 padOctave
 * @date   2020_10_27
 */

// Library Import
#include <Wire.h>
#include "H4_PadController.h"
#include "H4_MIDIController.h"
#include "H4_AnalogPad.h"
// Modes
#define PAD_TESTING

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------System Pin Definition-----------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Control and Configuration values Definition-------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
// Constant Value Definition
const bool OFF                  = 0;
const bool ON                   = 1;
const int qPadNotes             = 18;
const int OCTAVE_MIN            = 4;
const int OCTAVE_MAX            = 5;
const int INSTRUMENT_MIN        = 0;
const int INSTRUMENT_MAX        = 5;
const int SCALE_MIN             = 0;
const int SCALE_MAX             = 5;
const byte NATURAL              = 0;
const byte PRO                  = 1;
const byte MODE_MIN             = 0;
const byte MODE_MAX             = 11;

const byte PadInstrumentColor[6][2] = {{TURQOUISE_HIGH, WHITE_LOW},     // Escala Mayor             I-II-III-IV-V-VI-VII
                            {GREEN_HIGH, WHITE_LOW},                    // Escala Menor Natural     I-II-IIIb-IV-V-VI-VIIb
                            {BLUE_HIGH, WHITE_LOW},                     // Escala Arpegio Maj7      I-III-V-VII-I'-III'-V'
                            {ORANGE_HIGH, WHITE_LOW},                   // Escala Arpegio Men7      I-IIb-V-VIIb-I'-IIIb'-V'
                            {PURPLE_HIGH, WHITE_LOW},                   // Escala Pentatonico Mayor I-II-III-V-VI-I'-II'
                            {ZIEL_HIGH, WHITE_LOW}                      // Escala Pentatonico Menor I-IIIb-IV-VIIb-I'-IIIb'
};

const byte PadOctaverColor[2][2] = {{BLUE_LOW, WHITE_LOW},
                            {BLUE_HIGH, WHITE_LOW}};

const byte PadProNaturalColor[2][2] = {{GREEN_LOW, WHITE_LOW},
                                       {BLACK_OFF, BLACK_OFF},
};

const byte PadScalerColor[2][2] = {{YELLOW_LOW, WHITE_LOW},
                                   {BLACK_OFF, BLACK_OFF}
};

const byte PadModeColor[2][2] = {{RED_LOW, WHITE_LOW},
                                   {BLACK_OFF, BLACK_OFF}
};

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Custom Configuration Structs----------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Global Variables----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
int inMssg[10] = {0,0,0,0,0,0,0,0,0,0};

int PadProActivation[2][18] = {{1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,1,0,1},
                              {1,0,1,1,0,1,0,1,1,0,1,0,1,0,1,1,0,1}};

H4_PadController PadNotes[qPadNotes] = {H4_PadController(0,14),
                                 H4_PadController(1,37),
                                 H4_PadController(2,29),
                                 H4_PadController(3,27),
                                 H4_PadController(4,36),
                                 H4_PadController(5,26),
                                 H4_PadController(6,35),
                                 H4_PadController(7,25),
                                 H4_PadController(8,24),
                                 H4_PadController(9,30),
                                 H4_PadController(10,23),
                                 H4_PadController(11,32),
                                 H4_PadController(12,22),
                                 H4_PadController(13,31),
                                 H4_PadController(14,21),
                                 H4_PadController(15,20),
                                 H4_PadController(16,34),
                                 H4_PadController(17,8)};
H4_PadController PadOctaver     = H4_PadController(0,18);
H4_PadController PadInstrument  = H4_PadController(0,39);
H4_PadController PadScaler      = H4_PadController(0,41);
H4_PadController PadMode        = H4_PadController(0,40);
H4_PadController PadProNatural  = H4_PadController(0,12);

H4_MIDIController MIDI =        H4_MIDIController();

int octaveCorrector[6]           = {0, 1, 0, 0, 0, 1};
byte currentInstrument           = INSTRUMENT_MIN;   // Channel is also the Instrument
byte currentScale                = SCALE_MIN;   // Channel is also the Instrument
int currentOctave                = OCTAVE_MIN;   // Starting at value NOTE_C5 60
byte currentMode                 = MODE_MIN;
byte currentProNatural           = NATURAL;

int iterator = 0;

void setup() {
  // i2C Initialization
  Wire.begin();
  Wire.onReceive(recieveRequestResponse); // register event
  Serial.begin(115200);
  delay(2000);
  padInitialization();

  // Only to Initialice new ones
  //while(1){setNewI2cDirFromDefault(42);};
}

/*
 * LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
 */
void loop() {
  read_allPads();
  read_octaverPad();
  read_instrumentPad();
  read_modePad();
  read_scalerPad();
  read_proNaturalPad();
}

void padInitialization(){
  padInstrumentInitializationSequence();
  padModeInitializationSequence();
  padScalerInitializationSequence();
  padProNaturalInitializationSequence();
  padOctaverInitializationSequence();
  padInitializationSequence();
}

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

void setNewI2cDirFromDefault(int newDir){
  change_i2cDir(8, newDir);
}

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

void read_allPads(){
  int padResult = 0;
  for(int i = 0; i < qPadNotes; i++){
      padResult = PadNotes[i].get_padEvent(0);
      if(padResult == EVENT_TO_ON){
        MIDI.MIDI_TX((byte)PadNotes[i].INSTRUCTION.channelCode, (byte)PadNotes[i].INSTRUCTION.instructionCode_0, (byte)(PadNotes[i].INSTRUCTION.pitchCode + ((currentOctave + octaveCorrector[currentInstrument]) * 12)), (byte)PadNotes[i].INSTRUCTION.velocityCode_0);
      }else if(padResult == EVENT_TO_OFF){
        MIDI.MIDI_TX((byte)PadNotes[i].INSTRUCTION.channelCode, (byte)PadNotes[i].INSTRUCTION.instructionCode_1, (byte)(PadNotes[i].INSTRUCTION.pitchCode + ((currentOctave + octaveCorrector[currentInstrument]) * 12)), (byte)PadNotes[i].INSTRUCTION.velocityCode_1);
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
      if(PadProActivation[proNaturalMode][i]){
        // Not active Pad
        primaryColorToSet = OFFCOLOR;
        secondaryColorToSet = OFFCOLOR;
      }
    }
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
 * @brief Makes a color swipe of all the PadNotes from beginning to end
 */
void swipeColor_allPads(){
  // Turn them all to ON
  for(int i = 0; i < qPadNotes; i++){
    PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.fadeDuration);
    delay(50);
  }
  // Turn them all to OFF
    for(int i = 0; i < qPadNotes; i++){
    PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.fadeDuration);
    delay(50);
  }
}

void individualSwipeColor_SinglePad(H4_PadController *padToPerform){
   padToPerform->set_padLedActivity(LED_ID_FADEFROMNUCLEO, padToPerform->CONF.primaryColor, padToPerform->CONF.secondaryColor, padToPerform->CONF.fadeDuration);
   delay(50);
   padToPerform->set_padLedActivity(LED_ID_FADEFROMNUCLEO, padToPerform->CONF.secondaryColor, padToPerform->CONF.primaryColor, padToPerform->CONF.fadeDuration);
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

void padInitializationSequence(){
  set_allPadNote(currentScale, currentInstrument, currentProNatural);
}

void padOctaverInitializationSequence(){
  // Initilize Octaver
  PadOctaver.set_padConfiguration(PadOctaverColor[0][0],
                                    PadOctaverColor[0][1],
                                    DISCONNECTEDCOLOR,
                                    CONNECTEDCOLOR,
                                    BLINKDURATION,
                                    FADEDURATION,
                                    CH_ON,CH_OFF,CH_OFF,CH_OFF,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
   //PadOctaver.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
   individualSwipeColor_SinglePad(&PadOctaver);
   /*PadOctaver.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadOctaver.CONF.primaryColor, PadOctaver.CONF.secondaryColor, PadOctaver.CONF.fadeDuration);
   delay(50);
   PadOctaver.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadOctaver.CONF.secondaryColor, PadOctaver.CONF.primaryColor, PadOctaver.CONF.fadeDuration);*/
}

void padInstrumentInitializationSequence(){
  // Initilize Octaver
  PadInstrument.set_padConfiguration(PadInstrumentColor[currentInstrument][0],
                                    PadInstrumentColor[currentInstrument][1],
                                    DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                    BLINKDURATION,FADEDURATION,
                                    CH_ON,CH_OFF,CH_OFF,CH_ON,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
   //PadInstrument.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
   PadInstrument.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[currentInstrument][0], PadInstrumentColor[currentInstrument][1], PadInstrument.CONF.fadeDuration);
   delay(50);
}

void padScalerInitializationSequence(){
  if(currentProNatural == PRO){
    // Initilize Scaler for Pro MODE:
    PadScaler.set_padConfiguration(PadScalerColor[0][0],
                                      PadScalerColor[0][1],
                                      DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                      BLINKDURATION,FADEDURATION,
                                      CH_ON,CH_OFF,CH_OFF,CH_ON,
                                      CH_THR,CH_THR,CH_THR,CH_THR);
     //PadInstrument.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
     PadScaler.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadScalerColor[0][0], PadScalerColor[0][1], PadInstrument.CONF.fadeDuration);
     delay(50);
  }else{
     PadScaler.set_padConfiguration(PadScalerColor[1][0],
                                      PadScalerColor[1][1],
                                      DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                      BLINKDURATION,FADEDURATION,
                                      CH_OFF,CH_OFF,CH_OFF,CH_OFF,
                                      CH_THR,CH_THR,CH_THR,CH_THR);
     //PadInstrument.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
     PadScaler.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadScalerColor[1][0], PadScalerColor[1][1], PadInstrument.CONF.fadeDuration);
  }  
}

void padModeInitializationSequence(){
  if(currentProNatural == PRO){
    // Initilize Scaler
    PadMode.set_padConfiguration(PadModeColor[0][0],
                                      PadModeColor[0][1],
                                      DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                      BLINKDURATION,FADEDURATION,
                                      CH_ON,CH_OFF,CH_OFF,
                                      CH_ON,  // Active
                                      CH_THR,CH_THR,CH_THR,CH_THR);
     //PadInstrument.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
     PadMode.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadModeColor[0][0], PadModeColor[0][1], PadInstrument.CONF.fadeDuration);
     delay(50);
  }else{
    // Initilize Scaler
    PadMode.set_padConfiguration(PadModeColor[1][0],
                                      PadModeColor[1][1],
                                      DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                      BLINKDURATION,FADEDURATION,
                                      0,  // Deactivated
                                      CH_OFF,CH_OFF,
                                      0,  // Deactivated
                                      CH_THR,CH_THR,CH_THR,CH_THR);
     //PadInstrument.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
     PadMode.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadModeColor[1][0], PadModeColor[1][1], PadInstrument.CONF.fadeDuration);
  }
}

void padProNaturalInitializationSequence(){
  // Initilize Octaver
  PadProNatural.set_padConfiguration(PadProNaturalColor[0][0],
                                    PadProNaturalColor[0][1],
                                    DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                                    BLINKDURATION,FADEDURATION,
                                    CH_ON,CH_OFF,CH_OFF,CH_OFF,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
   //PadInstrument.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
   PadProNatural.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadProNaturalColor[0][0], PadProNaturalColor[0][1], PadInstrument.CONF.fadeDuration);
   delay(50);
}

void setInstrument_AllPads(int instrumentCode, int setScale){
  for(int i = 0; i < qPadNotes; i++){
    if(currentProNatural == NATURAL){
      PadNotes[i].set_padConfiguration(PadInstrumentColor[instrumentCode][0],
                                    PadInstrumentColor[instrumentCode][1],
                                    DISCONNECTEDCOLOR,CONNECTEDCOLOR,BLINKDURATION,FADEDURATION,
                                    CH_ON,CH_OFF,CH_OFF,CH_OFF,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
       PadNotes[i].set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
       PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[instrumentCode][1], PadInstrumentColor[instrumentCode][0], PadNotes[i].CONF.fadeDuration);
       delay(50); 
    }else{
      if(PadProActivation[i]){
         PadNotes[i].set_padConfiguration(PadInstrumentColor[instrumentCode][0],
                                    PadInstrumentColor[instrumentCode][1],
                                    DISCONNECTEDCOLOR,CONNECTEDCOLOR,BLINKDURATION,FADEDURATION,
                                    CH_ON,CH_OFF,CH_OFF,CH_OFF,
                                    CH_THR,CH_THR,CH_THR,CH_THR);
         PadNotes[i].set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
         PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[instrumentCode][1], PadInstrumentColor[instrumentCode][0], PadNotes[i].CONF.fadeDuration);
         delay(50); 
      }
    }
  }
  for(int i = 0; i < qPadNotes; i++){
    if(currentProNatural == NATURAL){
      PadNotes[qPadNotes-1-i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[instrumentCode][0], PadInstrumentColor[instrumentCode][1], PadNotes[i].CONF.fadeDuration);
      delay(50);
    }else{
      if(PadProActivation[i]){
        PadNotes[qPadNotes-1-i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[instrumentCode][0], PadInstrumentColor[instrumentCode][1], PadNotes[i].CONF.fadeDuration);
        delay(50);
      }
    }
  }
  MIDI.MIDIOFF_FIX(currentInstrument, 0, 127);
}

void read_octaverPad(){
  int padResult = 0;
  padResult = PadOctaver.get_padEvent(0);
  if(padResult == EVENT_TO_ON){
    if(currentOctave == OCTAVE_MIN){
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C10);
      currentOctave = OCTAVE_MAX;
      // TODO Change PAD Color
      
      // TODO Send MIDI Message
    }else if(currentOctave == OCTAVE_MAX){
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C10);
      currentOctave = OCTAVE_MIN;
    }
  }else{
    // Do nothing
  }
}

void read_instrumentPad(){
  int padUp = 0;
  int padDown = 0;
  padUp = PadInstrument.get_padEvent(3);
  padDown = PadInstrument.get_padEvent(0);
  if(padUp == EVENT_TO_ON){
    if(currentInstrument < INSTRUMENT_MAX){
      //MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C10);
      currentInstrument++;
      //delay(1000);
      // TODO Change PAD Color
      PadInstrument.set_padConfiguration(PadInstrumentColor[currentInstrument][0],
                              PadInstrumentColor[currentInstrument][1],
                              DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                              BLINKDURATION,FADEDURATION,
                              CH_ON,CH_OFF,CH_OFF,CH_ON,
                              CH_THR,CH_THR,CH_THR,CH_THR);
      PadInstrument.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[currentInstrument][0], PadInstrumentColor[currentInstrument][1], PadInstrument.CONF.fadeDuration);
      // TODO Send MIDI Message
      //MIDI.MIDI_TX(0, INSTRUMENT, currentInstrument, 0);
      setInstrument_AllPads(currentInstrument, currentScale);
    }
  }
  if(padDown == EVENT_TO_ON){
    if(currentInstrument > INSTRUMENT_MIN){
      //MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C10);
      currentInstrument--;
      //delay(1000);
      // TODO Change PAD Color
      PadInstrument.set_padConfiguration(PadInstrumentColor[currentInstrument][0],
                        PadInstrumentColor[currentInstrument][1],
                        DISCONNECTEDCOLOR,CONNECTEDCOLOR,
                        BLINKDURATION,FADEDURATION,
                        CH_ON,CH_OFF,CH_OFF,CH_ON,
                        CH_THR,CH_THR,CH_THR,CH_THR);
      PadInstrument.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[currentInstrument][0], PadInstrumentColor[currentInstrument][1], PadInstrument.CONF.fadeDuration);
      // TODO Send MIDI Message
      //MIDI.MIDI_TX(0, INSTRUMENT, currentInstrument, 0);
      setInstrument_AllPads(currentInstrument, currentScale);
    }
  }
}

void read_scalerPad(){
  int padUp = 0;
  int padDown = 0;
  padUp = PadScaler.get_padEvent(3);
  padDown = PadScaler.get_padEvent(0);
  if(padUp == EVENT_TO_ON){
    if(currentScale < SCALE_MAX){
      currentScale++;
      setInstrument_AllPads(currentInstrument, currentScale);
    }
  }else if(padDown == EVENT_TO_ON){
    if(currentScale > SCALE_MIN){
      currentScale--;
      setInstrument_AllPads(currentInstrument, currentScale);
    }
  }
}

void read_modePad(){
  int padUp = 0;
  int padDown = 0;
  padUp = PadMode.get_padEvent(3);
  padDown = PadMode.get_padEvent(0);
  if(padUp == EVENT_TO_ON){
    if(currentMode < MODE_MAX){
      currentMode++;
      }
  }else if(padDown == EVENT_TO_ON){
    if(currentMode > MODE_MIN){
      currentMode--;
      }
  }
}

void read_proNaturalPad(){
  int padResult = 0;
  padResult = PadProNatural.get_padEvent(0);
  if(padResult == EVENT_TO_ON){
    if(currentProNatural == NATURAL){
      currentProNatural = PRO;
      padInitialization();
    }else if(currentProNatural == PRO){
      currentProNatural = NATURAL;
      padInitialization();
    }
  }else{
    // Do nothing
  }
}
