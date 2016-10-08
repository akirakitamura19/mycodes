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
    int numA = 0;
    for (int i = 0; i < strlen(secondWord); i++) {
        if (secondWord[i] == 'A') {
            numA++;
        }
    }
    for (int i = 0; i < strlen(secondWord); i++) {
        if (secondWord[i] == '1') {
            sprintf(s, "%c", secondWord[i]);
            strcat(newSecondWord, s);
        } else if (secondWord[i] == '2') {
            sprintf(s, "%c", secondWord[i]);
            strcat(newSecondWord, s);
        } else if (secondWord[i] == '3') {
            sprintf(s, "%c", secondWord[i]);
            strcat(newSecondWord, s);
        } else if (secondWord[i] == 'H') {
            sprintf(s, "%c", secondWord[i]);
            strcat(newSecondWord, s);
        } else if (secondWord[i] == 'A') {
            int checker = 0;
            for (int j = 0; j < 26; j++) {
                if (index == j) {
                } else if ((infoBox->stluciaChecker[j] == 1) 
                        && (numA >= infoBox->healthChecker[j])) {
                    checker = 1;
                }
            }
            if (checker != 1) {
                sprintf(s, "%c", secondWord[i]);
                strcat(newSecondWord, s);
            }
        } else if (secondWord[i] == 'P') {
            if (infoBox->stluciaChecker[index] == 0) {
                sprintf(s, "%c", secondWord[i]);
                strcat(newSecondWord, s);
            }
        }
    }
    if ((strlen(newSecondWord) == 0) || (infoBox->count_reroll == 2)) {
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
    sprintf(buffer2, "%s\n", "stay");
    return 0;
}




