#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cells.h"

#define MAX_SIZE 100
#define CELL(row,col) grid[row][col]


int r,c,mines; //rows and columns
struct cell grid[30][30];
extern time_t timeStart;


void getSize() {

    do{
        // Add "Wrong entry" and clear screen
    puts("Please enter the grid dimensions in the form of : x y");
    fflush(stdin);
    scanf("%d %d", &r, &c);
    }while(r<2 || r>MAX_SIZE || c<2 || c>MAX_SIZE);
    mines = 1+(r*c)/10;     //get number of mines in the game
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

void putMines(){
    int minesPlaced = 0;
    srand((unsigned)timeStart);
    while(minesPlaced < mines){ //keep adding mines till it reaches the variable
        int row = rand() % r;
        int col = rand() % c;
        if(!(CELL(row,col).mined) && !(CELL(row,col).discovered) ){ //only add mine in a cell if it does not already have a mine and is not already discovered (first open)
                CELL(row,col).mined = 1;
                minesPlaced ++;
        }
    }
}

void putNumbers(){
    int row, col, i, j;
    for(row=0; row<r; row++){
        for(col=0; col<c; col++){
            if(CELL(row,col).mined){
                for(i = row-1; i<=row+1; i++){
                    for(j=col-1; j<=col+1; j++){
                        if(i<r && i>=0 && j<c && j>=0 && !(CELL(i,j).mined) ) //add numbers only to unmined cells inside the boundaries
                            (CELL(i,j).number) ++;
                    }
                }
            }
        }
    }
}
