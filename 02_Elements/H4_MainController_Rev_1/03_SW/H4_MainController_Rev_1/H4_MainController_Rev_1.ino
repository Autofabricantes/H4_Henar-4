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
const int OCTAVE_MIN            = 5;
const int OCTAVE_MAX            = 8;
const int INSTRUMENT_MIN        = 0;
const int INSTRUMENT_MAX        = 5;
const int SCALE_MIN             = 0;
const int SCALE_MAX             = 5;
const byte NATURAL_MODE         = 0;
const byte PRO_MODE             = 1;

const byte PadInstrumentColor[6][2] = {{TURQOUISE_HIGH, WHITE_LOW},     // Escala Mayor             I-II-III-IV-V-VI-VII
                            {GREEN_HIGH, WHITE_LOW},                    // Escala Menor Natural     I-II-IIIb-IV-V-VI-VIIb
                            {BLUE_HIGH, WHITE_LOW},                     // Escala Arpegio Maj7      I-III-V-VII-I'-III'-V'
                            {ORANGE_HIGH, WHITE_LOW},                   // Escala Arpegio Men7      I-IIb-V-VIIb-I'-IIIb'-V'
                            {PURPLE_HIGH, WHITE_LOW},                   // Escala Pentatonico Mayor I-II-III-V-VI-I'-II'
                            {ZIEL_HIGH, WHITE_LOW}                      // Escala Pentatonico Menor I-IIIb-IV-VIIb-I'-IIIb'
};

const byte PadOctaverColor[2][2] = {{BLUE_LOW, WHITE_LOW},
                            {BLUE_LOW, WHITE_LOW},
};

const byte PadScalerColor[2][2] = {{YELLOW_LOW, WHITE_LOW},
                                   {YELLOW_LOW, WHITE_LOW}
};

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Custom Configuration Structs----------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Global Variables----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
int inMssg[10] = {0,0,0,0,0,0,0,0,0,0};

H4_PadController PadNotes[qPadNotes] = {H4_PadController(0,30),
                                 H4_PadController(1,37),
                                 H4_PadController(2,29),
                                 H4_PadController(3,27),
                                 H4_PadController(4,36),
                                 H4_PadController(5,26),
                                 H4_PadController(6,35),
                                 H4_PadController(7,25),
                                 H4_PadController(8,24),
                                 H4_PadController(9,33),
                                 H4_PadController(10,23),
                                 H4_PadController(11,32),
                                 H4_PadController(12,22),
                                 H4_PadController(13,31),
                                 H4_PadController(14,21),
                                 H4_PadController(15,20),
                                 H4_PadController(16,34),
                                 H4_PadController(17,38)};
H4_MIDIController MIDI =        H4_MIDIController();

H4_PadController PadOctaver     = H4_PadController(0,40);
H4_PadController PadInstrument  = H4_PadController(0,39);
H4_PadController PadScaler      = H4_PadController(0,41);
H4_PadController PadMode        = H4_PadController(0,42);

int octaveCorrector[6]           = {0, 1, 0, 0, 0, 1};
byte currentInstrument           = INSTRUMENT_MIN;   // Channel is also the Instrument
byte currentScale                = SCALE_MIN;   // Channel is also the Instrument
int currentOctave                = OCTAVE_MIN;   // Starting at value NOTE_C5 60
byte currentMode                 = NATURAL_MODE;

int iterator = 0;

void setup() {
  // i2C Initialization
  Wire.begin();
  Wire.onReceive(recieveRequestResponse); // register event
  Serial.begin(115200);
  delay(2000);
  //Serial.println("ACTIVE");
  padOctaverInitializationSequence();
  padInstrumentInitializationSequence();
  padScalerInitializationSequence();
  padInitializationSequence();

  // Only to Initialice new ones
  //while(1){setNewI2cDirFromDefault(42);};
}

/*
 * LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOP
 */
void loop() {
  read_allPads();
  if(iterator >= 2){
    read_OctaverPad();
    read_instrumentPad();
    read_scalerPad();
    iterator = 0;
  }else{
    iterator++;
  }
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
      //Serial.print(" PAD PRESSED: ");
      //Serial.println(i);
      MIDI.MIDI_TX((byte)PadNotes[i].INSTRUCTION.channelCode, (byte)PadNotes[i].INSTRUCTION.instructionCode_0, (byte)(PadNotes[i].INSTRUCTION.pitchCode + ((currentOctave + octaveCorrector[currentInstrument]) * 12)), (byte)PadNotes[i].INSTRUCTION.velocityCode_0);
    }else if(padResult == EVENT_TO_OFF){
      //Serial.print(" PAD RELEASED: ");
      //Serial.println(i);
      MIDI.MIDI_TX((byte)PadNotes[i].INSTRUCTION.channelCode, (byte)PadNotes[i].INSTRUCTION.instructionCode_1, (byte)(PadNotes[i].INSTRUCTION.pitchCode + ((currentOctave + octaveCorrector[currentInstrument]) * 12)), (byte)PadNotes[i].INSTRUCTION.velocityCode_1);
    }
  }
}

