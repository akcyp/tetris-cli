#include "input.h"
enum DIRECTION readInput (void) {
  if (_kbhit()) {
    int code = _getch();
    enum DIRECTION dir;
    switch(code) {
      case 75: case  97: return LEFT;
      case 72: case 119: return TOP;
      case 77: case 100: return RIGHT;
      case 80: case 115: return BOTTOM;
    }
  }
  return NUL;
}