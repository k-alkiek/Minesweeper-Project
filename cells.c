#include "cells.h"


extern int r,c; //rows and columns
extern struct cell grid[100][100];

void getSize() {

    puts("Please enter the grid dimensions in the form of : x y");
    scanf("%d %d", &r, &c);
    printf("The game will have %d rows and %d columns\n", r, c);
}

void gridInit(){
    static int i,j;
    for(i=0; i<r; i++)
        for(j=0; j<c; j++)
            grid[i][j].show = 'X';
}

void draw() {
    static int i,j;
    printf("  ");
    for ( j = 0 ; j < c ; j++) {
        printf("%2d ",j);
    }
    printf("\n");
    for (i = 0 ; i < r ; i++ ) {
            printf("%2d", i);
        for (j = 0 ; j < c ; j++ ) {
            printf(" %c",grid[i][j].show);
        }
        printf("\n");
    }
}
