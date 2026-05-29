#include <Wire.h>
#include <U8g2lib.h>
#include <FastLED.h>

// ================= OLED =================
U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

// ================= BUTTONS =================
const int startButton   = 22;
const int stopButton    = 23;
const int player1Button = 24;
const int player2Button = 25;

// ================= RGB MATRIX =================
#define DATA_PIN   6
#define NUM_LEDS   256
#define MATRIX_W   16
#define MATRIX_H   16
#define BRIGHTNESS 10

CRGB leds[NUM_LEDS];

// ================= GAME VARIABLES =================
int scoreP1  = 0;
int scoreP2  = 0;
int targetX  = 0;
int targetY  = 0;
int currentScreen = 0;
int max_target = 13;

// ================= MATRIX HELPERS =================
int xy(int x, int y) {
  if (x < 0 || x >= MATRIX_W || y < 0 || y >= MATRIX_H) return -1;
  if (y % 2 == 0) return y * MATRIX_W + (MATRIX_W - 1 - x);
  else return y * MATRIX_W + x;
}

void drawPixel(int x, int y, CRGB color) {
  int index = xy(x, y);
  if (index >= 0) leds[index] = color;
}

void newTarget() {
  int prevX = targetX;
  int prevY = targetY;
  do {
    targetX = random(0, max_target);
    targetY = random(0, max_target);
  } while (targetX == prevX && targetY == prevY);
}

void matrixShowTarget(CRGB color) {
  FastLED.clear();
  for (int x = 0; x < 4; x++) {
    for (int y = 0; y < 4; y++) {
      // if (x > 16 || y > 16){
      //   continue;
      // }
      drawPixel(targetX + x, targetY + y, color);
    }
  }
  FastLED.show();
}

void matrixShowGameOver() {
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB::Red;
  FastLED.show();
}

void matrixClear() {
  FastLED.clear();
  FastLED.show();
}

// ================= OLED SCREENS =================
void showIdle() {
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tr);
  display.drawStr(25, 36, "PRESS START");
  display.sendBuffer();
}

void showHome() {
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);
  display.drawStr(15, 28, "HANDBALL");
  display.setFont(u8g2_font_6x10_tr);
  display.drawStr(15, 50, "Press P1 to start");
  display.sendBuffer();
}

void showP1() {
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tr);
  display.drawStr((128 - display.getStrWidth("PLAYER 1")) / 2, 12, "PLAYER 1");
  display.setFont(u8g2_font_ncenB14_tr);
  char buf[10];
  sprintf(buf, "Score = %d", scoreP1);
  display.drawStr((128 - display.getStrWidth(buf)) / 2, 42, buf);
  display.sendBuffer();
}

void showP2() {
  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tr);
  display.drawStr((128 - display.getStrWidth("PLAYER 2")) / 2, 12, "PLAYER 2");
  display.setFont(u8g2_font_ncenB14_tr);
  char buf[10];
  sprintf(buf, "Score = %d", scoreP2);
  display.drawStr((128 - display.getStrWidth(buf)) / 2, 42, buf);
  display.sendBuffer();
}

void showGameOver() {
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);
  const char* msg = "GAME OVER";
  int x = (128 - display.getStrWidth(msg)) / 2;
  display.drawStr(x, 38, msg);
  display.sendBuffer();
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  pinMode(startButton,   INPUT_PULLUP);
  pinMode(stopButton,    INPUT_PULLUP);
  pinMode(player1Button, INPUT_PULLUP);
  pinMode(player2Button, INPUT_PULLUP);

  display.begin();

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  randomSeed(analogRead(A0) * analogRead(A1) + millis());

  Serial.println("READY");
  currentScreen = 0;
  showIdle();
  matrixClear();
}

// ================= LOOP =================
void loop() {

  bool s  = digitalRead(startButton)   == LOW;
  bool p1 = digitalRead(player1Button) == LOW;
  bool p2 = digitalRead(player2Button) == LOW;
  bool st = digitalRead(stopButton)    == LOW;

  if (s) {
    Serial.println("START PRESSED");
    scoreP1 = 0;
    scoreP2 = 0;
    newTarget();
    currentScreen = 1;
    showHome();
    matrixClear();
    delay(300);
  }
  else if (p1) {
    Serial.println("P1 PRESSED");
    currentScreen = 2;
    newTarget();
    showP1();
    matrixShowTarget(CRGB::Red);
    delay(300);
  }
  else if (p2) {
    Serial.println("P2 PRESSED");
    currentScreen = 3;
    newTarget();
    showP2();
    matrixShowTarget(CRGB::Blue);
    delay(300);
  }
  else if (st) {
    Serial.println("STOP PRESSED");
    currentScreen = 4;
    showGameOver();
    matrixShowGameOver();
    delay(300);
  }

  delay(80);
}