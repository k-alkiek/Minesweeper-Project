#ifndef GAMEHEADER
#define GAMEHEADER
#include <stdbool.h>
bool inRange(int maximum, int input);
bool validAction(char input);
void clearScreen(void);
bool play();
bool startup();
void textColor(char * ForeColor, char* BackColor);
void openEmptyCell(int row, int col);


#endif
