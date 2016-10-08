typedef enum {
    NORMALEND = 0,
    WRONGNUM = 1,
    INVALIDNUM = 2,
    INVALIDPLAYER = 3,
    PIPE = 4,
    INVALIDMESSAGE = 5
} Err;

Err show_err(Err e);

struct PlayerInfo {
    char playerID;
    int healthChecker[26];
    int pointChecker[26];
    int tokenChecker[26];
    int statusChecker[26];
    int stluciaChecker[26];
    int count_reroll;
    int playerNum;
};

int send_message_to_stlucia(char *buffer2);
int recieve_message_from_stlucia(struct PlayerInfo *infoBox);
int print_turn_message(char *buffer);
int print_rerolled_message(char *buffer);
int print_rolled_message(char *buffer, struct PlayerInfo *infoBox);
int print_points_message(char *buffer, struct PlayerInfo *infoBox);
int print_attacks_message(char *buffer, struct PlayerInfo *infoBox);
int print_eliminated_message(char *buffer, struct PlayerInfo *infoBox);
int print_claim_message(char *buffer, struct PlayerInfo *infoBox);
int print_stay_message(char *buffer);
int print_winner_message(char *buffer, struct PlayerInfo *infoBox);
int print_shutdown_message(char *buffer);
int take_action_of_each_player(struct PlayerInfo *infoBox, char *buffer, char *buffer2);
int take_action_of_each_player2(struct PlayerInfo *infoBox, char *buffer2);
