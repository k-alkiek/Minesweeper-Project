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
        printf(" %3d",j); //print column numbers
    }
    printf("\n");

    for(j=0; j<c; j++){ //print first horizontal break
        if(j==0) printf("   +"); //print an extra '+' at the beginning of the first row
        printf("---+");
    }
    printf("\n");

    for (i = 0 ; i < r ; i++ ) {
            printf("%2d ", i);   //print row number
        for (j = 0 ; j < c ; j++ ) {
            if(j==0) printf("|"); //print first vertical break in the row
            printf("%2c |",grid[i][j].show); //print cell's char and a vertical break
        }
        printf("\n"); //new line after each row
        for(j=0; j<c; j++){ //horizontal break after each row
            if(j==0) printf("   +"); //print first '+'
            printf("---+");
        }
        printf("\n");
    }

}
