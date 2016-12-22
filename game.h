#ifndef GAMEHEADER
#define GAMEHEADER
#include <stdbool.h>
#include <time.h>

time_t timeStart,timeNow;

int initialOpen;

int flags,questions,moves;
double timePassed;
bool isIdle;
int boom;

struct DETONATIONS {
    int row;
    int col;
}detonations[8];
enum state{
    playing,won,lost
}gameState;

bool inRange(int maximum, int input);
bool validAction(char input);
void clearScreen(void);
void play(double,int);
void Game();
void textColor(char * ForeColor, char* BackColor);
void openEmptyCell(int row, int col);
void lose();
void win();

#endif
