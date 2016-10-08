#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "player.h"


// Decide the action how the player will work
int take_action_of_each_player(struct PlayerInfo *infoBox
        , char *buffer, char *buffer2) {
    char *firstWord = malloc(sizeof(char) * 20);
    char *secondWord = malloc(sizeof(char) * 20);
    int index = infoBox->playerID - 65;
    char newSecondWord[7] = "", s[7] = "";
    sscanf(buffer, "%s %s", firstWord, secondWord);
    for (int i = 0; i < strlen(secondWord); i++) {
        if (secondWord[i] == '1') {
        } else if (secondWord[i] == '2') {
        } else if (secondWord[i] == '3') {
        } else if (secondWord[i] == 'H') {
        } else if (secondWord[i] == 'A') {
            sprintf(s, "%c", secondWord[i]);
            strcat(newSecondWord, s);
        } else if (secondWord[i] == 'P') {
        }
    }
    if ((strlen(newSecondWord) == 0) || (infoBox->healthChecker[index] >= 5) 
            || (infoBox->count_reroll == 2)) {
        strcpy(buffer2, "keepall\n");
        infoBox->count_reroll = 0;
        int hCounter = 0;
        if (infoBox->stluciaChecker[index] == 0) {
            for (int i = 0; i < 6; i++) {
                if (secondWord[i] == 'H') {
                    hCounter++;
                }
            }
            if (hCounter > 0) {
                if (infoBox->healthChecker[index] + hCounter > 10) {
                    hCounter = 10 - infoBox->healthChecker[index];
                }
                infoBox->healthChecker[index] 
                        = infoBox->healthChecker[index] + hCounter;
            }
        }
    } else {
        sprintf(buffer2, "%s %s\n", "reroll", newSecondWord);
        infoBox->count_reroll++;
    }
    return 0;
}


// Decide the action when the player got stay? message from StLucia
int take_action_of_each_player2(struct PlayerInfo *infoBox, char *buffer2) {
    int id = infoBox->playerID - 65;
    int alivePlayer = 0;
    for (int i = 0; i < 26; i++) {
        if (infoBox->statusChecker[i] == 1) {
            alivePlayer++;
        }
    }
    if ((infoBox->healthChecker[id] < 4) && (alivePlayer > 2)) {
        sprintf(buffer2, "%s\n", "go");
        infoBox->stluciaChecker[id] = 0;
        return 0;
    } else {        
        sprintf(buffer2, "%s\n", "stay");
        return 0;
    }
    return 0;
}




