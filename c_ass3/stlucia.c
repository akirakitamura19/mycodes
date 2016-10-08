#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include "stlucia.h"

typedef enum {
    NORMALEND = 0,
    WRONGNUM = 1,
    INVALIDSCORE = 2,
    UNABLEROLLFILE = 3,
    INVALIDROLLS = 4,
    ERRORPIPE = 5,
    PLAYERQUIT = 6,
    INVALIDMES = 7,
    INVALIDREQ = 8,
    CATCHSIGINT = 9
} Err;

Err show_err(Err e);

// Show correct error message depending on the situation
Err show_err(Err e) {
    const char* estring = "";
    switch (e) {
        case NORMALEND:
            estring = "";
            break;
        case WRONGNUM:
            estring = "Usage: stlucia rollfile winscore prog1 prog2 [prog3 [prog4]]";
            break;
        case INVALIDSCORE:
            estring = "Invalid score";
            break;
        case UNABLEROLLFILE:
            estring = "Unable to access rollfile";
            break;
        case INVALIDROLLS:
            estring = "Error reading rolls";
            break;
        case ERRORPIPE:
            estring = "Unable to start subprocess";
            break;
        case PLAYERQUIT:
            estring = "Player quit";
            break;
        case INVALIDMES:
            estring = "Invalid message received from player";
            break;
        case INVALIDREQ:
            estring = "Invalid request by player";
            break;
        case CATCHSIGINT:
            estring = "SIGINT caught";
            break;
    }
    fprintf(stderr, "%s\n", estring);
    return e;
}

// Load file if it is valid file, otherwise error message pop up
char* load_file(char** argv) {
    FILE* file;
    char *result = malloc(sizeof(char) * 2000);
    int next = 0;
    int position = 0;
    // Open file
    file = fopen(argv[1], "r");
    if (file == NULL) {
        show_err(UNABLEROLLFILE);
        exit(3);
    }
    // Read the contents
    while (1) {
        next = fgetc(file);
        char c = next;
        if (next == EOF) {
            result[position] = '\0';
            return result;
        }else {
            result[position++] = (char)next;
        }
        if ((c != '1') && (c != '2') && (c != '3') 
                && (c != 'H') && (c != 'A') && (c != 'P') && (c != '\n')) {
            show_err(INVALIDROLLS);
            exit(4);
        }  
    }
}

// Get index of pipe

int get_index(char playerid, int direction, int rw) {
    int id = playerid - 65;
    return 4 * id + 2 * direction + rw;
}

// Set up pipes to connect with players
int setup_pipe(char** argv, int argc, struct StluciaInfo *infoBox) {
//    int fd1[2], fd2[2];
    int pid;
//    FILE* output;
    
    // Create pipes for 2 * playerNum
    int* collectionNums;
    collectionNums = malloc(sizeof(int) * (infoBox->playerNum * 4));
    for (int i = 0; i < (4 * infoBox->playerNum); i += 2) {


        if (pipe(&collectionNums[i]) == -1) {
            show_err(ERRORPIPE);
            exit(5);
        }
    }
    infoBox->collectionNums = collectionNums;

    for(char playerid = 'A'; playerid < infoBox->playerNum + 'A'; playerid++) {
        pid = fork();
        infoBox->playerid = playerid;
        if (pid == -1) {
            show_err(ERRORPIPE);
            exit(5);
        }
        if (pid == 0) {
            break;
        }
    }

    if(pid == 0) {
        // child
        // close (first pipe)
        int direction = 0;
        int rw = 1;
    
        close(collectionNums[get_index(infoBox->playerid, direction, rw)]);
        // dup2
        rw = 0;
        dup2(collectionNums[get_index(infoBox->playerid, direction, rw)], STDIN_FILENO);
        // close
        close(collectionNums[get_index(infoBox->playerid, direction, rw)]);

        // close (second pipe)
        direction = 1;
        rw = 0;
        close(collectionNums[get_index(infoBox->playerid, direction, rw)]);
        // dup2
        rw = 1;
        dup2(collectionNums[get_index(infoBox->playerid, direction, rw)], STDOUT_FILENO);
        // close
        close(collectionNums[get_index(infoBox->playerid, direction, rw)]);
        // dev null and dup2
        int devNull = open("/dev/null", O_WRONLY);
        dup2(devNull, STDERR_FILENO); 

        // execl
        char* s1 = malloc(sizeof(char) * 20);
        char* s2 = malloc(sizeof(char) * 20);
        sprintf(s1, "%d", infoBox->playerNum);
        sprintf(s2, "%c", infoBox->playerid);
        int id = infoBox->playerid - 65;
        execl(argv[id + 3], argv[id + 3], s1, s2, NULL);
        close(collectionNums[get_index(id + 3, direction, rw)]);
        exit(5);
    } else {
        // parent
        int direction = 0;
        int rw = 0;
        close(collectionNums[get_index(infoBox->playerid, direction, rw)]);
        direction = 1;
        rw = 1;
        close(collectionNums[get_index(infoBox->playerid, direction, rw)]);

        // Get ! from each players
        for(char playerid = 'A'; playerid < infoBox->playerNum + 'A'; playerid++) {
            direction = 1;
            rw = 0;
            char exclamation;
            FILE* thevoid = fdopen(collectionNums[get_index(playerid, direction, rw)], "r");
            exclamation = fgetc(thevoid);
            if (exclamation != '!') {
                show_err(ERRORPIPE);
                exit(5);
            }
        }
        return 0;
   }
    return 0; 
}

