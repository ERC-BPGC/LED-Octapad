#include <CapacitiveSensor.h>
#include <Wire.h>
#include "MIDIUSB.h"

#define midi_channel 0

#define touch_threshold 700

class touchPad {
  public:
    touchPad(int cpin, int tpin, byte npitch) {
      pitch = npitch;
      sensor = new CapacitiveSensor(cpin, tpin); 
      isOn = false;
    }
    CapacitiveSensor *sensor;
    byte pitch;
    bool isOn;
};

touchPad pad1 = touchPad(2, 3, 68);
touchPad pad2 = touchPad(4, 5, 69);
touchPad pad3 = touchPad(6, 7, 70);
touchPad pad4 = touchPad(8, 9, 71);
touchPad pad5 = touchPad(10, 11, 72);
touchPad pad6 = touchPad(12, 13, 73);

void setup() {
  Wire.begin();
}

void loop() {
  checkStatus(pad1);
  checkStatus(pad2);
  checkStatus(pad3);
  checkStatus(pad4);
  checkStatus(pad5);
  checkStatus(pad6);
}

void checkStatus(touchPad& pad) {
  long padValue = pad.sensor->capacitiveSensor(20);
  if (padValue >= touch_threshold && !pad.isOn){
    noteOn(midi_channel, pad.pitch, 99);
    MidiUSB.flush();
    writeToI2C(pad.pitch, true);
  }
  else if (padValue < touch_threshold && pad.isOn){
    noteOff(midi_channel, pad.pitch, 99);
    MidiUSB.flush();
    writeToI2C(pad.pitch, false);
  }
}

void writeToI2C(byte number, bool b){
  Wire.beginTransmission(2);
  Wire.write(number);
  Wire.write(B00000000 | b);
  Wire.endTransmission();
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
