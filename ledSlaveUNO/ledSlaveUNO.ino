#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define pixelCount 54
#define neoPin 10

Adafruit_NeoPixel strip(pixelCount, neoPin, NEO_GRB + NEO_KHZ800);

byte pitch, conditional;

void setup() {
  //i2c device #2
  Wire.begin(2);
  Wire.onReceive(receiveData);

  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(100);
  setAll(255, 255, 0);
}

int dly = 500;
void loop() {
  
//PUT ANIMATION funciton here

}


void setAll(byte r, byte g, byte b) {
  for (int i = 0; i < pixelCount; i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

void receiveData(int howMany) {
  if (Wire.available() >= 2) {
    pitch = Wire.read();
    conditional = Wire.read();
  }
  Serial.print(pitch);
  Serial.print('\t');
  Serial.println(conditional);
}

//zero indexed
void hexagonSetPixel(byte ringnum, byte lednum, uint32_t color) {
  uint16_t pixelnum;
  switch (ringnum) {
    case 0:
      pixelnum = lednum % 24;
      break;
    case 1:
      pixelnum = 24 + lednum % 18;
      break;
    case 2:
      pixelnum = 42 + lednum % 12;
      break;
  }
  strip.setPixelColor(pixelnum, color);
}
