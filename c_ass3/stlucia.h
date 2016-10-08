
#define READ 0
#define WRITE 1

struct StluciaInfo {
    int playerNum;
    char playerid;
    int goalPoint;
    int statusChecker[26];
    int stluciaChecker[26];
    int healthChecker[26];
    int pointChecker[26];
    int previousTurnPoint[26];
    int tokenChecker[26];
    int* collectionNums;
    char* fileContents;
    char* p1;
};

char* load_file(char** argv);
int setup_pipe(char** argv, int argc, struct StluciaInfo *infoBox);
int create_info_box(int argc, char** argv, struct StluciaInfo* infoBox, char* fileContent);
int get_index(char playerid, int direction, int rw);
char* dice_process(int index, struct StluciaInfo* infoBox);
char* decide_dice(struct StluciaInfo* infoBox);
int award_point(int index, struct StluciaInfo* infoBox);
char* arrange_dice(char* dices);
int send_turn_mes(int index, char* arrangedDices, struct StluciaInfo* infoBox);
int send_rerolled_mes(int index, char* arrangedDices, struct StluciaInfo* infoBox);
char* recieve_replay(int index, struct StluciaInfo* infoBox);
char* recieve_replay2(struct StluciaInfo* infoBox);
int check_replay(char* replay);
char* reroll_dice(char* dices, char* s2, struct StluciaInfo* infoBox);
int announce_rolled_mes(int index, char* arrangedDices, struct StluciaInfo* infoBox);
int healing_process(int index, struct StluciaInfo* infoBox, char* rolledDices);
int attack_process(int index, struct StluciaInfo* infoBox, char* rolledDices);
int points_process(int index, struct StluciaInfo* infoBox, char* rolledDices);
int elimination_process(struct StluciaInfo* infoBox);
int test_gameover_process(struct StluciaInfo* infoBox);
int announce_claim_mes(int index, struct StluciaInfo* infoBox);
int attack_outside(int aCounter, struct StluciaInfo* infoBox);
int attack_inside(int aCounter, struct StluciaInfo* infoBox);
int announce_attacks_outside_mes(int index, int aCounter, struct StluciaInfo* infoBox);
int announce_attacks_inside_mes(int index, int aCounter, struct StluciaInfo* infoBox);
int announce_points_mes(int index, struct StluciaInfo* infoBox, char* rolledDices);
int announce_eliminated_mes(int index, struct StluciaInfo* infoBox);
int announce_winner_mes(int index, struct StluciaInfo* infoBox);
int announce_stay_mes(struct StluciaInfo* infoBox);
int add_points(int index, struct StluciaInfo* infoBox, char* rolledDices);
//int get_exclamation_mark(void);
