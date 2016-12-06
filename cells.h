#ifndef CELLSHEADER
#define CELLSHEADER


struct cell{
    int discovered;
    int mined;
    int number;
    int flag;
    int question;
    char show;
};

void getSize();
void draw();
void gridInit();

#endif
