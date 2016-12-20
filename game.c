#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <pthread.h>

#include "cells.h"
#include "game.h"

#define CELL(row,col) grid[row][col]
#define IDLE_TIME 60

time_t timeStart,timeNow;
bool messageFlag = 0;
int initialOpen;
char message[100];
int flags,questions,moves;
extern int r,c, mines;
extern struct cell grid[30][30];

enum state gameState;
bool isIdle;

char * title[] =  {"  __  __  _                     ____                                         ",
                   " |  \\/  |(_) _ __    ___ __/\\__/ ___|__      __ ___   ___  _ __    ___  _ __ ",
                   " | |\\/| || || '_ \\  / _ \\\\    /\\___ \\\\ \\ /\\ / // _ \\ / _ \\| '_ \\  / _ \\| '__|",
                   " | |  | || || | | ||  __//_  _\\ ___) |\\ V  V /|  __/|  __/| |_) ||  __/| |   ",
                   " |_|  |_||_||_| |_| \\___|  \\/  |____/  \\_/\\_/  \\___| \\___|| .__/  \\___||_|   ",
                   "                                                          |_|                "};

void printTitle(){
    int titleSize = sizeof(title) / sizeof(char*);

    textColor("RED","DEFAULT");
    int i,j;
    for(i=0; i<titleSize; i++){
        for(j=0; title[i][j]!='\0' ; j++){
            putchar(title[i][j]);
            //Sleep(1);
        }
        printf("\n");
    }
    textColor("DEFAULT","DEFAULT");
    return;
}

bool inRange(int maximum, int input){
    if(input >=0 && input < maximum) return 1;
    else return 0;
}

bool validAction(char input) {
    switch(input){
        case 'O':
        case 'o':
        case 'F':
        case 'f':
        case 'Q':
        case 'q':
        case '?':
        case 'U':
        case 'u':
        case 'S':
        case 's': return 1;
        default : return 0;
    }


}

void clearScreen(void){
    system("cls");
    return;
}

void* idleTimer(void){  //function pointer for the idle thread
    double seconds = difftime(timeNow,timeStart);   //time counter initialized to current time
    while(isIdle){      //isIdle is set to true before input and set to false after input
        time(&timeNow);


        if( difftime(timeNow,timeStart) - seconds >= IDLE_TIME){    //when IDLE_TIME passes
            clearScreen(); printf("\n   Flags:%2d\t Time: %.f\n\n",1+(r*c)/10-flags,difftime(timeNow,timeStart)); //print remaining flags, difftime returns difference between two times
            draw();                 //redraw with new time
            puts("Please enter your move in the form of ( row col action )");
            seconds = difftime(timeNow,timeStart);  //update time counter for the next iteration
            Sleep(1000);    //to avoid multiple simultaneous redraw
        }

    }
    pthread_exit(0);
}

void checkWin(){
    int undiscovered = 0;
    int i, j;
    for (i = 0 ; i < r ; i++) {
        for ( j = 0 ; j < c ; j++) {
            if (! CELL(i,j).discovered)
                undiscovered++;
        }
    }
    if (undiscovered == mines && gameState == playing) {
        gameState = win;
    }
}

void gameWin(double timePassed) {
    // FLASH TODO
    char playerName[33];
    long long score = r*r*r*r*c*c*c*c/moves/(timePassed + difftime(timeNow,timeStart));
    puts("Enter your name: ");
    fgets(playerName,32,stdin);
    printf("%s",playerName);
    getch();

}
void openEmptyCell(int row, int col) {
    int i, j;
    for(i = row-1 ; i <= row+1 ; i++){
        for (j = col-1 ; j <= col+1; j++) {
            if (i<r && j < c && i>=0 && j>=0 && !CELL(i,j).discovered)
                openCell(i,j);
        }
    }
    messageFlag = 0;
    return;
}

