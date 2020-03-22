#include <Wire.h>

#define midi_channel 1
#define usePins true        //set usePins to true if direct pin2pin connection with ledslave
#define printPadValues true

// threshold values for touch sensing
#define touch_threshold_upper 4000
#define touch_threshold_lower 3300
#define debounce_time 80

byte notes[] = {36, 52, 55, 59, 62, 67};    // corresponds to CM9 chord
byte commPins[] = {17, 18, 19, 20, 21, 22}; // pins connected to UNO

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
    // windowing of values and return the max
    long getValue() {
      const int valsize = 10;
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

// initialising pads
touchPad pad1 = touchPad(0, 0);
touchPad pad2 = touchPad(1, 1);
touchPad pad3 = touchPad(3, 2);

touchPad pad4 = touchPad(4, 3);
touchPad pad5 = touchPad(15, 4);
touchPad pad6 = touchPad(16, 5);

void setup() {
  Serial.begin(115200);
  
  for (int i = 0; i < 6; i++){
    pinMode(commPins[i], OUTPUT);
  }
}

void loop() {
  long start = millis();
  checkStatus(pad1);
  checkStatus(pad2);
  checkStatus(pad3);
  checkStatus(pad4);
  checkStatus(pad5);
  checkStatus(pad6);

#if printPadValues
  Serial.print("Time taken = #");
  Serial.println(millis() - start);
#endif
  //  #if printPadValues
  //    Serial.println("");
  //  #endif
  //  delay(1);
}

void checkStatus(touchPad& pad) {
  long padValue = pad.getValue();
  #if printPadValues
    Serial.print(padValue);
    Serial.print('\t');
  #endif

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
  // check if pad has been on for too long and is stuck
  //  if (pad.isOn && pad.lastChange - millis() > 1000){
  //    usbMIDI.sendNoteOff(notes[pad.id], 99, midi_channel);
  //  }
}

//sends ledslave the byte containing the state of each touch sensor
//uses serial3 if useSerial is true, otherwise will send i2c to device #2
void sendSlave(byte id, bool on_off) {
  #if usePins
    digitalWrite(commPins[id], on_off);
  //else use i2c (this was a bitch to get working)
  #else
    static byte sendByte = 0;
    //generate touch byte
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
  #endif
}
