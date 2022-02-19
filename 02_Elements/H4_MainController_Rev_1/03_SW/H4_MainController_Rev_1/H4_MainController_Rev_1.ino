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
const int qPadNotes             = 11;


const byte PadInstrumentColor[6][2] = {{TURQOUISE_HIGH, WHITE_LOW},   // Escala Mayor             I-II-III-IV-V-VI-VII
                            {GREEN_HIGH, WHITE_LOW},   // Escala Menor Natural     I-II-IIIb-IV-V-VI-VIIb
                            {BLUE_HIGH, WHITE_LOW},   // Escala Arpegio Maj7      I-III-V-VII-I'-III'-V'
                            {ORANGE_HIGH, WHITE_LOW},   // Escala Arpegio Men7      I-IIb-V-VIIb-I'-IIIb'-V'
                            {PURPLE_HIGH, WHITE_LOW},   // Escala Pentatonico Mayor I-II-III-V-VI-I'-II'
                            {ZIEL_HIGH, WHITE_LOW}   // Escala Pentatonico Menor I-IIIb-IV-VIIb-I'-IIIb'
};

// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Custom Configuration Structs----------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Global Variables----------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------
int inMssg[10] = {0,0,0,0,0,0,0,0,0,0};

H4_PadController PadNotes[qPadNotes] = {H4_PadController(0,11),
                                 H4_PadController(1,17),
                                 H4_PadController(2,19),
                                 H4_PadController(3,16),
                                 H4_PadController(4,15),
                                 H4_PadController(5,13),
                                 H4_PadController(6,10),
                                 H4_PadController(7,12),
                                 H4_PadController(8,18),
                                 H4_PadController(9,8),
                                 H4_PadController(10,14)};
H4_MIDIController MIDI =        H4_MIDIController();
// This Analog Pads will be removed
H4_AnalogPad PAD_OCTAVER(A0, true, DEFAULT_CH_Thr_0);
H4_AnalogPad PAD_INSTRUMENT(A1, true, DEFAULT_CH_Thr_0);
H4_AnalogPad PAD_SCALER(A2, true, DEFAULT_CH_Thr_0);

 // PAD Flags
char PAD_ACTIVE_OCTAVER = 0;
char PAD_ACTIVE_INSTRUMENT = 0;
char PAD_ACTIVE_SCALER = 0;

int octaveCorrector[6]           = {0, 1, 0, 0, 0, 1};
byte currentInstrument           = 0;   // Channel is also the Instrument
byte currentScale                = 0;   // Channel is also the Instrument
int currentOctave                = 5;   // Starting at value NOTE_C5 60


int iterator = 0;

void setup() {
  // i2C Initialization
  Wire.begin();
  Wire.onReceive(recieveRequestResponse); // register event
  Serial.begin(115200);
  delay(2000);
  //Serial.println("ACTIVE");
  padInitializationSequence();

  PAD_OCTAVER.init();
  PAD_INSTRUMENT.init();
  PAD_SCALER.init();

  for (int i = 0; i < STABILIZATION_ITERATOR; i++) {
    PAD_OCTAVER.readPad();
    PAD_INSTRUMENT.readPad();
    PAD_SCALER.readPad();
  }
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

void setInstrument_AllPads(int instrumentCode, int setScale){
  MIDI.MIDIOFF_FIX(currentInstrument, 0, 127);
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
}

void read_OctaverPad(){
  PAD_OCTAVER.readPad();
  if ((!PAD_ACTIVE_OCTAVER) && PAD_OCTAVER.get_isPadActivated()) {
      PAD_ACTIVE_OCTAVER = 1;
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C8);
      if(currentOctave == 5){
        currentOctave = 4;
      }else{
        currentOctave = 5;
      }
      for(int i = 0; i < qPadNotes; i++){
        PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.fadeDuration);
      }
  }
  if ((PAD_ACTIVE_OCTAVER) && !PAD_OCTAVER.get_isPadActivated()) {
      PAD_ACTIVE_OCTAVER = 0;
      for(int i = 0; i < qPadNotes; i++){
        PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.fadeDuration);
      }
  }
}

void read_instrumentPad(){
  PAD_INSTRUMENT.readPad();
  if ((!PAD_ACTIVE_INSTRUMENT) && PAD_INSTRUMENT.get_isPadActivated()) {
      PAD_ACTIVE_INSTRUMENT = 1;
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C8);
      if(currentInstrument < 5){
        currentInstrument++;
      }else{
        currentInstrument = 0;
      }
      MIDI.MIDI_TX(0, INSTRUMENT, currentInstrument, 0);
      setInstrument_AllPads(currentInstrument, currentScale);
      /*for(int i = 0; i < qPadNotes; i++){
        PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.fadeDuration);
      }*/
  }
  if ((PAD_ACTIVE_INSTRUMENT) && !PAD_INSTRUMENT.get_isPadActivated()) {
      PAD_ACTIVE_INSTRUMENT = 0;
      //Nothing to do
  }
}

void read_scalerPad(){
  PAD_SCALER.readPad();
  if ((!PAD_ACTIVE_SCALER) && PAD_SCALER.get_isPadActivated()) {
      PAD_ACTIVE_SCALER = 1;
      MIDI.MIDIOFF_FIX(currentInstrument, NOTE_C4, NOTE_C8);
      if(currentScale < 5){
        currentScale++;
      }else{
        currentScale = 0;
      }
      for(int i = 0; i < currentScale+1; i++){
         PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[currentInstrument][1], WHITE_BRIGHT, PadNotes[i].CONF.fadeDuration);
         delay(50);
      }
      for(int i = currentScale+1; i < qPadNotes; i++){
        PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadInstrumentColor[currentInstrument][0], BLACK_OFF, PadNotes[i].CONF.fadeDuration);
        delay(50);
      }
      delay(1000);
      setInstrument_AllPads(currentInstrument, currentScale);
      /*for(int i = 0; i < qPadNotes; i++){
        PadNotes[i].set_padLedActivity(LED_ID_FADEFROMNUCLEO, PadNotes[i].CONF.secondaryColor, PadNotes[i].CONF.primaryColor, PadNotes[i].CONF.fadeDuration);
      }*/
  }
  if ((PAD_ACTIVE_SCALER) && !PAD_SCALER.get_isPadActivated()) {
      PAD_ACTIVE_SCALER = 0;
      //Nothing to do
  }
}
