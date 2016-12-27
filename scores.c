#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "game.h"
#include "scores.h"
#include "cells.h"


void getScore() {

    loadTopPlayers();

    char playerName[33];
    int score = r*r*r*r*c*c*c*c/moves/(timePassed + difftime(timeNow,timeStart));

    printf("\nYour score is %d\n",score);

    if(score<= topPlayer[9].score){
        puts("Press any key to return to main menu.");
        getch();
    }
    else{
        puts("You got a highscore!");
        int a=0;
        int repeat = 0;
        puts("Enter your name: ");
        do {
        if (repeat) {
            puts("Special character used. Please, enter your name again :");
            repeat = 0;
        }

        fgets(playerName,32,stdin); //Get the player's name.

        for (a = 0 ; a <= strlen(playerName) ; a++) {
            if (playerName[a] == '~') {
                repeat = 1;
                break;
            }
        }
        } while(repeat);
        //get the name then fill the rest of the string with spaces for display format purposes.
        for(a=strlen(playerName) - 1; a<31; a++){
            playerName[a] = ' ';
        }

        //playerName[strlen(playerName) - 1] = '\0';  //fgets saves \n in the string.

        playerName[31] = '\0';  //insert null at the end
        strcat(playerName, "~");                    //add colon for saving format.
        int i;
        //loadTopPlayers();

        for (i=0; i<10 ; i++) {
            if (topPlayer[i].score < score) {
                break;
            }
        }
        if (i != 10 ) {
            insertScore(playerName,score,i);
        }

        saveTopPlayers();
        displayLeaderboard(i);
    }
}

void insertScore(char* playerName,int score,int i){

    int c;
    for (c = 8 ; c >= i ; c--) {
        strcpy(topPlayer[c+1].name,topPlayer[c].name);
        topPlayer[c+1].score = topPlayer[c].score;
    }
    strcpy(topPlayer[i].name,playerName);
    topPlayer[i].score = score;
}

long getName (int i){
    char buffer[50];
    FILE * load = fopen("HighScores.txt","rt");
    int l = 2*i;
    while (l){
        fgets(buffer,100, load ) ;
        l--;
    }
    char ch;
    char name[35];
    int c = 0;
    while ( (name[c++] = ch = fgetc(load)) != '~');
    name[c] = '\0';
    strcpy(topPlayer[i].name,name);
    long tell = ftell(load);
    fclose(load);
    return tell;
}

void saveTopPlayers(){
    struct Player debugArr[11];
    int i;
    for(i=0; i<10; i++){
        strcpy(debugArr[i].name,topPlayer[i].name);
        debugArr[i].score = topPlayer[i].score;
    }

    FILE * saver = fopen("HighScores.txt","wt");
    if (saver==NULL)
        puts("Error loading scores.");
    else{
        int i;
        for (i = 0 ; i < 10 ; i++) {
            fflush(saver);
            fprintf(saver,"%s\n%d\n", topPlayer[i].name, topPlayer[i].score);
        }
    }
    fclose(saver);
}

void loadTopPlayers(){
    FILE * loader = fopen("HighScores.txt","rt");
    if (loader==NULL){
        clearLeaderboard();
        saveTopPlayers();
    }
    else{
        int i, success;
        long loaderPosition;
        for (i = 0 ; i < 10 ; i++) {
            loaderPosition = getName(i);
            fseek(loader, loaderPosition, 0);
            success = fscanf(loader, "%d", &topPlayer[i].score);
        }
    }
    fclose(loader);
}

void displayLeaderboard(int currentPlayer){

    loadTopPlayers();
    clearScreen();
    printf("\n");
    printf("\t+-------------------------------------------------------+\n");
    printf("\t|                     Leaderboard                       |\n");
    printf("\t+-------------------------------------------------------+\n");
    int i;
    for(i=0; i<10; i++){
        //printf("\t| %2d. %s\b\t\t%6d  |\n",i+1,topPlayer[i].name,topPlayer[i].score); // \b character to erase the colon
        printf("\t|");

        if(i==currentPlayer){
            changeTextColor("BRIGHT_WHITE","GREEN");
            printf(" %2d. %s\b\t\t%6d  ",i+1,topPlayer[i].name,topPlayer[i].score);
            changeTextColor("DEFAULT","DEFAULT");
        }
        else
            printf(" %2d. %s\b\t\t%6d  ",i+1,topPlayer[i].name,topPlayer[i].score);

        printf("|\n");
        printf("\t|                                                       |\n");
    }
    printf("\t+-------------------------------------------------------+\n");

    printf("\n\tPress 'c' to clear leaderboard or enter to return to main menu.\n\t");

    char action = getch();

    if (action == 'c' || action == 'C'){
        printf("Are you sure you want to clear the leaderboard? (y/n)\n\t");
        char confirm = getch();
        if(confirm == 'Y' || confirm == 'y')
            clearLeaderboard();
    }
    return;
}

void clearLeaderboard(){
    struct Player blank;
    strcpy(blank.name,"_______________________________~");
    blank.score = 0;
    int i;
    for(i=0; i<10; i++){
        topPlayer[i] = blank;
    }
    saveTopPlayers();
    return;
}

void changeTextColor(char * ForeColor, char* BackColor){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int f=0,b=0;

    if (ForeColor == "DEFAULT") f = 7;
    else if (ForeColor == "BLACK") f = 0;
    else if (ForeColor == "DARK_BLUE") f = 1;
    else if (ForeColor == "GREEN") f = 2;
    else if (ForeColor == "NAVY") f = 3;
    else if(ForeColor == "DARK_RED") f = 4;
    else if (ForeColor == "MAGENTA") f = 5;
    else if (ForeColor == "YELLOW") f = 6;
    else if (ForeColor == "GREY") f = 8;
    else if (ForeColor == "BLUE") f = 9;
    else if (ForeColor == "LIGHT_GREEN") f = 10;
    else if (ForeColor == "CYAN") f = 11;
    else if (ForeColor == "RED") f = 12;
    else if (ForeColor == "LIGHT_MAGENTA") f = 13;
    else if (ForeColor == "BRIGHT_WHITE") f = 15;

    if (BackColor == "DEFAULT") b=0;
    if (BackColor == "GREY") b=128;
    else if (BackColor == "BLUE") b=16;
    else if (BackColor == "GREEN") b=32;
    else if (BackColor == "RED") b=64;

    SetConsoleTextAttribute(hConsole, f|b);

    return ;
}
