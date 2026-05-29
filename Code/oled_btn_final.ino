#include <Wire.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

const int startButton   = 22;
const int stopButton    = 23;
const int player1Button = 24;
const int player2Button = 25;

int scoreP1 = 0;
int scoreP2 = 0;

void setup() {
  Serial.begin(9600);

  pinMode(startButton,   INPUT_PULLUP);
  pinMode(stopButton,    INPUT_PULLUP);
  pinMode(player1Button, INPUT_PULLUP);
  pinMode(player2Button, INPUT_PULLUP);

  display.begin();
  Serial.println("READY");
  showIdle();
}

void loop() {

  if (digitalRead(startButton) == LOW) {
    Serial.println("START PRESSED");
    delay(300);
    scoreP1 = 0;
    scoreP2 = 0;
    showHome();
    while (digitalRead(startButton) == LOW);
  }

  if (digitalRead(player1Button) == LOW) {
    Serial.println("P1 PRESSED");
    delay(300);
    showP1();
    while (digitalRead(player1Button) == LOW);
  }

  if (digitalRead(player2Button) == LOW) {
    Serial.println("P2 PRESSED");
    delay(300);
    showP2();
    while (digitalRead(player2Button) == LOW);
  }

  if (digitalRead(stopButton) == LOW) {
    Serial.println("STOP PRESSED");
    delay(300);
    showGameOver();
    while (digitalRead(stopButton) == LOW);
  }
}

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

  // label top center small
  display.setFont(u8g2_font_6x10_tr);
  display.drawStr((128 - display.getStrWidth("PLAYER 1")) / 2, 12, "PLAYER 1");

  // score big centered
  display.setFont(u8g2_font_ncenB14_tr);
  char buf[10];
  sprintf(buf, "Score = %d", scoreP1);
  display.drawStr((128 - display.getStrWidth(buf)) / 2, 42, buf);

  display.sendBuffer();
}

void showP2() {
  display.clearBuffer();

  // label top center small
  display.setFont(u8g2_font_6x10_tr);
  display.drawStr((128 - display.getStrWidth("PLAYER 2")) / 2, 12, "PLAYER 2");

  // score big centered
  display.setFont(u8g2_font_ncenB14_tr);
  char buf[10];
  sprintf(buf, "Score = %d", scoreP2);
  display.drawStr((128 - display.getStrWidth(buf)) / 2, 42, buf);

  display.sendBuffer();
}

void showGameOver() {
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);
  // center GAME OVER
  const char* msg = "GAME OVER";
  int x = (128 - display.getStrWidth(msg)) / 2;
  display.drawStr(x, 38, msg);
  display.sendBuffer();
}