#define CHEAT 0

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
void sortDice(dice_t dice[5]);
void throwDice(dice_t dice[5], int cheat);
void printScoreCard(play_t scores[12]);
void printDice(dice_t dice[5]);
void initDice(dice_t dice[5]);
void initScores(play_t scores[12]);
dice_t makeDice(int value, int hold);
void makeFiveDice(dice_t dice[5], int value1, int value2, int value3,
                  int value4, int value5);
play_t makePlay(const char *type, int points, int completed);
void test_cases(void);
