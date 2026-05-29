#include <FastLED.h>

#define DATA_PIN   6
#define NUM_LEDS   256
#define MATRIX_W   16
#define MATRIX_H   16
#define BRIGHTNESS 10

CRGB leds[NUM_LEDS];

int xy(int x, int y) {
  if (x < 0 || x >= MATRIX_W || y < 0 || y >= MATRIX_H) return -1;
  if (y % 2 == 0) return y * MATRIX_W + (MATRIX_W - 1 - x);
  else return y * MATRIX_W + x;
}

void drawPixel(int x, int y, CRGB color) {
  int index = xy(x, y);
  if (index >= 0) leds[index] = color;
}

const uint8_t font3x5[][5] = {
  {0b111, 0b101, 0b101, 0b101, 0b111}, // 0 [0]
  {0b010, 0b110, 0b010, 0b010, 0b111}, // 1 [1]
  {0b111, 0b001, 0b111, 0b100, 0b111}, // 2 [2]
  {0b111, 0b001, 0b011, 0b001, 0b111}, // 3 [3]
  {0b101, 0b101, 0b111, 0b001, 0b001}, // 4 [4]
  {0b111, 0b100, 0b111, 0b001, 0b111}, // 5 [5]
  {0b111, 0b100, 0b111, 0b101, 0b111}, // 6 [6]
  {0b111, 0b001, 0b001, 0b001, 0b001}, // 7 [7]
  {0b111, 0b101, 0b111, 0b101, 0b111}, // 8 [8]
  {0b111, 0b101, 0b111, 0b001, 0b111}, // 9 [9]
  {0b010, 0b101, 0b111, 0b101, 0b101}, // A [10]
  {0b110, 0b101, 0b110, 0b101, 0b110}, // B [11]
  {0b101, 0b101, 0b111, 0b101, 0b101}, // H [12]
  {0b101, 0b111, 0b101, 0b101, 0b101}, // N [13]
  {0b110, 0b101, 0b101, 0b101, 0b110}, // D [14]
  {0b100, 0b100, 0b100, 0b100, 0b111}, // L [15]
};

void drawChar3(int ox, int oy, int idx, CRGB color) {
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 3; col++) {
      if (font3x5[idx][row] & (1 << (2 - col))) {
        drawPixel(ox + col, oy + row, color);
      }
    }
  }
}

uint8_t hue = 0;
int target = 0;

void drawDisplay() {
  FastLED.clear();

  // HAND row y=1
  drawChar3(0,  1, 12, CHSV(hue,     255, 255)); // H
  drawChar3(4,  1, 10, CHSV(hue+32,  255, 255)); // A
  drawChar3(8,  1, 13, CHSV(hue+64,  255, 255)); // N
  drawChar3(12, 1, 14, CHSV(hue+96,  255, 255)); // D

  // BALL row y=7
  drawChar3(0,  7, 11, CHSV(hue+128, 255, 255)); // B
  drawChar3(4,  7, 10, CHSV(hue+160, 255, 255)); // A
  drawChar3(8,  7, 15, CHSV(hue+192, 255, 255)); // L
  drawChar3(12, 7, 15, CHSV(hue+224, 255, 255)); // L

  // divider
  for (int x = 0; x < MATRIX_W; x++) {
    drawPixel(x, 12, CRGB(20, 20, 20));
  }

  // T — top bar + stem
  drawPixel(0, 13, CRGB::White);
  drawPixel(1, 13, CRGB::White);
  drawPixel(2, 13, CRGB::White);
  drawPixel(1, 14, CRGB::White);
  drawPixel(1, 15, CRGB::White);

  // colon
  drawPixel(3, 13, CRGB::White);
  drawPixel(3, 15, CRGB::White);

  // tens digit
  drawChar3(5, 13, target / 10, CRGB::Yellow);

  // units digit
  drawChar3(10, 13, target % 10, CRGB::Yellow);

  FastLED.show();
}

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  randomSeed(analogRead(A0) * analogRead(A1) + millis());
  target = random(10, 99);
}

void loop() {
  hue += 2;
  drawDisplay();
  delay(80);
}