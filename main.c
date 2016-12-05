#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cells.h"

bool startup(void);

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
    scanf("%c",&input);
    switch(input){

    case 'n': {/*game();*/ break; }
    case 'l': {/*load();*/ break; }
    case 'x': return 1;
    default : {
    printf("Invalid entry, please try again\n");
    }


    }

    } while(1) ;
}