// Create struct to collect information as needed
int create_info_box(int argc, char** argv, struct StluciaInfo *infoBox, char* fileContent) {
    infoBox->playerNum = argc - 3;
    for (int i = 0; i < 26; i++) {
        if (i < argc - 3) {
            infoBox->statusChecker[i] = 1;
            infoBox->healthChecker[i] = 10;
        } else {
            infoBox->statusChecker[i] = 0;
            infoBox->healthChecker[i] = 0;
        }
        infoBox->pointChecker[i] = 0;
        infoBox->tokenChecker[i] = 0;
        infoBox->stluciaChecker[i] = 0;
        infoBox->previousTurnPoint[i] = 0;
    }
    infoBox->fileContents = fileContent;
    infoBox->p1 = fileContent;
    int goalPoint = 0;
    sscanf(argv[2], "%d", &goalPoint);
    infoBox->goalPoint = goalPoint;
    return 0;
}

// Process for deciding dices and accounce the result to every player
char* dice_process(int index, struct StluciaInfo* infoBox) {
    char* d1, * d2, * d3, * d4, * d5, * d6;
    char* dices = malloc(sizeof(char) * 6);
    char* arrangedDices = malloc(sizeof(char) * 6);
    award_point(index, infoBox);
    d1 = decide_dice(infoBox);
    ++(infoBox->p1);
    d2 = decide_dice(infoBox);
    ++(infoBox->p1);
    d3 = decide_dice(infoBox);
    ++(infoBox->p1);
    d4 = decide_dice(infoBox);
    ++(infoBox->p1);
    d5 = decide_dice(infoBox);
    ++(infoBox->p1);
    d6 = decide_dice(infoBox);
    ++(infoBox->p1);
    sprintf(dices, "%c%c%c%c%c%c", *d1, *d2, *d3, *d4, *d5, *d6);
    arrangedDices = arrange_dice(dices);
    send_turn_mes(index, arrangedDices, infoBox);
    while (1) {
        char* replay = malloc(sizeof(char) * 20);
//        if (infoBox->statusChecker[index] == 0) {
//            return arrangedDices;
//        }
        replay = recieve_replay(index, infoBox);
//    fprintf(stderr, "........2\n");
        int messageChecker = check_replay(replay);
        if (messageChecker == 1) {
            break;
        } else if (messageChecker == 2) {
            char* s1 = malloc(sizeof(char) * 10);
            char* s2 = malloc(sizeof(char) * 10);
            sscanf(replay, "%s %s", s1, s2);
            dices = reroll_dice(dices, s2, infoBox);
            arrangedDices = arrange_dice(dices);
            send_rerolled_mes(index, arrangedDices, infoBox);
        } else if ((messageChecker == 3) || (messageChecker == 4)) {
            show_err(INVALIDREQ);
            exit(8);
        } else {
            show_err(INVALIDMES);
            exit(7);
        }
    }
    announce_rolled_mes(index, arrangedDices, infoBox);
    char id = 'A' + index; 
    fprintf(stderr, "Player %c rolled %s\n", id, arrangedDices);
    return arrangedDices;
}