void setColor_allPads(int primaryColor, int secondaryColor){
  int padResult = 0;
  for(int i = 0; i < qPadNotes; i++){
    PadNotes[i].set_padConfiguration(primaryColor,
                                  secondaryColor,
                                  PadNotes[i].CONF.disconnectedColor,
                                  PadNotes[i].CONF.connectedColor,
                                  PadNotes[i].CONF.blinkDuration,
                                  PadNotes[i].CONF.fadeDuration,
                                  PadNotes[i].CONF.CH_On_0,
                                  PadNotes[i].CONF.CH_On_1,
                                  PadNotes[i].CONF.CH_On_2,
                                  PadNotes[i].CONF.CH_On_3,
                                  PadNotes[i].CONF.CH_Thr_0,
                                  PadNotes[i].CONF.CH_Thr_1,
                                  PadNotes[i].CONF.CH_Thr_2,
                                  PadNotes[i].CONF.CH_Thr_3);
     PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, primaryColor, secondaryColor, PadNotes[i].CONF.fadeDuration);
     delay(20);
  }
}

void padInitializationSequence(){
  for(int i = 0; i < qPadNotes; i++){
    PadNotes[i].set_padConfiguration(DEFAULT_primaryColor,
                                  DEFAULT_secondaryColor,
                                  DEFAULT_disconnectedColor,
                                  DEFAULT_connectedColor,
                                  DEFAULT_blinkDuration,
                                  DEFAULT_fadeDuration,
                                  DEFAULT_CH_On_0,
                                  DEFAULT_CH_On_1,
                                  DEFAULT_CH_On_2,
                                  DEFAULT_CH_On_3,
                                  DEFAULT_CH_Thr_0,
                                  DEFAULT_CH_Thr_1,
                                  DEFAULT_CH_Thr_2,
                                  DEFAULT_CH_Thr_3);
     PadNotes[i].set_padConfiguration(DEFAULT_primaryColor,
                                  DEFAULT_secondaryColor,
                                  DEFAULT_disconnectedColor,
                                  DEFAULT_connectedColor,
                                  DEFAULT_blinkDuration,
                                  DEFAULT_fadeDuration,
                                  DEFAULT_CH_On_0,
                                  DEFAULT_CH_On_1,
                                  DEFAULT_CH_On_2,
                                  DEFAULT_CH_On_3,
                                  DEFAULT_CH_Thr_0,
                                  DEFAULT_CH_Thr_1,
                                  DEFAULT_CH_Thr_2,
                                  DEFAULT_CH_Thr_3);
     PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, DEFAULT_secondaryColor, DEFAULT_primaryColor, DEFAULT_fadeDuration);
     PadNotes[i].set_padInstruction(currentInstrument, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
     delay(50);
  }
  for(int i = 0; i < qPadNotes; i++){
    PadNotes[qPadNotes-1-i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, DEFAULT_primaryColor, DEFAULT_secondaryColor, DEFAULT_fadeDuration);
    delay(50);
  }  
}

void padOctaverInitializationSequence(){
  // Initilize Octaver
  PadOctaver.set_padConfiguration(PadOctaverColor[0][0],
                                    PadOctaverColor[0][1],
                                    DEFAULT_disconnectedColor,
                                    DEFAULT_connectedColor,
                                    DEFAULT_blinkDuration,
                                    DEFAULT_fadeDuration,
                                    DEFAULT_CH_On_0,
                                    DEFAULT_CH_On_1,
                                    DEFAULT_CH_On_2,
                                    DEFAULT_CH_On_0,  // Active
                                    DEFAULT_CH_Thr_0 + 10,
                                    DEFAULT_CH_Thr_1,
                                    DEFAULT_CH_Thr_2,
                                    DEFAULT_CH_Thr_3 + 10);
   //PadOctaver.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
   PadOctaver.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadOctaverColor[0][0], PadOctaverColor[0][1], PadOctaver.CONF.fadeDuration);
}

void padInstrumentInitializationSequence(){
  // Initilize Octaver
  PadInstrument.set_padConfiguration(PadInstrumentColor[currentInstrument][0],
                                    PadInstrumentColor[currentInstrument][1],
                                    DEFAULT_disconnectedColor,
                                    DEFAULT_connectedColor,
                                    DEFAULT_blinkDuration,
                                    DEFAULT_fadeDuration,
                                    DEFAULT_CH_On_0,
                                    DEFAULT_CH_On_1,
                                    DEFAULT_CH_On_2,
                                    DEFAULT_CH_On_0,  // Active
                                    DEFAULT_CH_Thr_0,
                                    DEFAULT_CH_Thr_1,
                                    DEFAULT_CH_Thr_2,
                                    DEFAULT_CH_Thr_3);
   //PadInstrument.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
   PadInstrument.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[currentInstrument][0], PadInstrumentColor[currentInstrument][1], PadInstrument.CONF.fadeDuration);
}

