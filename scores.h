#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

struct player{
    char name[33];
    int score;
}topPlayer[10];

void insertScore(char* playerName,int score,int i);
void getScore();
long getName (int);

#endif // SCORE_H_INCLUDED
