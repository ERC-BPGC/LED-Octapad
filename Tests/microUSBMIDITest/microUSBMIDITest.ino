#include "MIDIUSB.h"

#define midi_channel 0
#define note_duration 50

void setup() {

}

void loop() {
  noteOn(midi_channel, 48, 64);
  MidiUSB.flush();
  delay(note_duration);
  noteOff(midi_channel, 48, 64);
  MidiUSB.flush();
  delay(note_duration / 4);
  
  noteOn(midi_channel, 52, 64);
  MidiUSB.flush();
  delay(note_duration);
  noteOff(midi_channel, 52, 64);
  MidiUSB.flush();
  delay(note_duration / 4);
  
  noteOn(midi_channel, 55, 64);
  MidiUSB.flush();
  delay(note_duration);
  noteOff(midi_channel, 55, 64);
  MidiUSB.flush();
  delay(note_duration / 4);
}

//duration in ms
void playNote(byte pitch, int duration){
  noteOn(midi_channel, pitch, 99);
  delay(duration);
  noteOff(midi_channel, pitch, 99);  
}

//pitch is note number (middle C is 48)
//velocity ranges from 0 to 127
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