// Gain 2 points if the player is inside of Stlucia
int award_point(int index, struct StluciaInfo* infoBox) {
    if (infoBox->stluciaChecker[index] == 1) {
        infoBox->pointChecker[index] += 2;
    }
    return 0;
}

// Get a dice from dice file
char* decide_dice(struct StluciaInfo* infoBox) {
    while (1) {
        if (*(infoBox->p1) == '\0') {
            infoBox->p1 = infoBox->fileContents;
        } else if (*(infoBox->p1) == '\n') {
            ++(infoBox->p1);
        } else {
            return infoBox->p1;
        }
    }
}

// Arrange the dice to order
char* arrange_dice(char* dices) {
    int oneCounter = 0;
    int twoCounter = 0;
    int threeCounter = 0;
    int hCounter = 0;
    int aCounter = 0;
    int pCounter = 0;

//fprintf(stderr, "dices = %s\n", dices);
    for (int i = 0; i < 6; i++) {
        if (dices[i] == '1') {
            oneCounter++;
        } else if (dices[i] == '2') {
            twoCounter++;
        } else if (dices[i] == '3') {
            threeCounter++;
        } else if (dices[i] == 'A') {
            aCounter++;
        } else if (dices[i] == 'H') {
            hCounter++;
        } else if (dices[i] == 'P') {
            pCounter++;
        }
    }
    char* finalDices = malloc(sizeof(char) * 6);
    int counterBox[6] = {oneCounter, twoCounter, threeCounter, hCounter, aCounter, pCounter};
    char counterCBox[6] = "123HAP";
    for (int i = 0; i < 6; i++) {
        if (counterBox[i] != 0) {
            for (int j = 0; j < counterBox[i]; j++) {
                sprintf(finalDices, "%s%c", finalDices, counterCBox[i]);
            }
        }
    }
    return finalDices;
}

// Send turn message to the exact player
int send_turn_mes(int index, char* arrangedDices, struct StluciaInfo* infoBox) {
    char playerid = 'A' + index;
    int direction = 0;
    int rw = 1;
    FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
    fprintf(thevoid, "turn %s\n", arrangedDices);
    fflush(thevoid);
    return 0;
}

// Send rerolled message to the exact player
int send_rerolled_mes(int index, char* arrangedDices, struct StluciaInfo* infoBox) {
    char playerid = 'A' + index;
    int direction = 0;
    int rw = 1;
    FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
    fprintf(thevoid, "rerolled %s\n", arrangedDices);
    fflush(thevoid);
    return 0;
}

// Announce rolled message to every player
int announce_rolled_mes(int index, char* arrangedDices, struct StluciaInfo* infoBox) {
    char id = 'A' + index;
    int direction = 0;
    int rw = 1;
    infoBox->statusChecker[index] = 0;
    for (int i = 0; i < 26; i++) {
        if (infoBox->statusChecker[i] == 1) {
            char playerid = 'A' + i;
            FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
            fprintf(thevoid, "rolled %c %s\n", id, arrangedDices);
            fflush(thevoid);
        }
    }    
    infoBox->statusChecker[index] = 1;
    return 0;
}



// Recieve turn replay from the exact player
char* recieve_replay(int index, struct StluciaInfo* infoBox){
    char playerid = 'A' + index;
    int direction  = 1;
    int rw = 0;
    FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "r");
    char* message = malloc(sizeof(char) * 20);
//    fscanf(thevoid, "%s", message);
    fgets(message, 20, thevoid);
