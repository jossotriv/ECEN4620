#include <Adafruit_NeoPixel.h>

#define LED_PIN     13  // Set the correct GPIO pin number
#define NUM_LEDS    1

// Define the index for each color
#define RED 0
#define BLUE 1
#define GREEN 2

// Create an instance of the Adafruit_NeoPixel class called 'strip'
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();  // Initialize the NeoPixel strip
  strip.show();   // Initialize all pixels to 'off'
}

void ledChoice(int color) {
  switch (color) {
    case RED:
      strip.setPixelColor(0, strip.Color(255, 0, 0)); // Set the first pixel to red
      break;
    case GREEN:
      strip.setPixelColor(0, strip.Color(0, 255, 0)); // Set the first pixel to green
      break;
    case BLUE:
      strip.setPixelColor(0, strip.Color(0, 0, 255)); // Set the first pixel to blue
      break;
  }
  strip.show();  // Update the strip with new color settings
  delay(500);    // Delay for visibility
}

void loop() {
  for (int color = RED; color <= BLUE; color++) {
    ledChoice(color);  // Change colors from red to blue
  }
  
  strip.clear();       // Turn off all pixels
  strip.show();
  delay(1000);         // Wait for a second
  Serial.println("Cycle complete, starting over...");
}
