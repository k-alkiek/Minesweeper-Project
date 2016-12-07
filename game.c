#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cells.h"
#include "game.h"

#define CELL grid[row][col]

extern int r,c;
extern struct cell grid[100][100];

bool gameState=1; //TODO: change to enum later, change in play() and openCell()

bool inRange(int max, int input){
    if(input >=0 && input < max) return 1;
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

void openCell(int row, int col) {
    //if(CELL.discovered)
    if(CELL.flagged || CELL.question){
    puts("The cell is marked. Unmark it first by the action 'u'");
    return;
    }
    if (CELL.mined) {
        gameState = 0;//TODO: change gameState to enum laeter
    }
}

bool play(){

    int rowIn, colIn;
    bool wrongInput = 0;
    char action;
    do
    {
        clearScreen(); printf("\n");
        draw();
        if(wrongInput){
            puts("Wrong entry.");
            wrongInput = 0;
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
            case 'f':       //TODO
            case 'Q':       //add
            case 'q':       //functions
            case 'U':       //later
            case 'u':
            case 'S':
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
            getSize();
            gridInit();
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


