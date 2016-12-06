#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cells.h"
#include "game.h"
bool gameState=1;
bool play() {

int row, col;
char action;

do {
        draw();
        puts("Please enter your move in the form of ( row col action )");
        scanf("%d %d %c", &row, &col, &action);

} while (gameState);
}

bool startup(void){

    do {
    printf("Do you want to start a new game (n), load a previous game (l) or exit (x)?\n");
    char input;
    fflush(stdin);
    scanf("%c",&input);
    switch(input){

    case 'n': {getSize(); gridInit(); play(); break; }
    case 'l': {/*load();*/ break; }
    case 'x': return 1; break;
    default : {
    printf("Invalid entry, please try again\n"); break;
    }

    }

    } while(1) ;
}