void padScalerInitializationSequence(){
  // Initilize Octaver
  PadScaler.set_padConfiguration(PadScalerColor[0][0],
                                    PadScalerColor[0][1],
                                    DEFAULT_disconnectedColor,
                                    DEFAULT_connectedColor,
                                    DEFAULT_blinkDuration,
                                    DEFAULT_fadeDuration,
                                    DEFAULT_CH_On_0,
                                    DEFAULT_CH_On_1,
                                    DEFAULT_CH_On_2,
                                    DEFAULT_CH_On_0,  // Active
                                    DEFAULT_CH_Thr_0 + 10,
                                    DEFAULT_CH_Thr_1,
                                    DEFAULT_CH_Thr_2,
                                    DEFAULT_CH_Thr_3 + 10);
   //PadInstrument.set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
   PadScaler.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadScalerColor[0][0], PadScalerColor[0][1], PadInstrument.CONF.fadeDuration);
}

void setInstrument_AllPads(int instrumentCode, int setScale){
  for(int i = 0; i < qPadNotes; i++){
    PadNotes[i].set_padConfiguration(PadInstrumentColor[instrumentCode][0],
                                  PadInstrumentColor[instrumentCode][1],
                                  DEFAULT_disconnectedColor,
                                  DEFAULT_connectedColor,
                                  DEFAULT_blinkDuration,
                                  DEFAULT_fadeDuration,
                                  DEFAULT_CH_On_0,
                                  DEFAULT_CH_On_1,
                                  DEFAULT_CH_On_2,
                                  DEFAULT_CH_On_3,
                                  DEFAULT_CH_Thr_0,
                                  DEFAULT_CH_Thr_1,
                                  DEFAULT_CH_Thr_2,
                                  DEFAULT_CH_Thr_3);
     PadNotes[i].set_padInstruction(instrumentCode, NOTE_ON, NOTE_OFF, SCALES[currentScale][i], MAX_MIDIVELOCITY, 0);
     PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[instrumentCode][1], PadInstrumentColor[instrumentCode][0], PadNotes[i].CONF.fadeDuration);
     delay(50);
     
  }
  for(int i = 0; i < qPadNotes; i++){
    PadNotes[qPadNotes-1-i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[instrumentCode][0], PadInstrumentColor[instrumentCode][1], PadNotes[i].CONF.fadeDuration);
    delay(50);
  }
  MIDI.MIDIOFF_FIX(currentInstrument, 0, 127);
}

void read_OctaverPad(){
  int padUp = 0;
  int padDown = 0;
  padUp = PadOctaver.get_padEvent(3);
  padDown = PadOctaver.get_padEvent(0);
  if(padUp == EVENT_TO_ON){
    if(currentOctave < OCTAVE_MAX){
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C10);
      currentOctave++;
      // TODO Change PAD Color
      
      // TODO Send MIDI Message
    }
  }else if(padDown == EVENT_TO_ON){
    if(currentOctave > OCTAVE_MIN){
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C10);
      currentOctave--;
      // TODO Change PAD Color
      // TODO Send MIDI Message
    }
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
                              DEFAULT_disconnectedColor,
                              DEFAULT_connectedColor,
                              DEFAULT_blinkDuration,
                              DEFAULT_fadeDuration,
                              DEFAULT_CH_On_0,
                              DEFAULT_CH_On_1,
                              DEFAULT_CH_On_2,
                              DEFAULT_CH_On_0,
                              DEFAULT_CH_Thr_0,
                              DEFAULT_CH_Thr_1,
                              DEFAULT_CH_Thr_2,
                              DEFAULT_CH_Thr_3);
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
                        DEFAULT_disconnectedColor,
                        DEFAULT_connectedColor,
                        DEFAULT_blinkDuration,
                        DEFAULT_fadeDuration,
                        DEFAULT_CH_On_0,
                        DEFAULT_CH_On_1,
                        DEFAULT_CH_On_2,
                        DEFAULT_CH_On_0,
                        DEFAULT_CH_Thr_0,
                        DEFAULT_CH_Thr_1,
                        DEFAULT_CH_Thr_2,
                        DEFAULT_CH_Thr_3);
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
  if(padUp == EVENT_TO_OFF){
    if(currentScale < SCALE_MAX){
      //MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C10);
      currentScale++;
      //delay(1000);
      // TODO Change PAD Color
      //PadScaler.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadScalerColor[0][0], PadScalerColor[0][1], PadScaler.CONF.fadeDuration);
      // TODO Send MIDI Message
      //MIDI.MIDI_TX(0, INSTRUMENT, currentInstrument, 0);
      setInstrument_AllPads(currentInstrument, currentScale);
    }
  }else if(padDown == EVENT_TO_OFF){
    if(currentScale > SCALE_MIN){
      //MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C10);
      currentScale--;
      //delay(1000);
      // TODO Change PAD Color
      //PadScaler.set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadScalerColor[0][1], PadScalerColor[0][0], PadScaler.CONF.fadeDuration);
      // TODO Send MIDI Message
      //MIDI.MIDI_TX(0, INSTRUMENT, currentInstrument, 0);
      setInstrument_AllPads(currentInstrument, currentScale);
    }
  }
}
