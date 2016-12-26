#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

struct Player{
    char name[33];
    int score;
}topPlayer[10];

void getScore(); // This function represents the main function for handling high scores. Calls “loadTopPlayers();”, calculates the score of the player, displays it, then compares whether it with the last of the top scores. If it is a high score, the function gets the name of the player and puts it in a structure along with their score. The function also calls “insertScore”, “getName”, “saveTopPlayers” and “displayLeaderboard”.
void insertScore(char* playerName,int score,int i); // Inserts a certain player’s information (playerName and score) in the leaderboard in position (i) and shifts the lower scores down the list.
long getName (int); // Reads the name of a player in the high score list. Returns the number of bytes read from the beginning of the file by “ftell(FILE*)” function in the process of loading.
void saveTopPlayers(); // Saves highscores in “Highscores.txt” .
void loadTopPlayers(); // Loads highscores from "Highscores.txt"
void displayLeaderboard(int); // This function displays the leaderboard and could be passed the position of the current player as a parameter by “getScores” in order to highlight their name when showing the leaderboard after winning and getting a high score directly.
void clearLeaderboard(); // Clears the leaderboard. The user has the ability to clear the leaderboard and start it over for normalizing the challenge of the game.
void changeTextColor(char * ForeColor, char* BackColor); // Handles color.
#endif // SCORE_H_INCLUDED
