#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define pixelCount 54     // 54 leds per hexagon
#define brightness 255    // reduce to cut power consumption

//set this to true if using serial to communicate with teensy
#define serialDebug true
#define indicatorLED 10

// pin numbers for each hexagon ka strip
int neoPins[] = {2, 3, 4, 5, 6, 7};
const int numStrips = sizeof(neoPins) / sizeof(int);

// commpins are like a direct bus from the touch uC
byte commPins[] = {16, 14, 15, 18, 19, 20};
bool commStates[6] = {0}, prevCommStates[6] = {0};

Adafruit_NeoPixel strip(pixelCount, neoPins[0], NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

  pinMode(indicatorLED, OUTPUT);
  indicateLED();

  for (int i = 0; i < 6; i++) {
    pinMode(commPins[i], INPUT);
  }

  strip.begin();
  strip.setBrightness(brightness);
}

long lastmillis = 0;
uint16_t hue = 0;
volatile bool stateChanged = false;
volatile byte touch = 0;

void loop() {
  // serial.print values if debug is true
  #if serialDebug
    for (int i = 0; i < 6; i++) {
      commStates[i] = digitalRead(commPins[i]);
      Serial.print(commStates[i]);
      Serial.print('\t');
    }
    Serial.println("");
    delay(50);
  #endif

  // if a hexagon is touched, animate it with Fire()
  // otherwise idle with a simple huefade
  for (int i = 0; i < 6; i++) {
    strip.setPin(neoPins[i]);
    if (padIsActive(i)) {
      Serial.print("active");
      Fire(5553, 555);
    }
    else {
      Serial.print("notfire");
      setAll(strip.ColorHSV(hue + 4000 * i));
    }
    Serial.print('\t');
  }
  Serial.println("");

  hue += 600;
  delay(10);
}

bool padIsActive(byte id) {
  return digitalRead(commPins[id]);
}

// not used currently but you can if you want!
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

// helps with debugging
void indicateLED() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(indicatorLED, HIGH);
    delay(30);
    digitalWrite(indicatorLED, LOW);
    delay(90);
  }
}

// a very cool neopixel animation
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

// helper function for Fire()
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