//    fprintf(stderr, "recieve message is %s\n", message);
    return message;
}

// Check the replay from player
int check_replay(char* replay) {
    char* s1 = malloc(sizeof(char) * 10);
    char* s2 = malloc(sizeof(char) * 10);
    char* s3 = malloc(sizeof(char) * 10);
    int successNum = sscanf(replay, "%s %s %s", s1, s2, s3);
    if ((successNum == 1) && ((strcmp(s1, "keepall")) == 0)) {
        return 1;
    } else if ((successNum == 2) && ((strcmp(s1, "reroll")) == 0) && (strlen(s2) <= 6)) {
        for (int i = 0; i < strlen(s2); i++) {
            if ((s2[i] != '1') && (s2[i] != '2') && (s2[i] != '3') 
                    && (s2[i] != 'A') && (s2[i] != 'H') && (s2[i] != 'P')) {
                show_err(INVALIDMES);
                exit(7);
            } else {
                return 2;
            }
        }
    } else if ((successNum == 1) && ((strcmp(s1, "stay")) == 0)) {
        return 3;
    } else if ((successNum == 1) && ((strcmp(s1, "go")) == 0)) {
        return 4;
    }
    return 0; 
}

// Get reroll dices
char* reroll_dice(char* dices, char* s2, struct StluciaInfo* infoBox) {
    char* rerollDices = malloc(sizeof(char) * 10);
    char* s = malloc(sizeof(char) * 5);
    for (int i = 0; i < 6; i++) {
        int c = dices[i];
        if (strchr(s2, c) == NULL) {
            sprintf(s, "%c", dices[i]);
            strcat(rerollDices, s);
        }
    }
//    fprintf(stderr, "rerollDices = %s\n", rerollDices);
//    exit(0);
    int rerollNum = 6 - strlen(rerollDices);
    for (int i = 0; i < rerollNum; i++) {
        sprintf(s, "%c", *decide_dice(infoBox));
        ++(infoBox->p1);
        strcat(rerollDices, s);
    }
//    fprintf(stderr, "!!rerollDices = %s\n", rerollDices);
//    exit(0);
    return rerollDices;
}
            
// Healing process
int healing_process(int index, struct StluciaInfo* infoBox, char* rolledDices) {
    int hCounter = 0;
    if (infoBox->stluciaChecker[index] == 1) {
        return 0;
    }
    for (int i = 0; i < 6; i++) {
        if (rolledDices[i] == 'H') {
            hCounter++;
        }
    }
    if (hCounter == 0) {
        return 0;
    }
    if (infoBox->healthChecker[index] + hCounter > 10) {
        hCounter = 10 - infoBox->healthChecker[index];
    }
    infoBox->healthChecker[index] = infoBox->healthChecker[index] + hCounter;
    char id = 'A' + index;
    fprintf(stderr, "Player %c healed %d, health is now %d\n", id, hCounter, infoBox->healthChecker[index]);
    return 0;
}

