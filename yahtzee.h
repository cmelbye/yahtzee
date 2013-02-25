#define BANNER " __   __    _     _ \n\
 \\ \\ / /   | |   | | \n\
  \\ V /__ _| |__ | |_ _______  ___ \n\
   \\ // _` | '_ \\| __|_  / _ \\/ _ \\ \n\
   | | (_| | | | | |_ / /  __/  __/ \n\
   \\_/\\__,_|_| |_|\\__/___\\___|\\___|"

#define CHEAT 0
#define PLAYER_1 101
#define PLAYER_2 102
#define DIVIDER "\\n--------------------------------------------------------------------------------\\n"

typedef struct dice_s {
    int value;
    int hold;
} dice_t;

typedef struct play_s {
    char *type;
    int points;
    int completed;
} play_t;

int calcUpper(dice_t dice[5], int val);
int calcEvens(dice_t dice[5]);
int calcOdds(dice_t dice[5]);
int calcYahtzee(dice_t dice[5]);
int calcChance(dice_t dice[5]);
void choosePlay(dice_t dice[5], play_t scores[12]);
int updatePlay(dice_t dice[5], play_t scores[12], int playIndex);
int calcPointsForPlayIndex(dice_t dice[5], int playIndex);
void sortDice(dice_t dice[5]);
void throwDice(dice_t dice[5], int cheat);
int selectDice(dice_t dice[5], play_t scores[12], int rollNumber, int player,
               int numberOfPlayers);
void toggleHold(dice_t dice[5], int diceIndex);
void holdAllDice(dice_t dice[5]);
int allDiceAreHeld(dice_t dice[5]);
void printScoreCard(dice_t dice[5], play_t scores[12]);
void printDice(dice_t dice[5]);
void initDice(dice_t dice[5]);
void initScores(play_t scores[12]);
int calcScore(play_t scores[12]);
dice_t makeDice(int value, int hold);
void makeFiveDice(dice_t dice[5], int value1, int value2, int value3,
                  int value4, int value5);
play_t makePlay(const char *type, int points, int completed);
void clearScreen(void);
void test_cases(void);
