/* Program to display the Game of Life on an 8x8 matrix
 * See https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 * for background on the Game of Life.
 *
 * Author: Florine de Geus
 * Last revised: 21-06-2020
 *
 * HARDWARE:
 *
 * - Arduino UNO
 * - MAX7219 8x8 LED matrix
 *
 * WIRING:
 *
 * MAX7219  | UNO
 * ---------------------
 * VCC      | 5V+
 * GND      | GND
 * DIN      | 12
 * CS       | ~10
 * CLK      | ~11
 *
 */

#include "LedControlMS.h"

#define DEAD 0
#define ALIVE 1

int DELAY = 100;
int INTENSITY = 8;

int DIN = 12;
int CS = 10;
int CLK = 11;

int GRIDSIZE = 8;

int grid[8][8];

int newGrid[8][8];

LedControl lc = LedControl(DIN, CLK, CS, 1);

// generate a random grid
void randomGrid() {
  randomSeed(analogRead(A0));

  for (int col = 0; col < GRIDSIZE; col++) {
    for (int row = 0; row < GRIDSIZE; row++) {
      grid[row][col] = random(2);
    }
  }
}

// show the grid on an 8 x 8 matrix
void showGrid() {
  lc.clearDisplay(0);
  for (int col = 0; col < GRIDSIZE; col++) {
    for (int row = 0; row < GRIDSIZE; row++) {
      lc.setLed(0, col, row, grid[col][row]);
    }
  }
}

// get the number of alive neighbors of a cell (wraparound)
int countNeighbors(int x, int y) {
  return grid[(x-1) % GRIDSIZE][(y-1) % GRIDSIZE]
         + grid[(x-1) % GRIDSIZE][y]
         + grid[(x-1) % GRIDSIZE][(y+1) % GRIDSIZE]
         + grid[x][(y-1) % GRIDSIZE]
         + grid[x][(y+1) % GRIDSIZE]
         + grid[(x+1) % GRIDSIZE][(y-1) % GRIDSIZE]
         + grid[(x+1) % GRIDSIZE][y]
         + grid[(x+1) % GRIDSIZE][(y+1) % GRIDSIZE];
}

// generate the generation of the game
void evolve() {
  memcpy(newGrid, grid, sizeof(newGrid));

  for (int col = 0; col < GRIDSIZE; col++) {
    for (int row = 0; row < GRIDSIZE; row++) {
      int neighbors = countNeighbors(col, row);
      int val = grid[col][row];

      if (val == ALIVE && (neighbors < 2 || neighbors > 3)) {
        newGrid[col][row] = DEAD;
      } else if (val == DEAD && neighbors == 3) {
        newGrid[col][row] = ALIVE;
      }
    }
  }

  memcpy(grid, newGrid, sizeof(grid));
}


void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, INTENSITY);
  lc.clearDisplay(0);
  randomGrid();
  delay(500);
}

void loop() {
  evolve();
  showGrid();
  delay(500);
}
