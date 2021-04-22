#include <Windows.h>
#include <stdio.h>
struct size {
  unsigned int width;
  unsigned int height;
};
enum COLOR {
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
  RESET
};
struct size getTerminalSize (void);
void clear(void);
void setCursor(int x, int y);
void printNTimes (const char *str, int n);
void setColor (enum COLOR color);