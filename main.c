#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cells.h"

bool startup(void);

int r,c;
struct cell grid[100][100] ;

int main(void){

    if (startup() )
        return 0;

    return 0;
}

bool startup(void){

    do {
    printf("Do you want to start a new game (n), load a previous game (l) or exit (x)?\n");
    char input;
    fflush(stdin);
    scanf("%c",&input);
    switch(input){

    case 'n': {getSize(); gridInit(); draw(); break; }
    case 'l': {/*load();*/ break; }
    case 'x': return 1; break;
    default : {
    printf("Invalid entry, please try again\n"); break;
    }

    }

    } while(1) ;
}
