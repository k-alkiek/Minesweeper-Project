#include <stdio.h>
#include <stdlib.h>

#include "cells.h"

#define MAX_SIZE 100
#define CELL(row,col) grid[row][col]


int r,c; //rows and columns
struct cell grid[100][100];

void getSize() {

    do{
        // Add "Wrong entry" and clear screen
    puts("Please enter the grid dimensions in the form of : x y");
    fflush(stdin);
    scanf("%d %d", &r, &c);
    }while(r<2 || r>MAX_SIZE || c<2 || c>MAX_SIZE);
}


void gridInit(){
    int row,col;
    for(row=0; row<r; row++)
        for(col=0; col<c; col++)
            putBlank(row,col);
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
            printf("%2c |",CELL(i,j).show); //print cell's char and a vertical break
        }
        printf("\n"); //new line after each row
        for(j=0; j<c; j++){ //horizontal break after each row
            if(j==0) printf("   +"); //print first '+'
            printf("---+");
        }
        printf("\n");
    }

}


void putBlank(int row, int col){
//    grid[row][col] = { 0, 0, 0, 0, 0, 'X'};
    CELL(row,col).discovered = 0;
    CELL(row,col).mined = 0;
    CELL(row,col).number = 0;
    CELL(row,col).flagged = 0;
    CELL(row,col).question = 0;
    CELL(row,col).show = 'X';
}
