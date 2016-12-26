#ifndef CELLSHEADER
#define CELLSHEADER

int r,c,mines; //rows, columns and mines.

struct Cell{
    int discovered;
    int mined;
    int number;
    int flagged;
    int question;
    char show;
}grid[30][30];

void getSize(); // Gets the dimensions of the desired grid of cells from the user. Only accepts each dimension in the (2 : 30) range.
void gridInit(); // Initializes the grid.
void draw(); // Draws the grid.
void putBlank(int row, int col); // This function is called by gridInit(); . As the grid is defined as a multidimensional (2D) array of structures, this functions sets all the cell’s information (discovered, mined, number, flagged, question) to zero/false and the last information (show) to ‘X’ which indicates a closed, undiscovered cell.
void putMines(); // Distributes mines randomly in the cells. Uses “timeStart” as seed for the random function.
void putNumbers(); // Calculates the number of surrounding mine for non-mined cells. It finds mines, and adds one to the “number” of every surrounding non-mined cell.
void colorPrintChar(char c); // Handles printing colors for characters in the game for better UI. Called by draw();
#endif
