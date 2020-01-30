#include <Wire.h>

void setup() {
  Wire.begin(2);
  Wire.onReceive(receiveData);
  Serial.begin(115200);
}

void loop() {

  
}
byte pitch, conditional;

void receiveData(int howMany){
  if (Wire.available() >= 2){
    pitch = Wire.read();
    conditional = Wire.read();
  }
}
