#include <Adafruit_NeoPixel.h>

#define PIN         10
#define NUMPIXELS   54
#define DELAY_VAL   50

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.setBrightness(100);
}

void loop() {
  for (int r = 0; r < 3; r++){
    for (int l = 0; l < (4 - r) * 6; l++){
      strip.clear();
      hexagonSetPixel(r, l, strip.Color(255, 100, 0));
      strip.show();
      delay(DELAY_VAL);
    }
  }

}

//zero indexed
void hexagonSetPixel(byte ringnum, byte lednum, uint32_t color){
  uint16_t pixelnum;
  switch (ringnum){
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
