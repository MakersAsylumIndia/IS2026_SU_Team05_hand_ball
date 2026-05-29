#include <Wire.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

const int ROWS = 3;
const int COLS = 3;

const int startButton   = 22;
const int stopButton    = 23;
const int player1Button = 24;
const int player2Button = 25;

bool gameRunning = false;
int targetX = 0;
int targetY = 0;
int currentPlayer = 1;
int scoreP1 = 0;
int scoreP2 = 0;

void setup() {
  Serial.begin(9600);

  pinMode(startButton,   INPUT_PULLUP);
  pinMode(stopButton,    INPUT_PULLUP);
  pinMode(player1Button, INPUT_PULLUP);
  pinMode(player2Button, INPUT_PULLUP);

  display.begin();
  Serial.println("=== HANDBALL GAME ===");

  randomSeed(analogRead(A15));
  showWelcome();
}

void loop() {
  if (digitalRead(startButton) == LOW && !gameRunning) {
    delay(300);
    startGame();
  }
  if (digitalRead(stopButton) == LOW && gameRunning) {
    delay(300);
    endGame();
  }
  if (digitalRead(player1Button) == LOW && gameRunning && currentPlayer == 1) {
    delay(300);
    player1Scored();
  }
  if (digitalRead(player2Button) == LOW && gameRunning && currentPlayer == 2) {
    delay(300);
    player2Scored();
  }
}

void startGame() {
  gameRunning = true;
  scoreP1 = 0;
  scoreP2 = 0;
  currentPlayer = 1;
  Serial.println("=== GAME STARTED ===");
  generateNewTarget();
}

void endGame() {
  gameRunning = false;
  Serial.println("=== GAME ENDED ===");
  Serial.print("P1: "); Serial.println(scoreP1);
  Serial.print("P2: "); Serial.println(scoreP2);

  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);
  display.drawStr(10, 20, "GAME");
  display.drawStr(10, 40, "OVER");
  display.setFont(u8g2_font_6x10_tr);
  char buf[20];
  sprintf(buf, "P1:%d  P2:%d", scoreP1, scoreP2);
  display.drawStr(0, 58, buf);
  display.sendBuffer();
}

void generateNewTarget() {
  targetX = random(0, ROWS);
  targetY = random(0, COLS);
  Serial.print("TARGET: ("); Serial.print(targetX); Serial.print(","); Serial.print(targetY); Serial.println(")");
  Serial.print("PLAYER "); Serial.print(currentPlayer); Serial.println(" TURN");
  updateOLED();
}

void player1Scored() {
  Serial.println("PLAYER 1 HIT!");
  scoreP1++;
  Serial.print("P1 Score: "); Serial.println(scoreP1);
  currentPlayer = 2;
  generateNewTarget();
}

void player2Scored() {
  Serial.println("PLAYER 2 HIT!");
  scoreP2++;
  Serial.print("P2 Score: "); Serial.println(scoreP2);
  currentPlayer = 1;
  generateNewTarget();
}

void updateOLED() {
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);
  display.drawStr(0, 16, currentPlayer == 1 ? "P1 TURN" : "P2 TURN");
  display.setFont(u8g2_font_6x10_tr);
  char target[20];
  sprintf(target, "Target: (%d,%d)", targetX, targetY);
  display.drawStr(0, 36, target);
  char scores[20];
  sprintf(scores, "P1:%d   P2:%d", scoreP1, scoreP2);
  display.drawStr(0, 52, scores);
  display.sendBuffer();
}

void showWelcome() {
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB14_tr);
  display.drawStr(5, 30, "HANDBALL");
  display.setFont(u8g2_font_6x10_tr);
  display.drawStr(20, 50, "PRESS START");
  display.sendBuffer();
}