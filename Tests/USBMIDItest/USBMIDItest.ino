//plays a dumb tune to usb MIDI
//tested on the teensy LC. WILL NOT work on arduino uno

const int channel = 1;

int notes[] = {60, 60, 67, 67, 69, 69, 67, 67, 65, 65, 64, 64, 62, 62, 60, 72};

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < sizeof(notes)/sizeof(int); i++){
    playMIDINote(notes[i], 300);
    delay(100);
  }
}

void playMIDINote(int note, int duration_ms){
  usbMIDI.sendNoteOn(note, 99, channel);
  delay(duration_ms);
  usbMIDI.sendNoteOff(note, 99, channel);
  Serial.print("Playing note ");
  Serial.println(note);
}
