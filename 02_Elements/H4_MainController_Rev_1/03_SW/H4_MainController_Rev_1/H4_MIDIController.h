/*
 * @author Pablo dMM (Pablodmm.isp@gmail.com)
 * @brief  Part of the Henar#4 Project. https://github.com/Autofabricantes/Henar-4 .
 * @date   2021_10_31
 */
#ifndef H4_MIDICONTROLLER_H
#define H4_MIDICONTROLLER_H

#include "Arduino.h"
//#include "H4_WireController.h"

const int NOTEDURATION          = 200;
const int nPADNOTE              = 18;
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
const byte NONOTE   =127;
// -----------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------Musical Parameters--------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------  
/*const byte SCALEPRO[6][nPADNOTE] = {{NOTE_C0, NOTE_D0, NOTE_E0, NOTE_F0, NOTE_G0, NOTE_A0, NOTE_B0, NOTE_C1, NOTE_D1, NOTE_E1},   // Escala Mayor             I-II-III-IV-V-VI-VII
                            {NOTE_C0, NOTE_D0, NOTE_D0s, NOTE_F0, NOTE_G0, NOTE_A0, NOTE_A0s, NOTE_C1, NOTE_D1, NOTE_D1s},   // Escala Menor Natural     I-II-IIIb-IV-V-VI-VIIb
                            {NOTE_C0, NOTE_E0, NOTE_G0, NOTE_B0, NOTE_C1, NOTE_E1, NOTE_G1, NOTE_B1, NOTE_C2, NOTE_E2},   // Escala Arpegio Maj7      I-III-V-VII-I'-III'-V'
                            {NOTE_C0, NOTE_D0s, NOTE_G0, NOTE_A0s, NOTE_C1, NOTE_D1s, NOTE_G1, NOTE_A1s, NOTE_C2, NOTE_D2s},   // Escala Arpegio Men7      I-IIb-V-VIIb-I'-IIIb'-V'
                            {NOTE_C0, NOTE_D0, NOTE_E0, NOTE_G0, NOTE_A0, NOTE_C1, NOTE_D1, NOTE_E1, NOTE_G1, NOTE_A1},   // Escala Pentatonico Mayor I-II-III-V-VI-I'-II'
                            {NOTE_C0, NOTE_D0s, NOTE_F0, NOTE_G0, NOTE_A0s, NOTE_C1, NOTE_D1s, NOTE_F1, NOTE_G1, NOTE_A1s}   // Escala Pentatonico Menor I-IIIb-IV-VIIb-I'-IIIb'
}; */

const byte NATURAALSCALE       = 0;

const byte SCALES[7][nPADNOTE] = {{NOTE_A0, NOTE_A0s, NOTE_B0, NOTE_C1, NOTE_C1s, NOTE_D1, NOTE_D1s, NOTE_E1, NOTE_F1, NOTE_F1s, NOTE_G1, NOTE_G1s, NOTE_A1, NOTE_A1s, NOTE_B1, NOTE_C2, NOTE_C2s, NOTE_D2},   // Modo Natural
                                    {NOTE_C0, NONOTE, NOTE_D0, NOTE_E0, NONOTE, NOTE_F0, NONOTE, NOTE_G0, NOTE_A0, NONOTE, NOTE_B0, NONOTE, NOTE_C1, NONOTE, NOTE_D1, NOTE_E1, NONOTE, NOTE_F1},   // Escala Mayor             I-II-III-IV-V-VI-VII
                                    {NOTE_C0, NONOTE, NOTE_D0, NOTE_D0s, NONOTE, NOTE_F0, NONOTE, NOTE_G0, NOTE_A0, NONOTE, NOTE_A0s, NONOTE, NOTE_C1, NONOTE, NOTE_D1, NOTE_D1s, NONOTE, NOTE_F1},   // Escala Menor Natural     I-II-IIIb-IV-V-VI-VIIb
                                    {NOTE_C0, NONOTE, NOTE_E0, NOTE_G0, NONOTE, NOTE_B0, NONOTE, NOTE_C1, NOTE_E1, NONOTE, NOTE_G1, NONOTE, NOTE_B1, NONOTE, NOTE_C2, NOTE_E2, NONOTE, NOTE_G2},   // Escala Arpegio Maj7      I-III-V-VII-I'-III'-V'
                                    {NOTE_C0, NONOTE, NOTE_D0s, NOTE_G0, NONOTE, NOTE_A0s, NONOTE, NOTE_C1, NOTE_D1s, NONOTE, NOTE_G1, NONOTE, NOTE_A1s, NONOTE, NOTE_C2, NOTE_D2s, NONOTE, NOTE_G2},   // Escala Arpegio Men7      I-IIb-V-VIIb-I'-IIIb'-V'
                                    {NOTE_C0, NONOTE, NOTE_D0, NOTE_E0, NONOTE, NOTE_G0, NONOTE, NOTE_A0, NOTE_C1, NONOTE, NOTE_D1, NONOTE, NOTE_E1, NONOTE, NOTE_G1, NOTE_A1, NONOTE, NOTE_C2},   // Escala Pentatonico Mayor I-II-III-V-VI-I'-II'
                                    {NOTE_C0, NONOTE, NOTE_D0s, NOTE_F0, NONOTE, NOTE_G0, NONOTE, NOTE_A0s, NOTE_C1, NONOTE, NOTE_D1s, NONOTE, NOTE_F1, NONOTE, NOTE_G1, NOTE_A1s, NONOTE, NOTE_C2}   // Escala Pentatonico Menor I-IIIb-IV-VIIb-I'-IIIb'
}; 

//extern H4_WireController WireController;


class H4_MIDIController
{
  public:
    H4_MIDIController();
    void MIDI_TX(byte MIDICHANNEL, byte MESSAGE, byte PITCH, byte VELOCITY);
    void MIDIOFF_FIX(byte CHANNEL, int INIT_NOTE, int END_NOTE);// This is a very rude and nasty sustitution to a General Note Off CMD
  private:

};

#endif
