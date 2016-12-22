#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "scores.h"
#include "cells.h"



void insertScore(char* playerName,int score,int i){
    //DEBUG
    struct player debugArr[11];
    int h;
    for(h = 0; h<10; h++){
        strcpy(debugArr[h].name,topPlayer[h].name);
        debugArr[h].score = topPlayer[h].score;
    }

    int c;
    for (c = 8 ; c >= i ; c--) {
        //topPlayer[c+1] = topPlayer[c];
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
    while ( (name[c++] = ch = fgetc(load)) != ':');
    name[c] = '\0';
    strcpy(topPlayer[i].name,name);
    long tell = ftell(load);
    fclose(load);
    return tell;
}

void saveTopPlayers(){
    //DEBUG
    struct player debugArr[11];
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
    if (loader==NULL)
        puts("Error loading scores.");
    else{
        int i, success;
        long loaderPosition;
        for (i = 0 ; i < 10 ; i++) {
            loaderPosition = getName(i);
            fseek(loader, loaderPosition, 0);
            success = fscanf(loader, "%d", &topPlayer[i].score);
        }
    }
}

void getScore() {


    // FLASH TODO
    char playerName[33];
    int score = r*r*r*r*c*c*c*c/moves/(timePassed + difftime(timeNow,timeStart));
    puts("Enter your name: ");
    fgets(playerName,32,stdin);
    playerName[strlen(playerName) - 1] = '\0';  //fgets saves \n in the string
    strcat(playerName, ":");                    //add colon for saving format
    int i;
    loadTopPlayers();

    //DEBUG
    struct player debugArrAfterLoad[11];
    for(i=0; i<10; i++){
        strcpy(debugArrAfterLoad[i].name,topPlayer[i].name);
        debugArrAfterLoad[i].score = topPlayer[i].score;
    }

    for (i=0; i<10 ; i++) {
        if (topPlayer[i].score < score) {
            break;
        }
    }
    if (i != 9 ) {
        insertScore(playerName,score,i);
    }

    struct player debugArrAfterInsert[11];
    int h;
    for(h = 0; h<10; h++){
        strcpy(debugArrAfterInsert[h].name,topPlayer[h].name);
        debugArrAfterInsert[h].score = topPlayer[h].score;
    }

    saveTopPlayers();
    printf("%s\nScore : %d",topPlayer[i].name,topPlayer[i].score);
    getch();
}
