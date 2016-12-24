#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <pthread.h>

#include "cells.h"
#include "game.h"
#include "scores.h"

#define CELL(row,col) grid[row][col]
#define IDLE_TIME 60


char message[100];
bool messageFlag = 0;

char * titleAscii[] =  {"  __  __  _                     ____                                         ",
                   " |  \\/  |(_) _ __    ___ __/\\__/ ___|__      __ ___   ___  _ __    ___  _ __ ",
                   " | |\\/| || || '_ \\  / _ \\\\    /\\___ \\\\ \\ /\\ / // _ \\ / _ \\| '_ \\  / _ \\| '__|",
                   " | |  | || || | | ||  __//_  _\\ ___) |\\ V  V /|  __/|  __/| |_) ||  __/| |   ",
                   " |_|  |_||_||_| |_| \\___|  \\/  |____/  \\_/\\_/  \\___| \\___|| .__/  \\___||_|   ",
                   "                                                          |_|                "};

char * wonAscii[] =  {  " __     __            __          __ _         _ ",
                        " \\ \\   / /            \\ \\        / /(_)       | |",
                        "  \\ \\_/ /___   _   _   \\ \\  /\\  / /  _  _ __  | |",
                        "   \\   // _ \\ | | | |   \\ \\/  \\/ /  | || '_ \\ | |",
                        "    | || (_) || |_| |    \\  /\\  /   | || | | ||_|",
                        "    |_| \\___/  \\__,_|     \\/  \\/    |_||_| |_|(_)"};

char * lostAscii[] =  { "   _____                            ____                    ",
                        "  / ____|                          / __ \\                   ",
                        " | |  __   __ _  _ __ ___    ___  | |  | |__   __ ___  _ __ ",
                        " | | |_ | / _` || '_ ` _ \\  / _ \\ | |  | |\\ \\ / // _ \\| '__|",
                        " | |__| || (_| || | | | | ||  __/ | |__| | \\ V /|  __/| |   ",
                        "  \\_____| \\__,_||_| |_| |_| \\___|  \\____/   \\_/  \\___||_|   "};

void printTitleAscii(){
    int titleSize = sizeof(titleAscii) / sizeof(char*);

    textColor("RED","DEFAULT");
    int i,j;
    for(i=0; i<titleSize; i++){
        for(j=0; titleAscii[i][j]!='\0' ; j++){
            putchar(titleAscii[i][j]);
            //Sleep(1);
        }
        printf("\n");
    }
    textColor("DEFAULT","DEFAULT");
    return;
}

void printWonAscii(){
    int size = sizeof(wonAscii) / sizeof(char*);

    textColor("LIGHT_GREEN","DEFAULT");
    int i,j;
    printf("\t");
    for(i=0; i<size; i++){
        for(j=0; wonAscii[i][j]!='\0' ; j++){
            putchar(wonAscii[i][j]);
            //Sleep(1);
        }
        printf("\n\t");
    }
    printf("\n");
    textColor("DEFAULT","DEFAULT");
    return;
}

void printLostAscii(){
    int size = sizeof(lostAscii) / sizeof(char*);

    textColor("RED","DEFAULT");
    int i,j;
    printf("\t");
    for(i=0; i<size; i++){
        for(j=0; lostAscii[i][j]!='\0' ; j++){
            putchar(lostAscii[i][j]);
            //Sleep(1);
        }
        printf("\n\t");
    }
    printf("\n");
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
        case 'u': return 1;
        default : return 0;
    }


}

void clearScreen(void){
    system("cls");
    return;
}

