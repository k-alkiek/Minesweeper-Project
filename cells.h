#ifndef CELLSHEADER
#define CELLSHEADER


struct cell{
    int discovered;
    int mined;
    int number;
    int flagged;
    int question;
    char show;
};

void getSize();
void draw();
void gridInit();
void putBlank(int row, int col);
void putMines();
void putNumbers();
#endif
