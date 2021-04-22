#include "console.h"
struct size getTerminalSize (void) {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  struct size terminalSize;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  terminalSize.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  terminalSize.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  return terminalSize;
};
void clear(void){
  system("cls");
};
void setCursor(int x, int y) {
  printf("\033[%d;%dH", y + 1, x + 1);
};
void printNTimes (const char *str, int n) {
  while(n--) {
    printf(str);
  }
};
void setColor (enum COLOR color) {
  switch (color) {
    case BLACK:   printf("\033[30m"); break;
    case RED:     printf("\033[31m"); break;
    case GREEN:   printf("\033[32m"); break;
    case YELLOW:  printf("\033[33m"); break;
    case BLUE:    printf("\033[34m"); break;
    case MAGENTA: printf("\033[35m"); break;
    case CYAN:    printf("\033[36m"); break;
    case WHITE:   printf("\033[37m"); break;
    case RESET:   printf("\033[0m");  break;
  }
}