void* idleTimer(void){  //function pointer for the idle thread
    double secondsPassed = difftime(timeNow,timeStart);   //time counter initialized to current time
    while(isIdle){      //isIdle is set to true before input and set to false after input
        time(&timeNow);


        if( difftime(timeNow,timeStart) - secondsPassed >= IDLE_TIME){    //when IDLE_TIME passes
            clearScreen();
            double seconds = timePassed + difftime(timeNow,timeStart);
            printf("\n    Moves: %d\t Flags: %d\t Question Marks: %d\t Time: %2d:%2.2d\n\n",moves,flags,questions,(int)seconds/60,(int)seconds%60); //print remaining flags, difftime returns difference between two times
            draw();                 //redraw with new time
            puts("Please enter your move in the form of ( row col action )");
            secondsPassed = difftime(timeNow,timeStart);  //update time counter for the next iteration
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
        gameState = won;
    }
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
        if (flagsAround >= CELL(row,col).number)
            openEmptyCell(row, col);
        else {
            messageFlag = 1;
            strcpy(message, "Not enough flags around this cell to open.");
        }
        return;
    }
    if(CELL(row,col).flagged || CELL(row,col).question){
        messageFlag = 1;
        strcpy (message,"The cell is marked. You can unmark it first by using the action 'u'");
        return;
    }
    if (CELL(row,col).mined) {
        if(gameState == playing)
            boom = 0;
        gameState = lost;
        detonations[boom].row = row;
        detonations[boom++].col = col;
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

void win () {
    int i,j;
        for (i = 0 ; i < r ; i++) { //Flagging mines.
            for (j = 0 ; j < c ; j++) {
                if (CELL(i,j).mined)
                    CELL(i,j).show = 'F';
            }
        }
        clearScreen();
        double seconds = timePassed + difftime(timeNow,timeStart);
        printf("\n    Moves: %d\t Flags: %d\t Question Marks: %d\t Time: %2d:%2.2d\n\n",moves,flags,questions,(int)seconds/60,(int)seconds%60); //print remaining flags, difftime returns difference between two times
        draw();
        printWonAscii();
        getScore();
}
void lose() {
    int i, j;
    for (i = 0 ; i < boom ; i++) {
        CELL((detonations[i].row),(detonations[i].col)).show = '!';
    }
    //Detonated mines marked with '!'.
    for (i = 0 ; i < r ; i++) {
        for (j = 0 ; j < c ; j++) {
            if (CELL(i,j).show == '!')
                continue;
            if (CELL(i,j).flagged && !CELL(i,j).mined) {
                CELL(i,j).show = '-'; //Incorrect flag.
            }
            else if (CELL(i,j).flagged && CELL(i,j).mined)
                CELL(i,j).show = '*'; //Correctly-flagged mine.
            else if (!CELL(i,j).flagged && CELL(i,j).mined)
                CELL(i,j).show = 'M'; //Missed mine.
            else if (CELL(i,j).number) //Showing the rest of the grid.
                CELL(i,j).show = CELL(i,j).number + '0';
            else CELL(i,j).show = ' ';
        }
    }
    clearScreen();
    double seconds = timePassed + difftime(timeNow,timeStart);
    printf("\n    Moves: %d\t Flags: %d\t Question Marks: %d\t Time: %2d:%2.2d\n\n",moves,flags,questions,(int)seconds/60,(int)seconds%60); //print remaining flags, difftime returns difference between two times
    draw();
    printLostAscii();
    puts("Press any key to return to main menu.");
    getch();
}

void play(double timeAlreadyPassed,int localInitialOpen){

    time(&timeStart);
    int rowIn, colIn;
    bool wrongInput = 0;
    char action;
    initialOpen = localInitialOpen;
    timePassed = timeAlreadyPassed;
    gameState = playing;
    mines = 1+(r*c)/10;

    do
    {
    time(&timeNow); //get current time
        clearScreen();
        double seconds = timePassed + difftime(timeNow,timeStart);
        printf("\n    Moves: %d\t Flags: %d\t Question Marks: %d\t Time: %2d:%2.2d\n\n",moves,flags,questions,(int)seconds/60,(int)seconds%60); //print remaining flags, difftime returns difference between two times
        draw();
        if(wrongInput){
            puts("Wrong entry.");
            wrongInput = 0;
        }
        if (messageFlag) {
            puts(message);
            messageFlag = 0 ;
        }

        printf("Please enter your move in the form of (row column action)\
             \nActions: Open(o)  Flag(f)  Question Mark(q)  Save(s)  Exit(x)\n");

        isIdle = 1;     //launch thread loop
        pthread_t idleThread;       //pthread stuff
        pthread_create(&idleThread,NULL,idleTimer,NULL);

        rowIn = -1 ; colIn = -1; action = 'z';  //invalid values

        fflush(stdin);
        char inputString[12];
        fgets(inputString,11,stdin);
        inputString[strlen(inputString)-1] = '\0';

        if ( !strcmp(inputString,"s")  || !strcmp(inputString,"S") || !strcmp(inputString,"save") || !strcmp(inputString,"Save") ){
                save();
                isIdle = 0;
        }
        else if ( !strcmp(inputString,"x")  || !strcmp(inputString,"X") || !strcmp(inputString,"exit") || !strcmp(inputString,"Exit") ){
                char confirm;
                puts("Do you really want to quit without saving ?");
                puts("Save and exit       \t(s)");
                puts("Exit without saving \t(x)");
                puts("Cancel              \t(c)");
                fflush(stdin);
                confirm = getch();

                switch(confirm){
                    case 's':{save();}
                    case 'x':{isIdle = 0; return;}
                    default :{break;}
                }
        }
        else{
            sscanf(inputString,"%d %d %c", &rowIn, &colIn, &action); //TODO: Adjust input method for saving
            isIdle = 0;


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
        }
        moves++;
    }

    isIdle = 0;
    fflush(stdin);

    }while (gameState==playing);

    if (gameState == won) {
        win();
    }
    else
        lose();
    return;
}

void Game(void){

    int loadTrial = 0;
    do
    {
        system("MODE 80, 35");
        clearScreen();
        printTitleAscii();
        printf("\n\t1. Start a new game.    \t(n)\
               \n\t2. Load a previous game.\t(l)\
               \n\t3. Leaderboard.         \t(b)\
               \n\t4. Exit.                \t(x)");
        if(loadTrial) {printf("\n\n\t No existing game to load."); loadTrial = 0;}
        printf("\n\t Type the letter for the desired option: ");

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
            loadTrial = 1;
            break;
        }
        case 'b':
            {
                displayLeaderboard(-1);
                break;
            }
        case 'x':
            return;
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

void save(){
    FILE* fp = fopen("save.txt","w");

    if(fp==NULL)
        printf("Error opening file.\n");
    else{
        fflush(fp);
        fprintf(fp,"%d %d %f %d %d %d %d\n",r,c,timePassed+difftime(timeNow,timeStart),moves,flags,questions,initialOpen);
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
    play(loadedTimePassed, loadedinitialOpen);
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
    else if (ForeColor == "BRIGHT_WHITE") f = 15;

    if (BackColor == "DEFAULT") b=0;
    if (BackColor == "GREY") b=128;
    else if (BackColor == "BLUE") b=16;
    else if (BackColor == "GREEN") b=32;
    else if (BackColor == "RED") b=64;

    SetConsoleTextAttribute(hConsole, f|b);

    return ;
}
