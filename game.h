#ifndef GAMEHEADER
#define GAMEHEADER

enum state{
    playing,win,lose
};
bool inRange(int maximum, int input);
bool validAction(char input);
void clearScreen(void);
bool play(double, int, bool);
void Game();
void textColor(char * ForeColor, char* BackColor);
void openEmptyCell(int row, int col);


#endif
