#include <stdio.h>
#include <math.h>
#include <time.h>

#include "core/input.h"
#include "core/console.h"
#include "core/tetris.h"

#define GAMESPEED 5
#define GAME_WIDTH 10
#define GAME_HEIGHT 20

int tetrisSpace[GAME_HEIGHT][GAME_WIDTH] = { 0 };
struct tetrisPiece unit;
enum COLOR unitColor;
struct size screen;

void rotateTetrisPiece (int times) {
  int i;
  for (i = 0; i < times; i++) {
    rotate3x3Matrix(unit.block);
  }
};

enum COLOR colors[] = { RED, GREEN, YELLOW, MAGENTA, CYAN };
void recreateTetrisUnit (void) {
  int N = sizeof(tetrisPieceSchemes) / sizeof(tetrisPieceSchemes[0]);
  int n = rand() % N;
  int x, y;
  for (y = 0; y < 3; y++) {
    for (x = 0; x < 3; x++) {
      unit.block[y][x] = tetrisPieceSchemes[n][y][x];
    }
  }
  unit.canRotate = tetrisPieceSchemesRotateDef[n];
  unitColor = colors[rand() % (sizeof(colors) / sizeof(colors[0]))];
  rotateTetrisPiece(rand() % 4);
  unit.position.y = -3;
  unit.position.x = rand() % (GAME_WIDTH - 5) + 3;
};

int getUnitBlockAt (int x, int y) {
  int dx = x - unit.position.x, dy = y - unit.position.y;
  if (dx < 0 || dy < 0 || dx > 2 || dy > 2) return 0;
  return unit.block[dy][dx];
}

int getBlockAt (int x, int y) {
  if (x < 0  || x >= GAME_WIDTH) return 1;
  if (y < 0) return 0;
  if (y >= GAME_HEIGHT) return 1;
  if (tetrisSpace[y][x] == 1) return 1;
  return 0;
};

int nextPositionIsBlocked (int vx, int vy) {
  int y, x, dy, dx;
  for (y = 0; y < 3; y++) {
    for (x = 0; x < 3; x++) {
      dy = unit.position.y + y;
      dx = unit.position.x + x;
      if (unit.block[y][x] && getBlockAt(dx + vx, dy + vy)) {
        return 1;
      }
    }
  }
  return 0;
};

int canUnitMove (enum DIRECTION direction) {
  int offset = 0;
  if (direction == LEFT)  offset = -1;
  if (direction == RIGHT) offset = +1;
  return unit.position.y > 0 && !nextPositionIsBlocked(offset, 0);
};

int canUnitRotate (void) {
  int x, y;
  if (!unit.canRotate) return 0;
  for (y = 0; y < 3; y++) {
    for (x = 0; x < 3; x++) {
      if (getBlockAt(unit.position.x + x, unit.position.y + y)) {
        return 0;
      }
    }
  }
  return 1;
}

void printGame (struct size *display, int score, int gameOver) {
  static int paddingLeft;
  static int paddingTop;
  // Resize event
  if ((*display).width != screen.width || (*display).height != screen.height) {
    clear();
    screen.width = (*display).width;
    screen.height = (*display).height;
    paddingLeft = floor(((*display).width - 2 * GAME_WIDTH) / 2);
    paddingTop = floor(((*display).height - GAME_HEIGHT) / 3);
  }
  int x, y, i, yOffset = 0;
  setCursor(0, 0); // Move cursor to (0,0)
  setCursor(paddingLeft, paddingTop + (yOffset++)); printf("Tetris C Edition");
  setCursor(paddingLeft, paddingTop + (yOffset++)); printNTimes("#", (GAME_WIDTH + 1) * 2);
  for (y = 0; y < GAME_HEIGHT; y++) {
    setCursor(paddingLeft, paddingTop + (yOffset++));
    printf("#");
    for (x = 0; x < GAME_WIDTH; x++) {
      if (getUnitBlockAt(x, y)) {
        setColor(unitColor); printf("[]"); setColor(RESET);
      } else if (getBlockAt(x, y)) {
        setColor(BLUE); printf("[]"); setColor(RESET);
      } else {
        printf("..");
      }
    }
    printf("#");
  }
  setCursor(paddingLeft, paddingTop + (yOffset++));
  printNTimes("#", (GAME_WIDTH + 1) * 2);
  if (gameOver) {
    setCursor(paddingLeft, paddingTop + (yOffset++)); printf("Game Over!");
    setCursor(paddingLeft, paddingTop + (yOffset++)); printf("Your score: %d", score);
  } else {
    setCursor(paddingLeft, paddingTop + (yOffset++)); printf("Score: %d", score);
  }
};

int main () {
  srand(time(NULL));
  clear();

  int score = 0;
  clock_t lastTs = 0;
  enum DIRECTION lastPressed;

  recreateTetrisUnit();

  // Game loop
  while (1) {
    struct size display = getTerminalSize();
    if (display.width < (GAME_WIDTH * 2 + 2) || display.height < (GAME_HEIGHT + 6)) {
      printf("Terminal size is not big enough!\n");
      return 0;
    }
    enum DIRECTION keypressed = readInput();
    if (keypressed != NUL) {
      lastPressed = keypressed;
    }
    if ((clock() - lastTs) < (1000 / GAMESPEED)) {
      continue; // skip
    }
    lastTs = clock();
    switch(lastPressed) {
      case LEFT:   if (canUnitMove(LEFT))  unit.position.x--; break;
      case RIGHT:  if (canUnitMove(RIGHT)) unit.position.x++; break;
      case TOP:    if (canUnitRotate()) rotateTetrisPiece(3); break;
      case BOTTOM: if (canUnitRotate()) rotateTetrisPiece(1); break;
    }
    lastPressed = NUL;
    score++;
    int immobilized = 0;
    if (nextPositionIsBlocked(0, 1)) {
      if (unit.position.y < 0) {
        break;
      }
      score += 5;
      // copy unit.block -> tetrisSpace
      int x, y;
      for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
          if (unit.block[y][x] == 1) {
            tetrisSpace[unit.position.y + y][unit.position.x + x] = 1;
          }
        }
      }
      immobilized = 1;
      recreateTetrisUnit();
    } else {
      unit.position.y++;
    }
    printGame(&display, score, 0);
    if (immobilized) {
      int x, y, yy, everyBoolean;
      // delete full lines
      for (y = 0; y < GAME_HEIGHT; y++) {
        everyBoolean = 1;
        for (x = 0; x < GAME_WIDTH; x++) {
          if (tetrisSpace[y][x] == 0) everyBoolean = 0;
        }
        if (everyBoolean) {
          score += 20;
          for (x = 0; x < GAME_WIDTH; x++) {
            tetrisSpace[y][x] = 0;
          }
          // swap lines
          for (yy = y; yy > 0; yy--) {
            for (x = 0; x < GAME_WIDTH; x++) {
              tetrisSpace[yy][x] = tetrisSpace[yy - 1][x];
            }
          }
        }
      }
    }
  }
  printGame(&screen, score, 1);
  return 0;
};