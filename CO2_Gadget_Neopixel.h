#ifndef CO2_Gadget_Neopixel_h
#define CO2_Gadget_Neopixel_h

// clang-format off
/*****************************************************************************************************/
/*********                                                                                   *********/
/*********               SETUP NEOPIXEL (ES2812b AND OTHERS) LED FUNCTIONALITY               *********/
/*********                                                                                   *********/
/*****************************************************************************************************/
// clang-format on
#include <Adafruit_NeoPixel.h>

// Next data always defined to be able to configure in menu
int16_t neopixelBrightness = 100;
neoPixelType selectedNeopixelType = NEO_GRB + NEO_KHZ800;

// Declare our NeoPixel strip object:
// Adafruit_NeoPixel strip(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip(NEOPIXEL_COUNT, NEOPIXEL_PIN, selectedNeopixelType);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 255, 0);
uint32_t lime = strip.Color(0, 0, 255);
uint32_t magenta = strip.Color(255, 255, 0);
uint32_t teal = strip.Color(0, 255, 255);
uint32_t yellow = strip.Color(255, 0, 255);
uint32_t white = strip.Color(255, 255, 255);
uint32_t orange = strip.Color(255, 0, 128);
uint32_t pink = strip.Color(255, 203, 192);
uint32_t purple = strip.Color(128, 128, 0);
uint32_t green = strip.Color(0, 0, 128);
uint32_t brown = strip.Color(165, 42, 42);
uint32_t maroon = strip.Color(128, 0, 0);

void initNeopixel() {
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void neopixelLoop() {
  if (co2 >= co2RedRange) {
    strip.fill(red);
  } else if (co2 >= co2OrangeRange) {
    strip.fill(yellow);
  } else {
    strip.fill(red);
  }
  strip.show();
}

void setNeopixelBrightness(uint8_t newBrightness) {
  Serial.printf("Setting neopixel brightness value at %d\n", newBrightness);
  strip.setBrightness(newBrightness);
  strip.show();
}

void setNeopixelType(neoPixelType newType) {
  Serial.printf("Setting neopixel type value at %d\n", newType);
  strip.updateType(newType);
  strip.clear();
}

#endif  // CO2_Gadget_Neopixel_h
