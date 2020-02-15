#include <Wire.h>

#define midi_channel 1

#define touch_threshold_upper 7000
#define touch_threshold_lower 4000
#define debounce_time 100

class touchPad {
  public:
    int touchPin;
    byte id;
    bool isOn;
    long lastChange;

    touchPad(int tpin, byte nid) {
      touchPin = tpin;
      id = nid;
      isOn = false;
      lastChange = 0;
    }
    long getValue() {
      const int valsize = 50;
      long values[valsize];
      long maxval = 0;

      for (int i = 0; i < valsize; i++) {
        values[i] = touchRead(touchPin);
        if (values[i] > maxval) {
          maxval = values[i];
        }
      }
      return maxval;
    }
};

touchPad pad1 = touchPad(0, 0);
touchPad pad2 = touchPad(1, 1);
touchPad pad3 = touchPad(3, 2);

touchPad pad4 = touchPad(4, 3);
touchPad pad5 = touchPad(15, 4);
touchPad pad6 = touchPad(16, 5);

byte notes[] = {24, 40, 43, 47, 50, 55};

void setup() {
  Serial.begin(115200);
//  Serial3.begin(115200);
  Wire.begin();
  
}


void loop() {
  checkStatus(pad1);
  checkStatus(pad2);
  checkStatus(pad3);
  checkStatus(pad4);
  checkStatus(pad5);
  checkStatus(pad6);
  
  Serial.println("");
  delay(1);
}

void checkStatus(touchPad& pad) {
  long padValue = pad.getValue();
  Serial.print(padValue);
  Serial.print('\t');

  //include debounce condition
  if (padValue >= touch_threshold_upper && !pad.isOn
      && millis() - pad.lastChange > debounce_time) {
    pad.isOn = true;
    pad.lastChange = millis();

    Serial.print("pad on: ");
    Serial.println(pad.id);

    usbMIDI.sendNoteOn(notes[pad.id], 99, midi_channel);
    sendSlave(pad.id, true);
  }
  //off condition
  else if (padValue < touch_threshold_lower && pad.isOn
           && millis() - pad.lastChange > debounce_time) {
    pad.isOn = false;
    pad.lastChange = millis();

    Serial.print("pad off: ");
    Serial.println(pad.id);

    usbMIDI.sendNoteOff(notes[pad.id], 99, midi_channel);
    sendSlave(pad.id, false);
  }
  //  if (pad.isOn && pad.lastChange - millis() > 1000){
  //    usbMIDI.sendNoteOff(notes[pad.id], 99, midi_channel);
  //  }
}

//sends ledslave the byte containing the state of each touch sensor
void sendSlave(byte id, bool on_off) {
  static byte sendByte = 0;
  if (on_off) {
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
  
  Serial3.println(sendByte);
}
