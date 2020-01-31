#include <CapacitiveSensor.h>
#include <Wire.h>
#include "MIDIUSB.h"

#define midi_channel 0

#define touch_threshold_upper 1000
#define touch_threshold_lower 300
#define debounce_time 30

class touchPad {
  public:
    touchPad(int cpin, int tpin, byte nid) {
      id = nid;
      sensor = new CapacitiveSensor(cpin, tpin); 
      isOn = false;
    }
    CapacitiveSensor *sensor;
    byte id;    
    bool isOn;
    long lastChange;
};

touchPad pad1 = touchPad(4, 5, 0);
touchPad pad2 = touchPad(6, 7, 1);
touchPad pad3 = touchPad(8, 9, 2);

touchPad pad4 = touchPad(10, 16, 3);
touchPad pad5 = touchPad(14, 15, 4);
touchPad pad6 = touchPad(A0, A1, 5);

byte notes[] = {69, 71, 73, 74, 76, 78};

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pad1.sensor->set_CS_AutocaL_Millis(0xFFFFFFFF);
  pad2.sensor->set_CS_AutocaL_Millis(0xFFFFFFFF);
  pad3.sensor->set_CS_AutocaL_Millis(0xFFFFFFFF);
  pad4.sensor->set_CS_AutocaL_Millis(0xFFFFFFFF);
  pad5.sensor->set_CS_AutocaL_Millis(0xFFFFFFFF);
  pad6.sensor->set_CS_AutocaL_Millis(0xFFFFFFFF);
}

void loop() {
  checkStatus(pad1);
  checkStatus(pad2);
  checkStatus(pad3);
  checkStatus(pad4);
  checkStatus(pad5);
  checkStatus(pad6);
  delay(10);
}

void checkStatus(touchPad& pad) {
  long padValue = pad.sensor->capacitiveSensor(20);
//  Serial.println(padValue);

  if (padValue >= touch_threshold_upper && !pad.isOn
      && millis() - pad.lastChange > debounce_time){
    pad.isOn = true;
    pad.lastChange = millis();
    
    Serial.print("pad on: ");
    Serial.println(pad.id);
    
    noteOn(midi_channel, notes[pad.id], 99);
    MidiUSB.flush();

    sendSlave(pad.id, true);
  }
  else if (padValue < touch_threshold_lower && pad.isOn
           && millis() - pad.lastChange > debounce_time){
    pad.isOn = false;
    pad.lastChange = millis();
    
    Serial.print("pad off: ");
    Serial.println(pad.id);
    
    noteOff(midi_channel, notes[pad.id], 99);
    MidiUSB.flush();
    sendSlave(pad.id, false);
  }
}

void sendSlave(byte id, bool on_off){
  static byte sendByte = 0;
  if (on_off){
    sendByte |= 1 << id;
  }
  else {
    sendByte ^= 1 << id;
  }
  Serial.print("sendbyte: ");
  Serial.println(sendByte, BIN);

  Wire.beginTransmission(2);
  Wire.write(sendByte);
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
