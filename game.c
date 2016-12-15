#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "cells.h"
#include "game.h"

#define CELL(row,col) grid[row][col]

time_t timeStart;  //variable to hold time value when the game starts. initialized in startup()
bool messageFlag = 0;
char message[100];
int flags = 0 ;
extern int r,c;
extern struct cell grid[30][30];

bool gameState=1; //TODO: change to enum later, change in play() and openCell()

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

void openEmptyCell(int row, int col) {
    int i, j;
        for(i = row-1 ; i <= row+1 ; i++){
            for (j = col-1 ; j <= col+1; j++) {
                if (i<r && j < c && i>=0 && j>=0 && !CELL(i,j).discovered)
                    openCell(i,j);
            }
        }
    return;
}

void openCell(int row, int col) {
    static bool initialOpen = 1;
    if(initialOpen){
        initialOpen = 0;
        CELL(row,col).discovered = 1;
        putMines();
        putNumbers();
        if (CELL(row,col).number)
            CELL(row,col).show = (char)(48 + CELL(row,col).number);
        else
            CELL(row,col).show = ' ';
            openEmptyCell(row, col);
        return ;
    }
    //if(CELL.discovered)
    if(CELL(row,col).discovered && !(CELL(row,col).number)){
        messageFlag = 1;
        strcpy (message,"The cell is empty.");
        return;
    }
    if (CELL(row,col).discovered == 1){
        messageFlag = 1;
        strcpy(message, "The cell is already opened.");
        return;
    }
    if(CELL(row,col).flagged || CELL(row,col).question){
        messageFlag = 1;
        strcpy (message,"The cell is marked. You can unmark it first by using the action 'u'");
        return;
    }
    if (CELL(row,col).mined) {
        gameState = 0;//TODO: change gameState to enum later - LOSS/LOSE HERE
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

bool play(){

    time_t timeNow;
    int rowIn, colIn;
    bool wrongInput = 0;
    char action;
    do
    {
    time(&timeNow); //get current time
    //double seconds = difftime(timeNow,timeStart);
        clearScreen(); printf("   Time: %.f\n\n",difftime(timeNow,timeStart)); //difftime returns difference between two times
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
        rowIn = -1 ; colIn = -1; action = 'z';  //invalid values
        scanf("%d %d %c", &rowIn, &colIn, &action); //TODO: Adjust input method for saving
        fflush(stdin);
        if ( !( inRange(r,rowIn) && inRange(c, colIn) && validAction(action) ) ){
            wrongInput = 1;
            continue;
        }
        switch(action){
            case 'O':
            case 'o': {openCell(rowIn, colIn); break;}
            case 'F':
            case 'f': {flagCell(rowIn, colIn); break;}
            case 'Q':
            case '?':
            case 'q': {questionCell(rowIn, colIn); break;}
            case 'U':
            case 'u': {unmarkCell(rowIn, colIn); break;}
            case 'S':  // add functions later
            case 's': break;
    }



        // Input validation implemented like a boss B-)
    }
    while (gameState);
    return;
}

bool startup(void){

    do
    {
        printf("Do you want to start a new game (n), load a previous game (l) or exit (x)?\n");
        char input;
        fflush(stdin);
        scanf("%c",&input);
        switch(input)
        {

        case 'n':
        {
            gameState = 1;
            getSize();
            gridInit();
            time(&timeStart); //initialize timeStart as soon as the game starts
            play();
            break;
        }
        case 'l':
        {
            /*TODO: load();*/ break;
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