// Attack process
int attack_process(int index, struct StluciaInfo* infoBox, char* rolledDices) {
    int aCounter = 0;
    for (int i = 0; i < 6; i++) {
        if (rolledDices[i] == 'A') {
            aCounter++;
        }
    }
    if (aCounter == 0) {
        return 0;
    }
    for (int i = 0; i < 26; i++) {
        if (infoBox->stluciaChecker[i] == 1) {
            if (i == index) {
                // Attack outside
                attack_outside(aCounter, infoBox);
                for (int j = 0; j < 26; j++) {
                    if ((infoBox->statusChecker[j] == 1) && (infoBox->stluciaChecker[j] == 0)) {
                        char damagedID = 'A' + j;
                        if (infoBox->healthChecker[j] < 0) {
                            int newAttack = aCounter + infoBox->healthChecker[j];
                            infoBox->healthChecker[j] = 0;
                            fprintf(stderr, "Player %c took %d damage, health is now %d\n", damagedID, newAttack, infoBox->healthChecker[j]);
                        } else {
                            fprintf(stderr, "Player %c took %d damage, health is now %d\n", damagedID, aCounter, infoBox->healthChecker[j]);
                        }
                    }
                }
                announce_attacks_outside_mes(index, aCounter, infoBox);
                return 0;
            } else {
                // Attack inside
                attack_inside(aCounter, infoBox);
                for (int j = 0; j < 26; j++) {
                    if ((infoBox->statusChecker[j] == 1) && (infoBox->stluciaChecker[j] == 1)) {
                        char damagedID = 'A' + j;
                        if (infoBox->healthChecker[j] < 0) {
                            int newAttack = aCounter + infoBox->healthChecker[j];
                            infoBox->healthChecker[j] = 0;
                            fprintf(stderr, "Player %c took %d damage, health is now %d\n", damagedID, newAttack, infoBox->healthChecker[j]);
                        } else {
                            fprintf(stderr, "Player %c took %d damage, health is now %d\n", damagedID, aCounter, infoBox->healthChecker[j]);
                        }
                    }
                }
                    announce_attacks_inside_mes(index, aCounter, infoBox);
                    announce_stay_mes(infoBox);
                    char* replay = malloc(sizeof(char) * 20);
                    replay = recieve_replay2(infoBox);
                    int messageChecker = check_replay(replay);
                    
                    if ((messageChecker == 1) || (messageChecker == 2)) {
                        show_err(INVALIDREQ);
                        exit(8);
                    } else if (messageChecker == 3) {
                        // got "stay" message so do nothing
                    } else if (messageChecker == 4) {
                        // got "go" message
                        for (int i = 0; i < 26; i++) {
                            if (infoBox->stluciaChecker[i] == 1) {
                                infoBox->stluciaChecker[i] = 0;
                            }
                        }
                        infoBox->stluciaChecker[index] = 1;
                        announce_claim_mes(index, infoBox);
                        infoBox->pointChecker[index]++;
                        char id = 'A' + index;
                        fprintf(stderr, "Player %c claimed StLucia\n", id);
                    } else {
                        show_err(INVALIDMES);
                        exit(7);
                    }
                    return 0;
                }
            }
        }
        // In case that no one is inside of stlucia
        announce_claim_mes(index, infoBox);
        infoBox->stluciaChecker[index] = 1;
        infoBox->pointChecker[index]++;
        char id = 'A' + index;
        fprintf(stderr, "Player %c claimed StLucia\n", id);
        return 0;
    }

    // Announce claim message to every player
    int announce_claim_mes(int index, struct StluciaInfo* infoBox) {
        char id = 'A' + index;
        int direction = 0;
        int rw = 1;
        for (int i = 0; i < 26; i++) {
            if (infoBox->statusChecker[i] == 1) {
                char playerid = 'A' + i;
                FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
                fprintf(thevoid, "claim %c\n", id);
                fflush(thevoid);
            }
        }    
        return 0;
    }

    // Attack outside
    int attack_outside(int aCounter, struct StluciaInfo* infoBox) {
        for (int i = 0; i < 26; i++) {
            if ((infoBox->statusChecker[i] == 1) && (infoBox->stluciaChecker[i] == 0)) {
                infoBox->healthChecker[i] -= aCounter;
            }
        }
        return 0;
    }

    // Attack inside
    int attack_inside(int aCounter, struct StluciaInfo* infoBox) {
        for (int i = 0; i < 26; i++) {
            if ((infoBox->statusChecker[i] == 1) && (infoBox->stluciaChecker[i] == 1)) {
                infoBox->healthChecker[i] -= aCounter;
            }
        }
        return 0;
    }


    // Announce attacks outside message to every player
    int announce_attacks_outside_mes(int index, int aCounter, struct StluciaInfo* infoBox) {
        char id = 'A' + index;
        int direction = 0;
        int rw = 1;
        for (int i = 0; i < 26; i++) {
            if (infoBox->statusChecker[i] == 1) {
                char playerid = 'A' + i;
                FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
                fprintf(thevoid, "attacks %c %d out\n", id, aCounter);
                fflush(thevoid);
            }
        }    
        return 0;
    }

    // Announce attacks inside message to every player
    int announce_attacks_inside_mes(int index, int aCounter, struct StluciaInfo* infoBox) {
        char id = 'A' + index;
        int direction = 0;
        int rw = 1;
        for (int i = 0; i < 26; i++) {
            if (infoBox->statusChecker[i] == 1) {
                char playerid = 'A' + i;
                FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
                fprintf(thevoid, "attacks %c %d in\n", id, aCounter);
                fflush(thevoid);
            }
        }    
        return 0;
    }

    // Announce stay message to the player inside of stlucia
    int announce_stay_mes(struct StluciaInfo* infoBox) {
        int direction = 0;
        int rw = 1;
        for (int i = 0; i < 26; i++) {
            if ((infoBox->statusChecker[i] == 1) && (infoBox->stluciaChecker[i] == 1)) {
                char playerid = 'A' + i;
                FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
                fprintf(thevoid, "stay?\n");
                fflush(thevoid);
            }
        }    
        return 0;
    }

    // Recieve turn replay from the exact player
    char* recieve_replay2(struct StluciaInfo* infoBox) {
        int index = 0;
        for (int i = 0; i < 26; i++) {
            if (infoBox->stluciaChecker[i] == 1) {
                index = i;
            }
        }       
        char playerid = 'A' + index;
        int direction  = 1;
        int rw = 0;
        char* message = malloc(sizeof(char) * 20);
        if(infoBox->statusChecker[index] == 1) {
            FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "r");
    //    fscanf(thevoid, "%s", message);
            fgets(message, 20, thevoid);
    //    fprintf(stderr, "recieve message is %s\n", message);
            }
        return message;
    }

    // points process
    int points_process(int index, struct StluciaInfo* infoBox, char* rolledDices) {
        add_points(index, infoBox, rolledDices);
        announce_points_mes(index, infoBox, rolledDices);

        return 0;
    }

    // Add points to the player depending on the dices
    int add_points(int index, struct StluciaInfo* infoBox, char* rolledDices) {
        int oneCounter = 0;
        int twoCounter = 0;
        int threeCounter = 0;
        int pCounter = 0;

    //fprintf(stderr, "dices = %s\n", rolledDices);
        for (int i = 0; i < 6; i++) {
            if (rolledDices[i] == '1') {
                oneCounter++;
            } else if (rolledDices[i] == '2') {
                twoCounter++;
            } else if (rolledDices[i] == '3') {
                threeCounter++;
            } else if (rolledDices[i] == 'P') {
                pCounter++;
            }
        }
        if (oneCounter > 2) {
            infoBox->pointChecker[index] += oneCounter - 2;
        }
        if (twoCounter > 2) {
            infoBox->pointChecker[index] += twoCounter - 1;
        }
        if (threeCounter > 2) {
            infoBox->pointChecker[index] += threeCounter;
        }
        if (pCounter > 0) {
            infoBox->tokenChecker[index] += pCounter;
            if (infoBox->tokenChecker[index] >= 10) {
                infoBox->pointChecker[index]++;
                infoBox->tokenChecker[index] -= 10;
            }
        }
        return 0;
    }


    // Announce attacks outside message to every player
    int announce_points_mes(int index, struct StluciaInfo* infoBox, char* rolledDices) {
        char id = 'A' + index;
        int direction = 0;
        int rw = 1;
        int points = infoBox->pointChecker[index] - infoBox->previousTurnPoint[index];
        if (points > 0) {
            for (int i = 0; i < 26; i++) {
                if (infoBox->statusChecker[i] == 1) {
                    char playerid = 'A' + i;
                    FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
                    fprintf(thevoid, "points %c %d\n", id, points);
                    fflush(thevoid);
                }
            }
            fprintf(stderr, "Player %c scored %d for a total of %d\n", id, points, infoBox->pointChecker[index]);
        }
        infoBox->previousTurnPoint[index] = infoBox->pointChecker[index];
        return 0;
    }

    // elimination process
    int elimination_process(struct StluciaInfo* infoBox) {
        int index = 0;
        for (int i = 0; i < 26; i++) {
            if ((infoBox->healthChecker[i] <= 0) && (infoBox->statusChecker[i] == 1)) {
                index = i;
                announce_eliminated_mes(index, infoBox);
                infoBox->statusChecker[i] = 0;
            }
        }
        return 0;
    }

    // Announce eliminated message to every player if a player has 0 health
    int announce_eliminated_mes(int index, struct StluciaInfo* infoBox) {
        char id = 'A' + index;
        int direction = 0;
        int rw = 1;
        for (int i = 0; i < 26; i++) {
            if (infoBox->statusChecker[i] == 1) {
                char playerid = 'A' + i;
                FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
                fprintf(thevoid, "eliminated %c\n", id);
                fflush(thevoid);
            }
        }
        return 0;
    }

    // Announce winner message to remaining player
    int announce_winner_mes(int index, struct StluciaInfo* infoBox) {
        char id = 'A' + index;
        int direction = 0;
        int rw = 1;
        for (int i = 0; i < 26; i++) {
            if (infoBox->statusChecker[i] == 1) {
                char playerid = 'A' + i;
                FILE* thevoid = fdopen(infoBox->collectionNums[get_index(playerid, direction, rw)], "w");
                fprintf(thevoid, "winner %c\n", id);
                fflush(thevoid);
            }
        }
        return 0;
    }



    // test gameover process
    int test_gameover_process(struct StluciaInfo* infoBox) {
        int index = 0;
        int alivePlayer = 0;
        for (int i = 0; i < 26; i++) {
            if (infoBox->pointChecker[i] >= infoBox->goalPoint) {
                index = i;
                char id = index + 65;
                announce_winner_mes(index, infoBox);
                fprintf(stderr, "Player %c wins\n", id);
                exit(0);
            }
            if (infoBox->statusChecker[i] == 1) {
                alivePlayer++;
            }
        }
        if (alivePlayer == 1) {
            for (int i = 0; i < 26; i++) {
                if (infoBox->statusChecker[i] == 1) {
                    index = i;
                    char id = index + 65;
                    announce_winner_mes(index, infoBox);
                    fprintf(stderr, "Player %c wins\n", id);
                    exit(0);
                }
            }
        }
        return 0;
    }




    int main(int argc, char** argv) {
        // Check the numbers of arguments
        if (argc < 5) {
            return show_err(WRONGNUM);
            exit(1);
        }
        // Load and check file
        char* fileContent = malloc(sizeof(char) * 1000);
        fileContent = load_file(argv);
    //fprintf(stderr, "dice: %s", fileContent);
        // Check the points to play to
        int successNum = 0;
        int numPoint = 0;
        successNum = sscanf(argv[2], "%d", &numPoint);
        if ((successNum != 1) || (numPoint < 1)) {
            show_err(INVALIDSCORE);
            exit(2);
        }

        // Create struct to collect information
        struct StluciaInfo *infoBox = malloc(sizeof(struct StluciaInfo));
        create_info_box(argc, argv, infoBox, fileContent);

        // Make pipes to connect to player
        if (setup_pipe(argv, argc, infoBox)) {
            exit(5);
        }

        // Start game
        while (1) {
            for (int index = 0; index < infoBox->playerNum; index++) {
                if(infoBox->statusChecker[index] == 1) {
    //                fprintf(stderr, "index = %d\n", index);
    //                fprintf(stderr, ".....0\n");
                    char* rolledDices = dice_process(index, infoBox);
    //                fprintf(stderr, ".....1\n");
                    healing_process(index, infoBox, rolledDices);
    //                fprintf(stderr, ".....2\n");
                    attack_process(index, infoBox, rolledDices);
    //                fprintf(stderr, ".....3\n");
                    points_process(index, infoBox, rolledDices);
    //                fprintf(stderr, ".....4\n");
                    elimination_process(infoBox);
//                fprintf(stderr, ".....5\n");
                test_gameover_process(infoBox);
//                fprintf(stderr, ".....6\n");
            }
        }
    }
    return 0;
}