void openCell(int row, int col) {
    if(initialOpen){
        initialOpen = 0;
        CELL(row,col).discovered = 1;
        putMines();
        putNumbers();
        if (CELL(row,col).number)
            CELL(row,col).show = (char)(48 + CELL(row,col).number);
        else{
            CELL(row,col).show = ' ';
            openEmptyCell(row, col);
            }
        return ;
    }
    //if(CELL.discovered)
    if(CELL(row,col).discovered && !(CELL(row,col).number)){
        messageFlag = 1;
        strcpy (message,"The cell is empty.");
        return;
    }
    if (CELL(row,col).discovered == 1){
        int flagsAround = 0;
        int i,j;
        for(i = row-1 ; i <= row+1 ; i++){
            for (j = col-1 ; j <= col+1; j++) {
                if (i<r && j < c && i>=0 && j>=0 && CELL(i,j).flagged)
                    flagsAround++;
            }
        }
        if (flagsAround == CELL(row,col).number)
            openEmptyCell(row, col);
        else {
            messageFlag = 1;
            strcpy(message, "The cell is already opened.");
        }
        return;
    }
    if(CELL(row,col).flagged || CELL(row,col).question){
        messageFlag = 1;
        strcpy (message,"The cell is marked. You can unmark it first by using the action 'u'");
        return;
    }
    if (CELL(row,col).mined) {
        gameState = lose;//TODO: change gameState to enum later - LOSS/LOSE HERE
        return;
    }
    if(!(CELL(row,col).discovered)){
        CELL(row,col).discovered = 1;
        if(CELL(row,col).number){
            CELL(row,col).show = (char)(48 + CELL(row,col).number);
            return;
        }
        else{
            CELL(row,col).show = ' ';
            openEmptyCell(row, col);
            //DFS algorithm
            return;
        }
    }
}

void flagCell(int row, int col) {
    if (CELL(row,col).discovered == 1) {
        strcpy(message, "The cell is already opened.");
        messageFlag = 1;
        return;
    }
    if (CELL(row,col).flagged || CELL(row,col).question == 1) {
        messageFlag = 1;
        strcpy (message,"The cell is already marked. You can unmark it using the action 'u'");
        return;
    }
    CELL(row,col).flagged = 1;
    CELL(row,col).show = 'F' ;
    flags++;
    return;
}

void questionCell(int row, int col) {
    if (CELL(row,col).discovered == 1) {
        strcpy(message, "The cell is already opened.");
        messageFlag = 1;
        return;
    }
    if (CELL(row,col).question || CELL(row,col).flagged == 1) {
        messageFlag = 1;
        strcpy (message,"The cell is already marked. You can unmark it using the action 'u'");
        return;
    }
    CELL(row,col).question = 1;
    CELL(row,col).show = '?' ;
    questions++;
    return;
}

void unmarkCell(int row, int col) {
    if (CELL(row,col).discovered == 1) {
        strcpy(message, "The cell is already opened.");
        messageFlag = 1;
        return;
    }
    if ((CELL(row,col).question || CELL(row,col).flagged) == 0) {
        strcpy(message, "The cell is already unmarked.");
        messageFlag = 1;
        return;
    }
    if (CELL(row,col).question == 1) {
        CELL(row,col).question = 0;
        questions--;
        CELL(row,col).show = 'X' ;
        return;
    }
    if (CELL(row,col).flagged == 1) {
        CELL(row,col).flagged = 0;
        CELL(row,col).show = 'X' ;
        flags--;
        return;
    }

}

void play(double timePassed, int localInitialOpen){

    time(&timeStart);
    int rowIn, colIn;
    bool wrongInput = 0;
    char action;
    initialOpen = localInitialOpen;
    gameState = playing;
    mines = 1+(r*c)/10;

    do
    {
    time(&timeNow); //get current time
    //double seconds = difftime(timeNow,timeStart);
        clearScreen(); printf("\n    Moves: %d\t Flags: %d\t Question Marks: %d\t Time: %.f\n\n",moves,flags,questions,timePassed + difftime(timeNow,timeStart)); //print remaining flags, difftime returns difference between two times
        draw();
        if(wrongInput){
            puts("Wrong entry.");
            wrongInput = 0;
        }
        if (messageFlag) {
            puts(message);
            messageFlag = 0 ;
        }

        puts("Please enter your move in the form of ( row col action )");

        isIdle = 1;     //launch thread loop
        pthread_t idleThread;       //pthread stuff
        pthread_create(&idleThread,NULL,idleTimer,NULL);

        rowIn = -1 ; colIn = -1; action = 'z';  //invalid values


        scanf("%d %d %c", &rowIn, &colIn, &action); //TODO: Adjust input method for saving
        isIdle = 0;
        fflush(stdin);
        if ( !( inRange(r,rowIn) && inRange(c, colIn) && validAction(action) ) ){
            wrongInput = 1;
            continue;
        }
        switch(action){
            case 'O':
            case 'o': {openCell(rowIn, colIn); checkWin(); break;}
            case 'F':
            case 'f': {flagCell(rowIn, colIn); break;}
            case 'Q':
            case '?':
            case 'q': {questionCell(rowIn, colIn); break;}
            case 'U':
            case 'u': {unmarkCell(rowIn, colIn); break;}
            case 'S':  // add functions later
            case 's': {save(timePassed+difftime(timeNow,timeStart));break;}
    }
    moves++;


        // Input validation implemented like a boss B-)
    }
    while (gameState==playing);
    if (gameState == win)
        gameWin(timePassed);  //TODO
    else
        //lose(); TODO
    return;
}

