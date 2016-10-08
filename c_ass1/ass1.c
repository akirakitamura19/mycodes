/*
 * s4372218 Akira Kitamura
 * Master of Information Technology
 * CSSE7231 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>

struct BoxInfo {
    int order;
    int* filenums;
    int height;
    int width;
    char** box;
    int** zerobox;
    int* comp1;
    int* comp2;
};

int initialize(int, char**);
void boxcreator(char**, struct BoxInfo *boxdata, int*, int*);
void loading_file(char** argv, struct BoxInfo *boxdata, int* complayer1, int* complayer2);
FILE* open_file(char** argv);
char* read_file(FILE* file);
char* get_first_line(char* result);
int* get_nine_numbers(char* firstLine);
void check_nine_numbers(int *nineNumbers); 
char* get_grid(char* result);
void check_grid_valid(char* grid, int* nineNumbers);
void create_filebox(char*, int*, struct BoxInfo *boxdata, int*, int*);
char** update_box(char** box, char* grid, int* nineNumbers);
int** create_zero_box(int row, int column);
int playerchecker(char**);
int dimensionchecker(char**);
char** newbox(int, int);
void printout(struct BoxInfo *boxdata);
int player1type(char**);
int player2type(char**);
void humanmove(struct BoxInfo *boxdata);
void filesaving(char*, struct BoxInfo *boxdata);
void nextstep1(int*, struct BoxInfo *boxdata);
void nextstep2(int*, struct BoxInfo *boxdata);
void firstcomputer(struct BoxInfo *boxdata);
void secondcomputer(struct BoxInfo *boxdata);
int spacechecker(int, int, struct BoxInfo *boxdata);
int firstlibertychecker(struct BoxInfo *boxdata);
int firstspacefinder(int, int, struct BoxInfo *boxdata);
int secondlibertychecker(struct BoxInfo *boxdata);
int secondspacefinder(int, int, struct BoxInfo *boxdata);

int main(int argc, char** argv) {
    int initialchecker = 0, player1, player2;
    int complayer1[9], complayer2[9];
    struct BoxInfo *boxdata = malloc(sizeof(struct BoxInfo));
    // initialize

    initialchecker = initialize(argc, argv);
    if(initialchecker == 1) {
        boxcreator(argv, boxdata, complayer1, complayer2);
    } else if (initialchecker == 2) {
        loading_file(argv, boxdata, complayer1, complayer2);
    }
    player1 = player1type(argv);
    player2 = player2type(argv);
    while(1){
        printout(boxdata);
        if ((firstlibertychecker(boxdata) == 0) && (secondlibertychecker(boxdata) == 0)) {
            if (boxdata->order % 2 == 0) {
                printf("Player X wins\n");
                break;
            } else {
                printf("Player O wins\n");
                break;
            }
        } else if (firstlibertychecker(boxdata) == 0) {
            printf("Player O wins\n");
            break;
        } else if (secondlibertychecker(boxdata) == 0) {
            printf("Player X wins\n");
            break;
        }
        if(boxdata->order % 2 == 0) {
            if(player1 == 1) {
                humanmove(boxdata);
            } else {
                firstcomputer(boxdata);
            }
			boxdata->order++;
        } else {
            if(player2 == 11) {
                humanmove(boxdata);
            } else {
                secondcomputer(boxdata);
            }
            boxdata->order++;
        }
    }        
    return 0;
}

int initialize(int argc, char** argv) {
    switch(argc) {
    // Set up height and width with int
        case 5:
            if(playerchecker(argv) == 1) {
                if(dimensionchecker(argv) == 1) {
                    return 1;
                }
            }
            break;
    // Set up with a file
        case 4:
            if(playerchecker(argv) == 1) {
                return 2;
            }
            break;
        default:
            fprintf(stderr, "Usage: nogo p1type p2type " 
                    "[height width | filename]\n");
            exit(1);
    }
    return 0;
}

void boxcreator(char** argv, struct BoxInfo *boxdata, int* complayer1, int* complayer2) {
    char** box;
    int i, j;
//    int** zerobox;
    int** newzerobox;
    newzerobox = malloc(sizeof(int *) * (atoi(argv[3]) + 4));
    for(j = 0; j <= (atoi(argv[3]) + 4); j++) {
        newzerobox[j] = malloc(sizeof(int) * (atoi(argv[4]) + 4));
    }

//    int *status[2];
    for(i = 0; i <= (atoi(argv[3]) + 4); i++) {
        for(j = 0; j <= (atoi(argv[4]) + 4); j++) {
            newzerobox[i][j] = 0;
        }
    }

    box = newbox((atoi(argv[3]) + 1), (atoi(argv[4]) + 2));
    complayer1[0] = 1;
    complayer1[1] = 4;
    complayer1[2] = 29;
    complayer1[3] = atoi(argv[3]);
    complayer1[4] = atoi(argv[4]);
    complayer1[5] = 0;
    complayer1[6] = 1 * atoi(argv[4]) + 4;
    complayer1[7] = 1;
    complayer1[8] = 4;
    complayer2[0] = 2;
    complayer2[1] = 10;
    complayer2[2] = 17;
    complayer2[3] = atoi(argv[3]);
    complayer2[4] = atoi(argv[4]);
    complayer2[5] = 0;
    complayer2[6] = 2 * atoi(argv[4]) + 10;
    complayer2[7] = 2;
    complayer2[8] = 10;

    boxdata->order = 0;
    boxdata->zerobox = newzerobox;
    boxdata->height = (atoi(argv[3]) + 1);
    boxdata->width = (atoi(argv[4]) + 2);
    boxdata->box = box;
    boxdata->comp1 = complayer1;
    boxdata->comp2 = complayer2;
}

void loading_file(char** argv, struct BoxInfo *boxdata, int* complayer1, int* complayer2) {
    FILE* file;
    char *result, *firstLine, *grid;
    int *nineNumbers;

    file = open_file(argv);

    result = read_file(file);

    firstLine = get_first_line(result);

    nineNumbers = get_nine_numbers(firstLine);
    
    check_nine_numbers(nineNumbers);

    grid = get_grid(result);

    check_grid_valid(grid, nineNumbers);

    create_filebox(grid, nineNumbers, boxdata, complayer1, complayer2);

}


FILE* open_file(char** argv) {
     FILE* file = fopen(argv[3], "r");
     if (file == NULL) {
         fprintf(stderr, "Unable to open file\n");
         exit(4);
     }
     return file;
}

char* read_file(FILE* file) {
    char* result = malloc(sizeof(char) * 2000);
    int position = 0;
    int next = 0;

    while (1) {
        next = fgetc(file);
        if (next == EOF) {
            result[position] = '\0';
            return result;
        } else {
            result[position++] = (char)next;
        }
    }
}

char* get_first_line(char* result) {
    char *firstLine = malloc(sizeof(char) * 80);
    int i;
    for(i = 0; *(result + i) != '\n'; i++) {
        firstLine[i] = result[i];
    }
    firstLine[i] = '\0';
    return firstLine;
}

int* get_nine_numbers(char* firstLine) {
    int count = 0;
    int *nineNumbers = malloc(sizeof(int) * 20);

    char** endps = malloc(sizeof(char*) * 20);
    for (int i = 0; i <= 10; i++) {
        *(endps + i) = malloc(sizeof(char) * 50);
    }

    nineNumbers[0] = strtol(firstLine, endps, 10);
    while (*(endps + (count - 1)) != *(endps + (count))) {
        nineNumbers[count + 1] = strtol(*(endps + count) , &*(endps + (count + 1)), 10);
        count++;
    }
    
    if (count != 9) {
        fprintf(stderr, "Incorrect file contents\n");
        exit(5);
    }
    return nineNumbers;
}


void check_nine_numbers(int *nineNumbers) { 
// Check the 9 intergers are valid or not
    if(nineNumbers[0] >= 4 && nineNumbers[0] <= 1000 && nineNumbers[1] >= 4 &&
            nineNumbers[1] <= 1000 && (nineNumbers[2] == 0 || nineNumbers[2] == 1) &&
            nineNumbers[3] >= 0 && nineNumbers[3] <= nineNumbers[0] &&
            nineNumbers[4] >= 0 && nineNumbers[4] <= nineNumbers[1] &&
            nineNumbers[5] >= 0 && nineNumbers[6] >= 0 &&
            nineNumbers[6] <= nineNumbers[0] &&
            nineNumbers[7] >= 0 && nineNumbers[7] <= nineNumbers[1]
            && nineNumbers[8] >= 0) {
// Do nothing
    } else {
        fprintf(stderr, "Incorrect file contents\n");
        exit(5);
    }
}

char* get_grid(char* result) {
    char* grid = malloc(sizeof(char) * 2000);
    int count = 0, i = 0, j = 0;
    while (result[i] != '\0') {
        if (count == 0) {
        // Do nothing
        } else { 
            grid[j] = result[i];
            j++;
        }
        if (result[i] == '\n') {
            count++;
        }
        i++;
    }
    return grid;
}


void check_grid_valid(char* grid, int* nineNumbers) {
    int i = 0, count1 = 0, count2 = 0, answer;
    while (grid[i] != '\0') {
        if (grid[i] == 'O' || grid[i] == 'X' || grid[i] == '.') {
            count1++;
        } else if (grid[i] == '\n') {
            count2++;
        }
        i++;
    }
    answer = strlen(grid);
    if (count1 != nineNumbers[0] * nineNumbers[1] || 
            answer != nineNumbers[0] * nineNumbers[1] + nineNumbers[0] ||
            count2 != nineNumbers[0]) {
        fprintf(stderr, "Incorrect file contents\n");
        exit(5);
        }
}


void create_filebox(char* grid, int* nineNumbers, struct BoxInfo *boxdata, int* complayer1, int* complayer2) {
    int filenumbers[8] = {10, nineNumbers[3], nineNumbers[4], nineNumbers[5],
        10, nineNumbers[6], nineNumbers[7], nineNumbers[8]};
    char** box;
    int** newzerobox;
    int* filenums = filenumbers;
    box = newbox(nineNumbers[0] + 1, nineNumbers[1] + 2);
    box = update_box(box, grid, nineNumbers);

    newzerobox = create_zero_box(nineNumbers[0], nineNumbers[1]);

    complayer1[0] = 1;
    complayer1[1] = 4;
    complayer1[2] = 29;
    complayer1[3] = nineNumbers[0];
    complayer1[4] = nineNumbers[1];
    complayer1[5] = 0;
    complayer1[6] = 1 * nineNumbers[1] + 4;
    complayer1[7] = 1;
    complayer1[8] = 4;
    complayer2[0] = 2;
    complayer2[1] = 10;
    complayer2[2] = 17;
    complayer2[3] = nineNumbers[0];
    complayer2[4] = nineNumbers[1];
    complayer2[5] = 0;
    complayer2[6] = 2 * nineNumbers[1] + 10;
    complayer2[7] = 2;
    complayer2[8] = 10;

    boxdata->order = nineNumbers[2];
    boxdata->filenums = filenums;
    boxdata->zerobox = newzerobox;
    boxdata->height = (nineNumbers[0] + 1);
    boxdata->width = (nineNumbers[1] + 2);
    boxdata->box = box;
    boxdata->comp1 = complayer1;
    boxdata->comp2 = complayer2;
} 

char** update_box(char** box, char* grid, int* nineNumbers) {
    int count=0;

    for(int i = 1; i <= nineNumbers[0]; i++) {
        for(int j = 1; j <= (nineNumbers[1] + 1); j++) {
            if (grid[count] == '\n') {
                count++;
            } else {
                box[i][j] = grid[count];
                count++;
            }
        }
    }
    return box;
}

int** create_zero_box(int row, int column) {
    int **zerobox = malloc(sizeof(int *) * (row + 4));
    for (int i = 0; i < row + 6; i++) {
        *(zerobox + i) = malloc(sizeof(int) * (column + 6));
    }
    for(int i = 0; i < row + 4; i++) {
        for(int j = 0; j < column + 4; j++) {
            zerobox[i][j] = 0;
        }
    }
    return zerobox;
}


int playerchecker(char** argv) {
    if(((strcmp(argv[1], "c") == 0) || (strcmp(argv[1], "h") == 0)) &&
        ((strcmp(argv[2], "c") == 0) || (strcmp(argv[2], "h") == 0))) {
        return 1;
    } else {
        fprintf(stderr, "Invalid player type\n");
        exit(2);
    }
}

int dimensionchecker(char** argv) {
    if(((atoi(argv[3]) >= 4) && (atoi(argv[3]) <= 1000)) &&
        ((atoi(argv[4]) >= 4) && (atoi(argv[4]) <= 1000))) {
        return 1;
    } else {
        fprintf(stderr, "Invalid board dimension\n");
        exit(3);
    }
}

char** newbox(int height, int width) {
    int i, j;
    char** box;
    box = malloc(sizeof(char *) * height);
    for(j = 0; j <= height; j++) {
        box[j] = malloc(sizeof(char) * width);
    }
    for(j = 0; j <= height; j++) {
        if(j == 0) {
            for(i = 0; i < width; i++) {
                if(i == 0) {
                    box[j][i] = '/';
                } else if(i == width - 1) {
                    box[j][width - 1] = '\\';
                } else {
                    box[j][i] = '-';
                }
            }
            box[j][width] = '\0';
        } else if(j != 0 && j != height) {
            for(i = 0; i < width; i++) {
                if(i == 0) {
                    box[j][i] = '|';
                } else if(i == width - 1) {
                    box[j][width - 1] = '|';
                } else {
                    box[j][i] = '.';
                }
            }
            box[j][width] = '\0';
        } else {
            for(i = 0; i < width; i++) {
                if(i == 0) {
                    box[j][i] = '\\';
                } else if(i == width - 1) {
                    box[j][width - 1] = '/';
                } else {
                    box[j][i] = '-';
                }
            }	
            box[j][width] = '\0';
        }
    }
    return box;
}

void printout(struct BoxInfo *boxdata){
	int i;
    for(i = 0; i <= boxdata->height; i++) {
        printf("%s\n", *((boxdata->box) + i));
    }
}

int plaayer1type(char** argv) {
    if(astrcmp(argv[1], "h") == 0) {
        return 1;
    } else {
        return 2;
    }
}

int player2type(char** argv) {
    if(strcmp(argv[2], "h") == 0) {
        return 11;
    } else {
        return 12;
    }
}

void humanmove(struct BoxInfo *boxdata) {
    int row, column;
    while(1) {
        if(boxdata->order % 2 == 0) {
            printf("Player O> ");
        } else {
            printf("Player X> ");
        }
        char str[70], *ch1, *ch2, *ch3, *ch1endp, *ch2endp;
        if(!fgets(str, sizeof(str), stdin)){
            fprintf(stderr, "End of input from user\n");
            exit(6);
        }
        if(str[0] == 'w') {
            filesaving((str + 1), boxdata);
        }
        ch1 = strtok(str, " ");
        if(ch1 != NULL) {
            ch2 = strtok(NULL, " ");
            if(ch2 != NULL){
                ch3 = strtok(NULL, " ");
                if(ch3 == NULL) {
                    row = strtol(ch1, &ch1endp, 10);
                    column = strtol(ch2, &ch2endp, 10);
                    if(*ch1endp == '\0' && *ch2endp == 10) {
                        if(spacechecker(row, column, boxdata) == 1) {
                            break;
                        }
                    }
                }
            }
        }
    }
    if(boxdata->order % 2 == 0){
        boxdata->box[row + 1][column + 1] = 'O';
    } else {
        boxdata->box[row + 1][column + 1] = 'X';
    }
}


void filesaving(char* str, struct BoxInfo *boxdata) {
    int i, j, copycomp1[9], copycomp2[9];
printf("2.................");
    memcpy(copycomp1, boxdata->comp1, sizeof(copycomp1));
    memcpy(copycomp2, boxdata->comp2, sizeof(copycomp2));
printf("2.................");
    nextstep1(copycomp1, boxdata);
    nextstep2(copycomp2, boxdata);
    
    i = strlen(str);
    str[i - 1] = '\0';
    FILE *file;
    file = fopen(str, "w");
    fprintf(file, "%d %d %d %d %d %d %d %d %d\n",
        (boxdata->height - 1), (boxdata->width - 2), (boxdata->order % 2),
        copycomp1[7], copycomp1[8], *((boxdata->comp1) + 5),
        copycomp2[7], copycomp2[8], *((boxdata->comp2) + 5));
    for(i = 1; i <= boxdata->height - 1; i++) {
        for(j = 1; j <= boxdata->width - 2; j++) {
            fprintf(file, "%c", (boxdata->box)[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void nextstep1(int* copycomp1, struct BoxInfo *boxdata) {
    int N;
printf("0..................");
    do {
printf("1..................");
        if(*(boxdata->filenums + 0) == 10) {
printf("2.................");
            *(copycomp1 + 7) = *(boxdata->filenums + 1);
            *(copycomp1 + 8) = *(boxdata->filenums + 2);
            *(copycomp1 + 5) = *(boxdata->filenums + 3);
            *(boxdata->filenums + 0) = 11;
        } else  if(*(copycomp1 + 5) == 0) {
   //DO nothing
printf("3.................");
        } else {
printf("4.................");
            if(*(copycomp1 + 5) % 5 == 0) {
                N = -((*(copycomp1 + 6) + *(copycomp1 + 5)
                    / 5 * *(copycomp1 + 2)) % 1000003);
                *(copycomp1 + 7) = -(N / *(copycomp1 + 4));
                *(copycomp1 + 8) = -(N % *(copycomp1 + 4));
            } else if (*(copycomp1 + 5) % 5 == 1) {
                *(copycomp1 + 7) += 1;
                *(copycomp1 + 8) += 1;
            } else if (*(copycomp1 + 5) % 5 == 2) {
                *(copycomp1 +7) += 2;
                *(copycomp1 + 8) += 1;
            } else if (*(copycomp1 + 5) % 5 == 3) {
                *(copycomp1 + 7) += 1;
                *(copycomp1 + 8) += 0;
            } else {
                *(copycomp1 + 7) += 0;
                *(copycomp1 + 8) +=  1;
            }    
        }
printf("5.................");
        *(copycomp1 + 7) =
                 *(copycomp1 + 7) % *(copycomp1 + 3);
        *(copycomp1 + 8) =
                 *(copycomp1 + 8) % *(copycomp1 + 4);
        *(copycomp1 + 5) = *(copycomp1 + 5) + 1;
    } while(spacechecker(*(copycomp1 + 7),
        *(copycomp1 + 8), boxdata) != 1);
}


void nextstep2(int* copycomp2, struct BoxInfo *boxdata) {
    int N;
    do {
        if(*(boxdata->filenums + 4) == 10) {
            *(copycomp2 + 7) = *(boxdata->filenums + 5);
            *(copycomp2 + 8) = *(boxdata->filenums + 6);
            *(copycomp2 + 5) = *(boxdata->filenums + 7);
            *(boxdata->filenums + 4) = 11;
        } else if (*(copycomp2 + 5) == 0) {
   //DO nothing
        } else {
            if(*(copycomp2 + 5) % 5 == 0) {
                N = -((*(copycomp2 + 6) + *(copycomp2 + 5)
                    / 5 * *(copycomp2 + 2)) % 1000003);
                *(copycomp2 + 7) = -(N / *(copycomp2 + 4));
                *(copycomp2 + 8) = -(N % *(copycomp2 + 4));
            } else if (*(copycomp2 + 5) % 5 == 1) {
                *(copycomp2 + 7) += 1;
                *(copycomp2 + 8) += 1;
            } else if (*(copycomp2 + 5) % 5 == 2) {
                *(copycomp2 + 7) += 2;
                *(copycomp2 + 8) += 1;
            } else if (*(copycomp2 + 5) % 5 == 3) {
                *(copycomp2 + 7) += 1;
                *(copycomp2 + 8) += 0;
            } else {
                *(copycomp2 + 7) += 0;
                *(copycomp2 + 8) +=  1;
            }    
        }
        *(copycomp2 + 7) =
                 *(copycomp2 + 7) % *(copycomp2 + 3);
        *(copycomp2 + 8) =
                 *(copycomp2 + 8) % *(copycomp2 + 4);
        *(copycomp2 + 5) = *(copycomp2 + 5) + 1;
    } while(spacechecker(*(copycomp2 + 7),
        *(copycomp2 + 8), boxdata) != 1);
}





int spacechecker(int row, int column, struct BoxInfo *boxdata) {
    if(row >= 0 && row <= boxdata->height -2 &&
        column >= 0 && column <= boxdata->width -3 && 
        boxdata->box[row + 1][column + 1] == '.') {
        return 1;
    } else {
        return 0;
    }
}

void firstcomputer(struct BoxInfo *boxdata) {
    int N;
    do {
        if(*((boxdata->comp1) + 5) == 0) {
   //DO nothing
        } else {
            if(*((boxdata->comp1) + 5) % 5 == 0) {
                N = -((*((boxdata->comp1) + 6) + *((boxdata->comp1) + 5)
                    / 5 * *((boxdata->comp1) + 2)) % 1000003);
                *((boxdata->comp1) + 7) = -(N / *((boxdata->comp1) + 4));
                *((boxdata->comp1) + 8) = -(N % *((boxdata->comp1) + 4));
            } else if (*((boxdata->comp1) + 5) % 5 == 1) {
                *((boxdata->comp1) + 7) += 1;
                *((boxdata->comp1) + 8) += 1;
            } else if (*((boxdata->comp1) + 5) % 5 == 2) {
                (boxdata->comp1)[7] += 2;
                *((boxdata->comp1) + 8) += 1;
            } else if (*((boxdata->comp1) + 5) % 5 == 3) {
                *((boxdata->comp1) + 7) += 1;
                *((boxdata->comp1) + 8) += 0;
            } else {
                *((boxdata->comp1) + 7) += 0;
                *((boxdata->comp1) + 8) += 1;
            }    
        }
        *((boxdata->comp1) + 7) =
                 *((boxdata->comp1) + 7) % *((boxdata->comp1) + 3);
        *((boxdata->comp1) + 8) =
                 *((boxdata->comp1) + 8) % *((boxdata->comp1) + 4);
        *((boxdata->comp1) + 5) = *((boxdata->comp1) + 5) + 1;
    } while(spacechecker(*((boxdata->comp1) + 7),
        *((boxdata->comp1) + 8), boxdata) != 1);
    printf("Player O: %d %d\n",
        *((boxdata->comp1) + 7), *((boxdata->comp1) + 8));
    boxdata->box[*((boxdata->comp1) + 7) + 1]
        [*((boxdata->comp1) + 8) + 1] = 'O';
}


void secondcomputer(struct BoxInfo *boxdata) {
    int N;
    do {
        if(*((boxdata->comp2) + 5) == 0) {
   //DO nothing
        } else {
            if(*((boxdata->comp2) + 5) % 5 == 0) {
                N = -((*((boxdata->comp2) + 6) + *((boxdata->comp2) + 5)
                    / 5 * *((boxdata->comp2) + 2)) % 1000003);
                *((boxdata->comp2) + 7) = -(N / *((boxdata->comp2) + 4));
                *((boxdata->comp2) + 8) = -(N % *((boxdata->comp2) + 4));
            } else if (*((boxdata->comp2) + 5) % 5 == 1) {
                *((boxdata->comp2) + 7) += 1;
                *((boxdata->comp2) + 8) += 1;
            } else if (*((boxdata->comp2) + 5) % 5 == 2) {
                (boxdata->comp2)[7] += 2;
                *((boxdata->comp2) + 8) += 1;
            } else if (*((boxdata->comp2) + 5) % 5 == 3) {
                *((boxdata->comp2) + 7) += 1;
                *((boxdata->comp2) + 8) += 0;
            } else {
                *((boxdata->comp2) + 7) += 0;
                *((boxdata->comp2) + 8) +=  1;
            }    
        }
        *((boxdata->comp2) + 7) =
                 *((boxdata->comp2) + 7) % *((boxdata->comp2) + 3);
        *((boxdata->comp2) + 8) =
                 *((boxdata->comp2) + 8) % *((boxdata->comp2) + 4);
        *((boxdata->comp2) + 5) = *((boxdata->comp2) + 5) + 1;
    } while(spacechecker(*((boxdata->comp2) + 7),
        *((boxdata->comp2) + 8), boxdata) != 1);
    printf("Player X: %d %d\n",
        *((boxdata->comp2) + 7), *((boxdata->comp2) + 8));
    boxdata->box[*((boxdata->comp2) + 7) + 1]
        [*((boxdata->comp2) + 8) + 1] = 'X';
}





int firstlibertychecker(struct BoxInfo *boxdata) {
    int row, column, i, j;
    for(row = 1; row <= boxdata->height - 1; row++) {
        for(column = 1; column <= boxdata->width - 2; column++) {
            if((boxdata->box)[row][column] == 'X') {
                for(i = 0; i <= boxdata->height + 2; i++) {
                    for(j = 0; j <= boxdata->width + 2; j++) {
                        (boxdata->zerobox)[i][j] = 0;
                    }
                }
                if(firstspacefinder(row, column, boxdata) == 0) {
                   return 0;
                }
            }
        }
    }
    return 1;
}

int firstspacefinder(int row, int column, struct BoxInfo *boxdata) {
    (boxdata->zerobox)[row][column] = 1;
    if(((boxdata->box)[row - 1][column] == '.') ||
            ((boxdata->box)[row][column + 1] == '.') ||
            ((boxdata->box)[row + 1][column] == '.') ||
            ((boxdata->box)[row][column - 1] == '.')) {
        return 1;
    }
    if ((boxdata->box)[row - 1][column] == 'X') {
        if(((boxdata->zerobox)[row - 1][column] == 0) &&
                (firstspacefinder((row - 1), column, 
                boxdata) == 1)) {
            return 1;
        }
    }
    if ((boxdata->box)[row][column + 1] == 'X') {
        if(((boxdata->zerobox)[row][column + 1] == 0) &&
                (firstspacefinder(row, (column + 1),
                boxdata) == 1)) {
            return 1;
        }
    }
    if ((boxdata->box)[row + 1][column] == 'X') {
        if(((boxdata->zerobox)[row + 1][column] == 0) && 
                (firstspacefinder((row + 1), column,
                boxdata) == 1)) {
            return 1;
        }
    }
    if ((boxdata->box)[row][column - 1] == 'X') {
        if(((boxdata->zerobox)[row][column - 1] == 0) &&
                (firstspacefinder(row, (column - 1), 
                boxdata) == 1)) {
            return 1;
        }   
    }
    return 0;
}



int secondlibertychecker(struct BoxInfo *boxdata) {
    int row, column, i, j;
    for(row = 1; row <= boxdata->height - 1; row++) {
        for(column = 1; column <= boxdata->width - 2; column++) {
            if((boxdata->box)[row][column] == 'O') {
                for(i = 0; i <= boxdata->height + 2; i++) {
                    for(j = 0; j <= boxdata->width + 2; j++) {
                        (boxdata->zerobox)[i][j] = 0;
                    }
                }
                if(secondspacefinder(row, column, boxdata) == 0) {
                   return 0;
                }
            }
        }
    }
    return 1;
}

int secondspacefinder(int row, int column, struct BoxInfo *boxdata) {
    (boxdata->zerobox)[row][column] = 1;
    if(((boxdata->box)[row - 1][column] == '.') ||
            ((boxdata->box)[row][column + 1] == '.') ||
            ((boxdata->box)[row + 1][column] == '.') ||
            ((boxdata->box)[row][column - 1] == '.')) {
        return 1;
    }

    if ((boxdata->box)[row - 1][column] == 'O') {
        if(((boxdata->zerobox)[row - 1][column] == 0) &&
                (secondspacefinder((row - 1), column, 
                boxdata) == 1)) {
            return 1;
        }
    }
    if ((boxdata->box)[row][column + 1] == 'O') {
        if(((boxdata->zerobox)[row][column + 1] == 0) &&
                (secondspacefinder(row, (column + 1),
                boxdata) == 1)) {
            return 1;
        }
    }
    if ((boxdata->box)[row + 1][column] == 'O') {
        if(((boxdata->zerobox)[row + 1][column] == 0) && 
                (secondspacefinder((row + 1), column,
                boxdata) == 1)) {
            return 1;
        }
    }
    if ((boxdata->box)[row][column - 1] == 'O') {
        if(((boxdata->zerobox)[row][column - 1] == 0) &&
                (secondspacefinder(row, (column - 1), 
                boxdata) == 1)) {
            return 1;
        }   
    }
    return 0;
}

