#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "player.h"

// Show correct error message to players depending invalid input
Err show_err(Err e) {
    const char* estring = "";
    switch (e) {
        case NORMALEND: 
            estring = "";
            break;
        case WRONGNUM: 
            estring = "Usage: player number_of_players my_id";
            break;
        case INVALIDNUM:
            estring = "Invalid player count";
            break;
        case INVALIDPLAYER:
            estring = "Invalid player ID";
            break;
        case PIPE:
            estring = "Unexpectedly lost contact with StLucia";
            break;
        case INVALIDMESSAGE:
            estring = "Bad message from StLucia";
            break;
    }
    fprintf(stderr, "%s\n", estring);
    return e;
}

// Send a message from player to Stlucia
int send_message_to_stlucia(char* buffer) {
    fprintf(stdout, "%s", buffer);
    fflush(stdout);
    return 0;
}

// Recieve messages from stlucia to player
int recieve_message_from_stlucia(struct PlayerInfo* infoBox) {
    char* buffer = malloc(sizeof(char) * 100);
    char* buffer2 = malloc(sizeof(char) * 100);
    fgets(buffer, 100, stdin);
    char* firstWord = malloc(sizeof(char) * 20);
    sscanf(buffer, "%s ", firstWord);
    int statusChecker = 0;
    if ((strcmp(firstWord, "turn")) == 0) {
        statusChecker = print_turn_message(buffer);
        if (statusChecker == 0) {
            take_action_of_each_player(infoBox, buffer, buffer2);
            send_message_to_stlucia(buffer2);
        }
    } else if ((strcmp(firstWord, "rerolled")) == 0) {
        statusChecker = print_rerolled_message(buffer);
        if (statusChecker == 0) {
            take_action_of_each_player(infoBox, buffer, buffer2);
            send_message_to_stlucia(buffer2);
        }
    } else if ((strcmp(firstWord, "rolled")) == 0) {
        statusChecker = print_rolled_message(buffer, infoBox);
    } else if ((strcmp(firstWord, "points")) == 0) {
        statusChecker = print_points_message(buffer, infoBox);
    } else if ((strcmp(firstWord, "attacks")) == 0) {
        statusChecker = print_attacks_message(buffer, infoBox);
    } else if ((strcmp(firstWord, "eliminated")) == 0) {
        statusChecker = print_eliminated_message(buffer, infoBox);
    } else if ((strcmp(firstWord, "claim")) == 0) {
        statusChecker = print_claim_message(buffer, infoBox);
    } else if ((strcmp(firstWord, "stay?")) == 0) {
        statusChecker = print_stay_message(buffer);
        if (statusChecker == 0) {
            take_action_of_each_player2(infoBox, buffer2);
            send_message_to_stlucia(buffer2);
        }
    } else if ((strcmp(firstWord, "winner")) == 0) {
        statusChecker = print_winner_message(buffer, infoBox);
        if (statusChecker == 0) {
            exit(0);
        }
    } else if ((strcmp(firstWord, "shutdown")) == 0) {
        statusChecker = print_shutdown_message(buffer);
    } else if (firstWord == NULL || !strcmp(firstWord, "")) {
        return show_err(PIPE);
    } else {
        fprintf(stderr, "From StLucia:%s", buffer);
        return show_err(INVALIDMESSAGE);
    }
    return statusChecker;
}

// Print turn message if it is valid, otherwise showing error message
int print_turn_message(char* buffer) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    char* s3 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s %s", s1, s2, s3);
//int a = strlen(s2);
//printf("strlen(s2) = %d", a);
    for (int i = 0; i < 6; i++) {
        if ((successNum != 2) || (strlen(s2) != 6) || ((s2[i] != '1') 
                && (s2[i] != '2') && (s2[i] != '3') && (s2[i] != 'H')
                && (s2[i] != 'A') && (s2[i] != 'P'))) {
            fprintf(stderr, "From StLucia:%s", buffer);
            return show_err(INVALIDMESSAGE);
            exit(5);
        }
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    return 0;
} 

// Print rerolloed message if it is valid, otherwise showing error message
int print_rerolled_message(char* buffer) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    char* s3 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s %s", s1, s2, s3);
    for (int i = 0; i < 6; i++) {
        if ((successNum != 2) || ((s2[i] != '1') && (s2[i] != '2') 
                && (s2[i] != '3') && (s2[i] != 'H') 
                && (s2[i] != 'A') && (s2[i] != 'P'))) {
            fprintf(stderr, "From StLucia:%s", buffer);
            return show_err(INVALIDMESSAGE);
            exit(5);
        }
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    return 0;
}


