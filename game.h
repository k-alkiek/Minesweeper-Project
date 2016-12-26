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
}detonations[4];
enum State{
    playing,won,lost
}gameState;

void printTitleAscii(); //Functions used
void printWonAscii();   //for displaying
void printLostAscii();  //ASCII art.
bool inRange(int maximum, int input); //This function is used to check whether a certain input is within (0 : maximum) range. Returns true and false.
bool validAction(char input); //This function checks whether the input “action” character is one of the accepted actions. Returns true and false.
void clearScreen(void); //Clears the screen for showing more relevant information.
void* idleTimer(void); //Function pointer for the idle thread.
void checkWin(); //Checks whether the player has won so far. The player wins when all the cells are discovered except the ones containing mines. Its logic depends on counting undiscovered cells and checking whether it equals the number of mines.It changes the global enumerator “gameState” from “playing” to “won” on meeting this condition.
void openEmptyCell(int row, int col); //The logic of this function depends on a DFS (Depth First Search) algorithm. This function is used to open an empty cell on (row,col) co-ordinates. It continuously calls “openCell” function for the eight surrounding cells.
void openCell(int row, int col); // One of the most important functions of the game. Opens a cell with co-ordinates of (row,col). It calls putMines(); putNumbers(); in the first open of the game.
void flagCell(int row, int col); // Marks a cell with co-ordinates of (row,col) with a flag.
void questionCell(int row, int col); // Marks a cell with co-ordinates of (row,col) with a question mark.
void unmarkCell(int row, int col); // Unmarks a cell with co-ordinates of (row,col).
void win (); // This is the winning function. It’s called only by “play” function when the gameState changes from “playing” to “won”. The function flags mines, redraws the grid after winning and calls printWonAscii(); and getScore();
void lose(); // This is the losing function. It’s called only by “play” function when the gameState enumerator changes from “playing” to “lost”. The function marks detonated mines marked with '!', incorrect flags with ‘-‘, correctly-flagged mines with ‘*’, missed mines with ‘M’ and reveals the whole grid. Also calls printLostAscii(); .
void play(double,int); // This function controls the flow of the game and handles the player moves. It calls most of the previous functions in its body. As it is called by loading a previous game, its parameters are “timeAlreadyPassed”, which is self-commenting, and “localInitialOpen” which records the first “openCell” function call which is vital to distributing the mines and numbers as explained.
void Game(); // The main game function that holds all the game logic within and is generally active on the main menu where the player chooses one of four options : 1. Start a new game. 2. Load a previous game. 3. Highscores. 4. Exit.
void save(); // Saves the current game in “save.txt”.
void load(); // Loads a previous game in “save.txt”.
void textColor(char * ForeColor, char* BackColor); // Handles color.

#endif
