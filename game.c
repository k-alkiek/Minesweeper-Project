#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cells.h"
#include "game.h"

extern int r,c;

bool gameState=1;

bool inRange(int max, int input){
    if(input >=0 && input < max) return 1;
    else return 0;
}

void clearScreen(void){
    system("cls");
    return;
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
        scanf("%d %d %c", &rowIn, &colIn, &action);
        fflush(stdin);
        if ( !( inRange(r,rowIn) && inRange(c, colIn ) ) ){
            wrongInput = 1;
            continue;
        }
        // we still need to check the actions, which is the easiest thing ever so we won't do it now :'D

    }
    while (gameState);
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
            /*load();*/ break;
        }
        case 'x':
            return 1;
            break;
        default :
        {
            printf("Invalid entry, please try again\n");
            break;
        }

        }

    }
    while(1) ;
}