// Print rolled message if it is valid, otherwise showing error message
int print_rolled_message(char* buffer, struct PlayerInfo* infoBox) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    char* s3 = malloc(sizeof(char) * 20);
    char* s4 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s %s %s", s1, s2, s3, s4);
    char playerID = s2[0];
    int startAlfabet = 'A';
    int endAlfabet = 'A' + infoBox->playerNum - 1;
    int numID = playerID;
    for (int i = 0; i < 6; i++) {
        if ((successNum != 3) || ((strlen(s2)) != 1) 
                || (isupper(playerID) == 0) || (numID < startAlfabet) 
                || (numID > endAlfabet) || ((s3[i] != '1') && (s3[i] != '2') 
                && (s3[i] != '3') && (s3[i] != 'H') && (s3[i] != 'A') 
                && (s3[i] != 'P'))) {
            fprintf(stderr, "From StLucia:%s", buffer);
            return show_err(INVALIDMESSAGE);
            exit(5);
        }
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    int hCounter = 0;
    int index = s2[0] - 65;
    if (infoBox->stluciaChecker[index] == 0) {
        for (int i = 0; i < 6; i++) {
            if (s3[i] == 'H') {
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
    return 0;
}


// Print points message if it is valid, otherwise showing error message
int print_points_message(char* buffer, struct PlayerInfo* infoBox) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    int s3 = 0;
    char* s4 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s %d %s", s1, s2, &s3, s4);
    char playerID = s2[0];
    int startAlfabet = 'A';
    int endAlfabet = 'A' + infoBox->playerNum - 1;
    int numID = playerID;
    if ((successNum != 3) || ((strlen(s2)) != 1) || (isupper(playerID) == 0) 
            || (s3 <= 0) || (numID < startAlfabet) || (numID > endAlfabet)) {
        fprintf(stderr, "From StLucia:%s %s %d\n", s1, s2, s3);
        return show_err(INVALIDMESSAGE);
        exit(5);
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    int id = playerID - 65;
    infoBox->pointChecker[id] = s3;
    return 0;
}

// Print attacks message if it is valid, otherwise showing error message
int print_attacks_message(char* buffer, struct PlayerInfo* infoBox) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    int s3 = 0;
    char* s4 = malloc(sizeof(char) * 20);
    char* s5 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s %d %s %s", s1, s2, &s3, s4, s5);
    char playerID = s2[0];
    int startAlfabet = 'A';
    int endAlfabet = 'A' + infoBox->playerNum - 1;
    int numID = playerID;
    if ((successNum != 4) || ((strlen(s2)) != 1) || (isupper(playerID) == 0) 
            || (s3 <= 0) || (numID < startAlfabet) || (numID > endAlfabet) 
            || (((strcmp(s4, "in")) != 0) && ((strcmp(s4, "out")) != 0))) {
        fprintf(stderr, "From StLucia:%s", buffer);
        return show_err(INVALIDMESSAGE);
        exit(5);
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    if (strcmp(s4, "out") == 0) {
        for (int i = 0; i < 26; i++) {
            if ((infoBox->statusChecker[i] == 1) 
                    && (infoBox->stluciaChecker[i] == 0)) {
                infoBox->healthChecker[i] -= s3;
            }
        }
    }
    if (strcmp(s4, "in") == 0) {
        for (int i = 0; i < 26; i++) {
            if ((infoBox->statusChecker[i] == 1) 
                    && (infoBox->stluciaChecker[i] == 1)) {
                infoBox->healthChecker[i] -= s3;
            }
        }
    }
    return 0;
}

// Print eliminated message if it is valid, otherwise showing error message
int print_eliminated_message(char* buffer, struct PlayerInfo* infoBox) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    char* s3 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s %s", s1, s2, s3);
    char playerID = s2[0];
    int startAlfabet = 'A';
    int endAlfabet = 'A' + infoBox->playerNum - 1;
    int numID = playerID;
    if ((successNum != 2) || ((strlen(s2)) != 1) || (isupper(playerID) == 0)
            || (numID < startAlfabet) || (numID > endAlfabet)) {
        fprintf(stderr, "From StLucia:%s", buffer);
        return show_err(INVALIDMESSAGE);
        exit(5);
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    if (playerID == infoBox->playerID) {
        exit(0);
    }
    int id = numID - 65;
    infoBox->statusChecker[id] = 0;
    return 0;
}

// Print claim message if it is valid, otherwise showing error message
int print_claim_message(char* buffer, struct PlayerInfo* infoBox) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    char* s3 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s %s", s1, s2, s3);
    char playerID = s2[0];
    int startAlfabet = 'A';
    int endAlfabet = 'A' + infoBox->playerNum - 1;
    int numID = playerID;
    if ((successNum != 2) || ((strlen(s2)) != 1) || (isupper(playerID) == 0)
            || (numID < startAlfabet) || (numID > endAlfabet)) {
        fprintf(stderr, "From StLucia:%s", buffer);
        return show_err(INVALIDMESSAGE);
        exit(5);
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    // Give 1 point to player
    int id = numID - 65;
    infoBox->pointChecker[id]++;
    infoBox->stluciaChecker[id] = 1;
    return 0;
}

// Print stay message if it is valid, otherwise showing error message
int print_stay_message(char* buffer) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s", s1, s2);
    if (successNum != 1) {
        fprintf(stderr, "From StLucia:%s", buffer);
        return show_err(INVALIDMESSAGE);
        exit(5);
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    return 0;
}


// Print winner message if it is valid, otherwise showing error message
int print_winner_message(char* buffer, struct PlayerInfo* infoBox) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    char* s3 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s %s", s1, s2, s3);
    char playerID = s2[0];
    int startAlfabet = 'A';
    int endAlfabet = 'A' + infoBox->playerNum - 1;
    int numID = playerID;
    if ((successNum != 2) || ((strlen(s2)) != 1) || (isupper(playerID) == 0)
            || (numID < startAlfabet) || (numID > endAlfabet)) {
        fprintf(stderr, "From StLucia:%s", buffer);
        return show_err(INVALIDMESSAGE);
        exit(5);
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    return 0;
}


// Print shutdown message if it is valid, otherwise showing error message
int print_shutdown_message(char* buffer) {
    char* s1 = malloc(sizeof(char) * 20);
    char* s2 = malloc(sizeof(char) * 20);
    int successNum = sscanf(buffer, "%s %s", s1, s2);
    if (successNum != 1) {
        fprintf(stderr, "From StLucia:%s", buffer);
        return show_err(INVALIDMESSAGE);
        exit(5);
    }
    fprintf(stderr, "From StLucia:%s", buffer);
    return 0;
}


// Create each player infomation box
int create_each_player_info(char** argv, struct PlayerInfo* infoBox) {
    int buffer = 0;
    sscanf(argv[1], "%d", &buffer);
    infoBox->playerNum = buffer;
    infoBox->playerID = argv[2][0];
    for (int i = 0; i < 26; i++) {
        if (i < buffer) {
            infoBox->healthChecker[i] = 10;
            infoBox->statusChecker[i] = 1;
        } else {
            infoBox->healthChecker[i] = 0;
            infoBox->statusChecker[i] = 0;
        }
        infoBox->pointChecker[i] = 0;
        infoBox->tokenChecker[i] = 0;
        infoBox->stluciaChecker[i] = 0;
    }
    infoBox->count_reroll = 0;
    return 0;
}


int main(int argc, char** argv) {
    // Check arguments length
    if (argc != 3) {
        return show_err(WRONGNUM);
    }
    // Check player numbers
    char* nerr = 0;
    unsigned int numsPlayer = (unsigned int)strtoul(argv[1], &nerr, 10);
    if ((*nerr != '\0') || (numsPlayer < 2) || (numsPlayer > 26)) {
        return show_err(INVALIDNUM);
    }
    // Check playerID is A - Z
    char id = argv[2][0];
    int len = strlen(argv[2]);
    if ((len != 1) || (isupper(id) == 0)) {
        return show_err(INVALIDPLAYER);
    }
    // Send "!" to stdout
    fprintf(stdout, "%c", '!');
    fflush(stdout);
    // Create struct to collect player information
    struct PlayerInfo* infoBox = malloc(sizeof(struct PlayerInfo));
    create_each_player_info(argv, infoBox);
    // Start game
    int result;
    while(1) {
        result = recieve_message_from_stlucia(infoBox);
        if (result) { 
            return result;
        }
    }
    return 0;
}