void Game(void){

    do
    {
        clearScreen();
        printTitle();
        printf("\n\t1. Start a new game     (n)\n\t2. Load a previous game (l)\n\t3. Exit                 (x)\n\n\tType the letter for the desired option: ");
        char input;
        fflush(stdin);
        scanf("%c",&input);
        switch(input)
        {

        case 'n':
        {
            clearScreen();
            getSize();
            gridInit();
            play(0, 1);
            break;
        }
        case 'l':
        {
            load();
            break;
        }
        case 'x':
            return 1;
            break;
        default :
        {
            clearScreen();
            printf("Invalid entry. Please, try again\n");
            break;
        }

        }

    }
    while(1) ;
}

void save(double timePassed){
    FILE* fp = fopen("save.txt","w");

    if(fp==NULL)
        printf("Error opening file.\n");
    else{
        fflush(fp);
        fprintf(fp,"%d %d %lf %d %d %d %d\n",r,c,timePassed,moves,flags,questions,initialOpen);
        int row, col;
        for(row =0; row<r; row++){
            for(col=0; col<c; col++){
                if (CELL(row,col).show == ' ')
                    CELL(row,col).show = 's';
                fflush(fp);
                fprintf(fp,"%d %d %d %d %d %c\n",CELL(row,col).discovered,CELL(row,col).mined,CELL(row,col).number,CELL(row,col).flagged,CELL(row,col).question,CELL(row,col).show);
            }
            fprintf(fp,"\n");
        }
        if (CELL(r,c).show == ' ')
                    CELL(r,c).show = 's';
                fflush(fp);         //TODO later
                fprintf(fp,"%d %d %d %d %d %c\n",CELL(r,c).discovered,CELL(r,c).mined,CELL(r,c).number,CELL(r,c).flagged,CELL(r,c).question,CELL(r,c).show);


    }
}

void load() {
    FILE* fp = fopen("save.txt","rt");

    if(fp==NULL)
        puts("Error loading file.");
    else{

        double loadedTimePassed; int loadedinitialOpen;
        fscanf(fp,"%d %d %lf %d %d %d %d",&r,&c,&loadedTimePassed,&moves,&flags,&questions,&loadedinitialOpen);
        int row, col;
        for(row =0; row<r; row++){
            for(col=0; col<c; col++){
                fscanf(fp,"%d %d %d %d %d %c",&CELL(row,col).discovered,&CELL(row,col).mined,&CELL(row,col).number,&CELL(row,col).flagged,&CELL(row,col).question,&CELL(row,col).show);
                if (CELL(row,col).show == 's')
                    CELL(row,col).show = ' ';
            }
        }
    play(loadedTimePassed,loadedinitialOpen);
    }

}
void textColor(char * ForeColor, char* BackColor){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

    int f=0,b=0;

    if (ForeColor == "DEFAULT") f = 7;
    else if (ForeColor == "BLACK") f = 0;
    else if (ForeColor == "DARK_BLUE") f = 1;
    else if (ForeColor == "GREEN") f = 2;
    else if (ForeColor == "NAVY") f = 3;
    else if(ForeColor == "DARK_RED") f = 4;
    else if (ForeColor == "MAGENTA") f = 5;
    else if (ForeColor == "YELLOW") f = 6;
    else if (ForeColor == "GREY") f = 8;
    else if (ForeColor == "BLUE") f = 9;
    else if (ForeColor == "LIGHT_GREEN") f = 10;
    else if (ForeColor == "CYAN") f = 11;
    else if (ForeColor == "RED") f = 12;
    else if (ForeColor == "LIGHT_MAGENTA") f = 13;

    if (BackColor == "DEFAULT") b=0;
    if (BackColor == "GREY") b=128;
    else if (BackColor == "BLUE") b=16;
    else if (BackColor == "GREEN") b=32;
    else if (BackColor == "RED") b=64;

    SetConsoleTextAttribute(hConsole, f|b);

    return ;
}
