#include <LedControlMS.h>
#include <SevSeg.h>

#ifdef ARDUINO
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#endif

/* Joystick pins */
#define VRX A4
#define VRY A5
#define SW 2

/* LED matrix pins */
#define DIN 16
#define CS 15
#define CLK 14

#define SIZE 8
#define ADVDELAY 200
#define MSGSPEED 5

int snake[SIZE * SIZE][2];
int length;
int score;
int food[2], v[2];
bool gameOver = false;
long currTime;
long prevAdv;
int blinkCnt;

SevSeg sevseg;
LedControl lc = LedControl(DIN, CLK, CS);

/* Initialize the seven segment display that displays the score */
void initSevSeg()
{
  byte numDigits = 4;
  byte digitPins[] = {13, 12, 11, 10};
  byte segmentPins[] = {7, 6, 5, 4, 3, 2, 1};

  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_ANODE;
  bool updateWithDelays = false;
  bool leadingZeros = true;
  bool disableDecPoint = true;

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins,
               resistorsOnSegments, updateWithDelays, leadingZeros,
               disableDecPoint);
}

/* Setup the game itself */
void setupGame()
{
  prevAdv = currTime = 0;
  blinkCnt = 3;
  int half = SIZE / 2;
  length = SIZE / 3;

  for (int i = 0; i < length; i++)
  {
    snake[i][0] = half - 1;
    snake[i][1] = half + 1;
  }

  food[0] = half + 1;
  food[1] = half - 1;

  v[0] = 0;
  v[1] = -1;
}

/* Show game over msg. TODO: make more fancy */
void showGameOver()
{
  sevseg.setChars("DEAD");
  sevseg.refreshDisplay();
}

/* Render the current state of the game */
void render()
{
  for (int i = 0; i < length; i++)
  {
    lc.setLed(0, snake[i][0], snake[i][1], 1);
  }

  lc.setLed(0, food[0], food[1], 1);
}

/* Wipe the screen */
void clearScreen()
{
  lc.clearDisplay(0);
}

/* Advance the snek one timestep.
 * Return true of game over
 */
bool advance()
{
  int head[2] = {snake[0][0] + v[0], snake[0][1] + v[1]};

  if ((head[0] < 0 || head[0] >= SIZE) || (head[1] < 0 || head[1] >= SIZE))
  {
    delay(1000);
    showGameOver();
    return true;
  }

  for (int i = 0; i < length; i++)
  {
    if (snake[1][0] == head[0] && snake[i][1] == head[1])
    {
      delay(1000);
      showGameOver();
      return true;
    }
  }

  /* Grow the snek */
  if (head[0] == food[0] && head[1] == food[1])
  {
    length++;
    /* A3 is not used, so we can use it as seed for the RGN */
    randomSeed(A3);
    food[0] = random(SIZE);
    food[1] = random(SIZE);
  }

  for (int i = length - 1; i >= 0; i--)
  {
    snake[i+1][0] = snake[1][0];
    snake[i+1][1] = snake[1][1];
  }

  snake[0][0] += v[0];
  snake[0][1] += v[1];

  return false;
}

/* Restart the game */
void restart()
{
  setupGame();
  gameOver = false;
}

/* Read the controls from the joystick */
void readCtrls()
{
  int dx = map(analogRead(VRX), 0, 906, 2, -2);
  int dy = map(analogRead(VRY), 0, 906, -2, 2);

  if (dx != 0) {dx /= abs(dx);}
  if (dy != 0) {dy /= abs(dy);}

  if (dy != 0 && v[0] != 0)
  {
    v[0] = 0;
    v[1] = dy;
  }

  if (dx != 0 && v[1] != 0)
  {
    v[0] = dx;
    v[1] = 0;
  }
}

void setup()
{
  pinMode(SW, INPUT_PULLUP);
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);

  attachInterrupt(digitalPinToInterrupt(SW), restart, RISING);

  initSevSeg();

  sevseg.setChars("SNEK");
  lc.shutdown(0, false);

  setupGame();
  render();
}

void loop()
{
  if (!gameOver)
  {
    clearScreen();
    render();

    if (currTime - prevAdv > ADVDELAY)
    {
      gameOver = advance();
      prevAdv = currTime;
    }
  }
  else
  {
    while (blinkCnt > 0)
    {
      clearScreen();
      delay(300);
      showGameOver();
      render();
      delay(300);
      blinkCnt--;
    }
  }

  readCtrls();
  currTime++;
}
