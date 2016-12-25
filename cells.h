#ifndef CELLSHEADER
#define CELLSHEADER

int r,c,mines; //rows, columns and mines.

struct cell{
    int discovered;
    int mined;
    int number;
    int flagged;
    int question;
    char show;
}grid[30][30];

void getSize();
void draw();
void gridInit();
void putBlank(int row, int col);
void putMines();
void putNumbers();
#endif
