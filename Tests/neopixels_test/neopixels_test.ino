// NeoPixel sketch that cycles through each led in strip

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        10 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 54 // Popular NeoPixel strip size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 50 // Time (in milliseconds) to pause between pixels

void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  //keep brightness less to keep power consumtion manageable
  pixels.setBrightness(255);
}

void loop() {
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(random(255), random(255), random(255)));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
  }
}
