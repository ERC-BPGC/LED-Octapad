#include <Adafruit_NeoPixel.h>

// 54 neopixels in each hexagon
#define pixelCount 54
#define delaytime 50
#define brightness 255

// pins for each hexagon's strip
int neoPins[] = {2, 3, 4, 5, 6, 7};
const int numStrips = sizeof(neoPins) / sizeof(int);

Adafruit_NeoPixel strip(pixelCount, neoPins[0], NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(brightness);
}

long lastmillis = 0;
uint16_t hue = 0;
volatile bool stateChanged = false;
volatile byte touch = 0;

void loop() {
  //do a simple hue fade along each hexagon
  if (millis() - lastmillis >= delaytime) {
    lastmillis = millis();

    for (int i = 0; i < numStrips; i++) {
      strip.setPin(neoPins[i]);
      setAll(strip.ColorHSV(hue + 5000 * i));
    }

    hue += 1000;
    Serial.println(hue);
  } 
}
// other animations you can test
void Sparkle() {
  strip.setPixelColor(random(pixelCount), random(255), random(255), random(255));
  strip.show();
}

void setAll(uint32_t color) {
  for (int i = 0; i < pixelCount; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void setAll(byte r, byte g, byte b) {
  for (int i = 0; i < pixelCount; i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

void Fire(int Cooling, int Sparking) {
  static byte heat[pixelCount];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < pixelCount; i++) {
    cooldown = random(0, ((Cooling * 10) / pixelCount) + 2);
    if (cooldown > heat[i]) {
      heat[i] = 0;
    } else {
      heat[i] = heat[i] - cooldown;
    }
  }
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = pixelCount - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if (random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160, 255);
    //heat[y] = random(160,255);
  }
  // Step 4.  Convert heat to LED colors
  for ( int j = 0; j < pixelCount; j++) {
    setPixelHeatColor(j, heat[j] );
  }
  strip.show();
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature / 255.0) * 191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if ( t192 > 0x80) {                    // hottest
    strip.setPixelColor(Pixel, 255, 255, heatramp);
  } else if ( t192 > 0x40 ) {            // middle
    strip.setPixelColor(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    strip.setPixelColor(Pixel, heatramp, 0, 0);
  }
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
