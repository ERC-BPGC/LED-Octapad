#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define pixelCount 54
#define neoPin 10

Adafruit_NeoPixel strip(pixelCount, neoPin, NEO_GRB + NEO_KHZ800);

volatile byte pitch, touch;
float delaytime = 50;
float lastmillis;

void animationA()   //when nothing is touched
{
  strip.fill(255,255,255);
  strip.show();  
}
void animationB()
{
  Fire(55,120,15);
}
void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[pixelCount];
  int cooldown;
 
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < pixelCount; i++) {
    cooldown = random(0, ((Cooling * 10) / pixelCount) + 2);
   
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
 
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= pixelCount - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
   
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < pixelCount; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  showStrip();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}
// *** REPLACE TO HERE ***

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < pixelCount; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
void setup() {
  Wire.begin(2);
  Wire.onReceive(receiveData);

  Serial.begin(115200);

  strip.begin();
  strip.setBrightness(100);
}

int dly = 500;
void loop() {
  
  if (millis() - lastmillis >= delaytime){
    lastmillis = millis();
    if (touch){
      animationA();
    }
    else {
      animationB();
    }
  }
//  
//  if (touch){
//    Sparkle();
//  }
//  else {
//    strip.fill(255, 0, 100);
//    strip.show();
//  }
//  delay(10);
}

void receiveData(int howMany) {
  if (Wire.available() >= 2) {
    pitch = Wire.read();
    touch = Wire.read();
  }
  Serial.print(pitch);
  Serial.print('\t');
  Serial.println(touch);
}

void Sparkle(){
  strip.setPixelColor(random(pixelCount), random(255), random(255), random(255));
  strip.show();
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
