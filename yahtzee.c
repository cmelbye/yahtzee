#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "checkit.h"
#include "yahtzee.h"

int main(void)
{
    int turn;
    int round;
    int numberOfPlayers = 0;
    int currentDiceRoll = 0;
    int currentPlayer = 0;
    int selectResult;
    
    dice_t gameDice[5];
    play_t playerOneScores[12];
    play_t playerTwoScores[12];
    
    int playerOneScore, playerTwoScore;
    
    srand((unsigned int) time(0));

#ifndef NCHECKIT
    test_cases();
#endif
    
    clearScreen();
    
    printf("\n\n");
    printf(BANNER);
    printf("\n\n\n");
    
    printf("   Welcome! :-) It's recommended that your terminal is at least as wide as this.\n\n");
    
    while(numberOfPlayers != 1 && numberOfPlayers != 2)
    {
        printf("   How many players? ");
        
        scanf("%d", &numberOfPlayers);
        getchar();
        
        if(numberOfPlayers != 1 && numberOfPlayers != 2)
        {
            printf("   Either one or two players can play, try again.\n\n");
        }
    }
    
    initScores(playerOneScores);
    
    if(numberOfPlayers == 2)
    {
        initScores(playerTwoScores);
    }
    
    for(round = 0; round < 12; round++)
    {
        currentPlayer = PLAYER_1;
        
        for(turn = 0; turn < numberOfPlayers; turn++)
        {
            initDice(gameDice);
            
            clearScreen();
            
            for(currentDiceRoll = 0; currentDiceRoll < 3; currentDiceRoll++)
            {
                throwDice(gameDice, CHEAT);
                
                if(currentDiceRoll != 2)
                {
                    if(currentPlayer == PLAYER_1)
                    {
                        selectResult = selectDice(gameDice, playerOneScores,
                                                  currentDiceRoll,
                                                  currentPlayer,
                                                  numberOfPlayers);
                    }
                    else
                    {
                        selectResult = selectDice(gameDice, playerTwoScores,
                                                  currentDiceRoll,
                                                  currentPlayer,
                                                  numberOfPlayers);
                    }
                    
                    clearScreen();
                    
                    if(selectResult == -1)
                    {
                        break;
                    }
                }
            }
            
            if(currentPlayer == PLAYER_1)
            {
                choosePlay(gameDice, playerOneScores);
                currentPlayer = PLAYER_2;
            }
            else
            {
                choosePlay(gameDice, playerTwoScores);
            }
        }
    }
    
    clearScreen();
    
    printDualScoreCard(playerOneScores, playerTwoScores, numberOfPlayers);
    
    printf("\nGame over!\n");
    
    if(numberOfPlayers == 1)
    {
        printf("Your score was %d.\n", calcScore(playerOneScores));
    }
    else
    {
        playerOneScore = calcScore(playerOneScores);
        playerTwoScore = calcScore(playerTwoScores);
        
        printf("Player 1 score: %d\n", playerOneScore);
        printf("Player 2 score: %d\n\n", playerTwoScore);
        
        if(playerOneScore == playerTwoScore)
        {
            printf("It's a tie!\n");
        }
        else if(playerOneScore > playerTwoScore)
        {
            printf("Player 1 wins!\n");
        }
        else
        {
            printf("Player 2 wins!\n");
        }
    }
    
    return 0;
}

int calcUpper(dice_t dice[5], int val)
{
    int i;
    int totalPoints = 0;
    
    for(i = 0; i < 5; i++)
    {
        if(dice[i].value == val)
        {
            totalPoints += dice[i].value;
        }
    }
    
    return totalPoints;
}

int calcLargeStraight(dice_t dice[5])
{
    int i;
    int pointsEarned = 40;
    
    dice_t sortedDice[5];
    
    memcpy(sortedDice, dice, sizeof(sortedDice));
    
    sortDice(sortedDice);
    
    for(i = 1; i < 5; i++)
    {
        if(sortedDice[i].value != (sortedDice[i - 1].value + 1))
        {
            pointsEarned = 0;
            break;
        }
    }
    
    return pointsEarned;
}

int calcSmallStraight(dice_t dice[5])
{
    int i;
    int pointsEarned = 0;
    int totalNonIncrements = 0;
    int currentStreak = 1;
    
    dice_t sortedDice[5];
    
    memcpy(sortedDice, dice, sizeof(sortedDice));
    
    sortDice(sortedDice);
    
    for(i = 1; i < 5; i++)
    {
        if(sortedDice[i].value != sortedDice[i - 1].value)
        {
            if(sortedDice[i].value != (sortedDice[i - 1].value + 1))
            {
                currentStreak = 1;
                totalNonIncrements++;
                
                if(totalNonIncrements > 1)
                {
                    break;
                }
            }
            else
            {
                currentStreak++;
            }
        }
        
        if(currentStreak == 4)
        {
            pointsEarned = 30;
            break;
        }
    }
    
    return pointsEarned;
}

int calcEvens(dice_t dice[5])
{
    int i;
    int pointsEarned = 20;
    
    for(i = 0; i < 5; i++)
    {
        if((dice[i].value % 2) != 0)
        {
            pointsEarned = 0;
            break;
        }
    }
    
    return pointsEarned;
}

int calcOdds(dice_t dice[5])
{
    int i;
    int pointsEarned = 20;
    
    for(i = 0; i < 5; i++)
    {
        if((dice[i].value % 2) == 0)
        {
            pointsEarned = 0;
            break;
        }
    }
    
    return pointsEarned;
}

int calcYahtzee(dice_t dice[5])
{
    int i;
    int pointsEarned = 50;
    
    for(i = 1; i < 5; i++)
    {
        if(dice[i].value != dice[0].value)
        {
            pointsEarned = 0;
            break;
        }
    }
    
    return pointsEarned;
}

int calcChance(dice_t dice[5])
{
    int i;
    int totalPoints = 0;
    
    for(i = 0; i < 5; i++)
    {
        totalPoints += dice[i].value;
    }
    
    return totalPoints;
}

void choosePlay(dice_t dice[5], play_t scores[12])
{
    int playChoice = 0;
    
    printf("FINAL ROLL\n");
    printDice(dice);
    
    printScoreCard(dice, scores);
    
    while((playChoice < 1) || (playChoice > 12))
    {
        printf("Choice: ");
        scanf("%d", &playChoice);
        getchar();
        
        if((playChoice < 1) || (playChoice > 12))
        {
            printf("That's not a valid choice, try again.\n");
        }
        else if(updatePlay(dice, scores, (playChoice - 1)) == -1)
        {
            printf("You already have a score for that play, try again.\n");
            playChoice = 0;
        }
    }
}

int updatePlay(dice_t dice[5], play_t scores[12], int playIndex)
{
    int points = 0;
    
    if(scores[playIndex].completed)
    {
        return -1;
    }
    
    points = calcPointsForPlayIndex(dice, playIndex);
    
    scores[playIndex].points = points;
    scores[playIndex].completed = 1;
    
    return 0;
}

int calcPointsForPlayIndex(dice_t dice[5], int playIndex)
{
    int points = 0;
    
    switch (playIndex) {
        case 0: /* Aces */
            points = calcUpper(dice, 1);
            break;
        case 1: /* Twos */
            points = calcUpper(dice, 2);
            break;
        case 2: /* Threes */
            points = calcUpper(dice, 3);
            break;
        case 3: /* Fours */
            points = calcUpper(dice, 4);
            break;
        case 4: /* Fives */
            points = calcUpper(dice, 5);
            break;
        case 5: /* Sixes */
            points = calcUpper(dice, 6);
            break;
        case 6: /* Small Straight */
            points = calcSmallStraight(dice);
            break;
        case 7: /* Large Straight */
            points = calcLargeStraight(dice);
            break;
        case 8: /* All Evens */
            points = calcEvens(dice);
            break;
        case 9: /* All Odds */
            points = calcOdds(dice);
            break;
        case 10: /* Yahtzee */
            points = calcYahtzee(dice);
            break;
        case 11: /* Chance */
            points = calcChance(dice);
            break;
        default:
            printf("ERROR: Invalid playIndex in updatePlay().\n");
            exit(1);
            break;
    }
    
    return points;
}

void sortDice(dice_t dice[5])
{
    int c, d, position;
    dice_t swap;
    
    for(c = 0; c < 5 - 1; c++)
    {
        position = c;
        
        for(d = c + 1; d < 5; d++)
        {
            if(dice[position].value > dice[d].value)
            {
                position = d;
            }
        }
        
        if(position != c)
        {
            swap = dice[c];
            dice[c] = dice[position];
            dice[position] = swap;
        }
    }
}

void throwDice(dice_t dice[5], int cheat)
{
    int i;
    int dice1, dice2, dice3, dice4, dice5;
    int goodInput = 0;
    
    if(cheat)
    {
        while(!goodInput)
        {
            printf("(CHEAT) Enter five dice values separated by spaces: ");
            
            scanf("%d %d %d %d %d",
                  &dice1,
                  &dice2,
                  &dice3,
                  &dice4,
                  &dice5);
            getchar();
            
            if((dice1 >= 1) && (dice1 <= 6) &&
               (dice2 >= 1) && (dice2 <= 6) &&
               (dice3 >= 1) && (dice3 <= 6) &&
               (dice4 >= 1) && (dice4 <= 6) &&
               (dice5 >= 1) && (dice5 <= 6))
            {
                goodInput = 1;
            }
            else
            {
                printf("Invalid input, try again...\n");
            }
        }
        
        makeFiveDice(dice, dice1, dice2, dice3, dice4, dice5);
    }
    else
    {
        for(i = 0; i < 5; i++)
        {
            if(!dice[i].hold)
            {
                dice[i].value = (rand() % 6) + 1;
            }
        }
    }
}

int selectDice(dice_t dice[5], play_t scores[12], int rollNumber, int player,
               int numberOfPlayers)
{
    char diceInput[80];
    int index = 0;
    int doneWithInput = 0;
    int outputValue = 0;
    
    while(!doneWithInput)
    {
        index = 0;
        
        if(numberOfPlayers == 2)
        {
            if(player == PLAYER_1)
            {
                printf("PLAYER 1, ");
            }
            else
            {
                printf("PLAYER 2, ");
            }
        }
        
        printf("ROLL #%d\n", (rollNumber + 1));
        printDice(dice);
        
        printf("Choose dice, specified by their letters, to toggle between held and not held.\n");
        printf("Multiple dice can be chosen at once by typing multiple letters without spaces.\n");
        printf("Type 's' to see your score card.\n");
        printf("Type 'q' if you DON'T need to roll again and want to pick your score.\n");
        printf("Type 'r' to roll the dice again.\n");
        printf("Choice: ");
        
        fgets(diceInput, 80, stdin);
        diceInput[strlen(diceInput)-1] = '\0';
        
        while(diceInput[index] != '\0')
        {
            switch (diceInput[index]) {
                case 'q':
                    doneWithInput = 1;
                    outputValue = -1;
                    break;
                case 'r':
                    doneWithInput = 1;
                    break;
                case 's':
                    clearScreen();
                    printScoreCard(dice, scores);
                    printf("\n\n(press ENTER to continue)\n");
                    getchar();
                    break;
                case 'a':
                    toggleHold(dice, 0);
                    break;
                case 'b':
                    toggleHold(dice, 1);
                    break;
                case 'c':
                    toggleHold(dice, 2);
                    break;
                case 'd':
                    toggleHold(dice, 3);
                    break;
                case 'e':
                    toggleHold(dice, 4);
                    break;
                default:
                    break;
            }
            
            index++;
        }
        
        if(!doneWithInput)
        {
            clearScreen();
        }
    }
    
    return outputValue;
}

void toggleHold(dice_t dice[5], int diceIndex)
{
    if(dice[diceIndex].hold == 1)
    {
        dice[diceIndex].hold = 0;
    }
    else
    {
        dice[diceIndex].hold = 1;
    }
}

void holdAllDice(dice_t dice[5])
{
    int i;
    
    for(i = 0; i < 5; i++)
    {
        dice[i].hold = 1;
    }
}

int allDiceAreHeld(dice_t dice[5])
{
    int i;
    
    for(i = 0; i < 5; i++)
    {
        if(dice[i].hold == 0)
        {
            return 0;
        }
    }
    
    return 1;
}

void printScoreCard(dice_t dice[5], play_t scores[12])
{
    int i, rowNumber;
    
    printf("SCORE CARD\n\n");
    
    for(i = 0; i < 12; i++)
    {
        rowNumber = i + 1;
        
        if(rowNumber < 10)
        {
            printf(" %d. ", rowNumber);
        }
        else
        {
            printf("%d. ", rowNumber);
        }
        
        printf("%-20s", scores[i].type);
        
        if(scores[i].completed)
        {
            printf("%d points\n", scores[i].points);
        }
        else
        {
            printf("(not used, potential points: %d)\n",
                   calcPointsForPlayIndex(dice, i));
        }
    }
}

void printDualScoreCard(play_t playerOneScores[12], play_t playerTwoScores[12],
                        int numberOfPlayers)
{
    int i, rowNumber, scoreCard;
    
    if(numberOfPlayers == 2)
    {
        printf("  PLAYER 1");
        printf("                                    ");
        printf("PLAYER 2\n\n");
    }
    else
    {
        printf(" FINAL SCORE CARD\n");
    }
    
    for(i = 0; i < 12; i++)
    {
        rowNumber = i + 1;
        
        printf(" ");
        
        for(scoreCard = 0; scoreCard < numberOfPlayers; scoreCard++)
        {
            if(rowNumber < 10)
            {
                printf(" %d. ", rowNumber);
            }
            else
            {
                printf("%d. ", rowNumber);
            }
            
            if(scoreCard == 0)
            {
                printf("%-20s", playerOneScores[i].type);
                
                if(playerOneScores[i].points < 10)
                {
                    printf(" ");
                }
                
                printf("%d points", playerOneScores[i].points);
            }
            else
            {
                printf("%-20s", playerTwoScores[i].type);
                
                if(playerTwoScores[i].points < 10)
                {
                    printf(" ");
                }
                
                printf("%d points", playerTwoScores[i].points);
            }
            
            if(scoreCard == 0)
            {
                printf("           ");
            }
        }
        
        printf("\n");
    }
    
    printf("\n");
}

void printDice(dice_t dice[5])
{
    int i;
    
    printf("\n");
    
    printf("  ");
    
    for(i = 0; i < 5; i++)
    {
        if(dice[i].hold)
        {
            printf("\33[31m");
        }
        
        printf("-------    ");
        
        if(dice[i].hold)
        {
            printf("\33[m");
        }
    }
    
    printf("\n");
    
    for(i = 0; i < 5; i++)
    {
        printf(" ");
        
        if(dice[i].hold)
        {
            printf("\33[31m");
        }
        
        if(dice[i].value == 1)
        {
            printf("|       |");
        }
        else if(dice[i].value == 2 || dice[i].value == 3)
        {
            printf("|     o |");
        }
        else if(dice[i].value == 4 || dice[i].value == 5 || dice[i].value == 6)
        {
            printf("| o   o |");
        }
        
        if(dice[i].hold)
        {
            printf("\33[m");
        }
        
        printf(" ");
    }
    
    printf("\n");
    
    for(i = 0; i < 5; i++)
    {
        printf(" ");
        
        if(dice[i].hold)
        {
            printf("\33[31m");
        }
        
        if(dice[i].value == 1 || dice[i].value == 3 || dice[i].value == 5)
        {
            printf("|   o   |");
        }
        else if(dice[i].value == 2 || dice[i].value == 4)
        {
            printf("|       |");
        }
        else if(dice[i].value == 6)
        {
            printf("| o   o |");
        }
        
        if(dice[i].hold)
        {
            printf("\33[m");
        }
        
        printf(" ");
    }
    
    printf("\n");
    
    for(i = 0; i < 5; i++)
    {
        printf(" ");
        
        if(dice[i].hold)
        {
            printf("\33[31m");
        }
        
        if(dice[i].value == 1)
        {
            printf("|       |");
        }
        else if(dice[i].value == 2 || dice[i].value == 3)
        {
            printf("| o     |");
        }
        else if(dice[i].value == 4 || dice[i].value == 5 || dice[i].value == 6)
        {
            printf("| o   o |");
        }
        
        if(dice[i].hold)
        {
            printf("\33[m");
        }
        
        printf(" ");
    }
    
    printf("\n");
    printf("  ");
    
    for(i = 0; i < 5; i++)
    {
        if(dice[i].hold)
        {
            printf("\33[31m");
        }
        
        printf("-------    ");
        
        if(dice[i].hold)
        {
            printf("\33[m");
        }
    }
    
    printf("\n");
    
    for(i = 0; i < 5; i++)
    {
        printf("     ");
        
        if(dice[i].hold)
        {
            printf("\33[31m");
        }
        
        printf("%c", (97 + i));
        
        if(dice[i].hold)
        {
            printf("\33[m");
        }
        
        printf("     ");
    }
    
    printf("\n\n");
}

/*
 * Initializes the dice array with five brand-new dice with the hold flag on
 * each set to 0.
 *
 * WARNING: Dice values for the new dice will initially be set in sequential
 * order (1, 2, 3, 4, 5) as a placeholder. Use throwDice() to randomize values.
 */
void initDice(dice_t dice[5])
{
    int i;
    
    for(i = 0; i < 5; i++)
    {
        dice[i] = makeDice(i + 1, 0);
    }
}

void initScores(play_t scores[12])
{
    int i;
    
    const char *types[] = {
        "Aces",
        "Twos",
        "Threes",
        "Fours",
        "Fives",
        "Sixes",
        "Small Straight",
        "Large Straight",
        "All Evens",
        "All Odds",
        "Yahtzee",
        "Chance"
    };
    
    for(i = 0; i < 12; i++)
    {
        scores[i] = makePlay(types[i], 0, 0);
    }
}

int calcScore(play_t scores[12])
{
    int i = 0;
    int score = 0;
    
    for(i = 0; i < 12; i++)
    {
        score += scores[i].points;
    }
    
    return score;
}

dice_t makeDice(int value, int hold)
{
    dice_t newDice;
    
    newDice.value = value;
    newDice.hold = hold;
    
    return newDice;
}

/*
 * This function is used in tests to build mock dice rolls to test score
 * calculation functions.
 *
 * It's also used to modify the entire dice array at once to facilitate
 * "cheating".
 *
 * WARNING: This function sets the hold flag of all dice to 1.
 */
void makeFiveDice(dice_t dice[5], int value1, int value2, int value3,
                  int value4, int value5)
{
    dice[0] = makeDice(value1, 1);
    dice[1] = makeDice(value2, 1);
    dice[2] = makeDice(value3, 1);
    dice[3] = makeDice(value4, 1);
    dice[4] = makeDice(value5, 1);
}

play_t makePlay(const char *type, int points, int completed)
{
    play_t newPlay;
    
    newPlay.type = malloc(strlen(type) + 1);
    strcpy(newPlay.type, type);
    
    newPlay.points = points;
    newPlay.completed = completed;
    
    return newPlay;
}

void clearScreen(void)
{
    printf("\033[2J\033[1;1H\n");
}

void test_cases(void)
{
    dice_t dice[5];
    
    /* calcUpper */
    
    checkit_section("calcUpper");
    
    makeFiveDice(dice, 2, 1, 2, 3, 2);
    checkit_int(calcUpper(dice, 2), 6);
    
    makeFiveDice(dice, 5, 5, 5, 5, 1);
    checkit_int(calcUpper(dice, 5), 20);
    
    makeFiveDice(dice, 5, 2, 3, 3, 1);
    checkit_int(calcUpper(dice, 1), 1);
    
    makeFiveDice(dice, 1, 5, 6, 2, 2);
    checkit_int(calcUpper(dice, 3), 0);
    
    makeFiveDice(dice, 5, 5, 5, 5, 5);
    checkit_int(calcUpper(dice, 3), 0);
    
    /* calcLargeStraight */
    
    checkit_section("calcLargeStraight");
    
    makeFiveDice(dice, 1, 2, 3, 4, 5);
    checkit_int(calcLargeStraight(dice), 40);
    
    makeFiveDice(dice, 2, 3, 4, 5, 6);
    checkit_int(calcLargeStraight(dice), 40);
    
    makeFiveDice(dice, 6, 3, 2, 5, 4);
    checkit_int(calcLargeStraight(dice), 40);
    
    makeFiveDice(dice, 1, 2, 3, 4, 2);
    checkit_int(calcLargeStraight(dice), 0);
    
    makeFiveDice(dice, 6, 1, 2, 3, 4);
    checkit_int(calcLargeStraight(dice), 0);
    
    /* calcSmallStraight */
    
    checkit_section("calcSmallStraight");
    
    makeFiveDice(dice, 2, 3, 1, 5, 4); /* test sorting */
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 1, 2, 3, 3, 4);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 1, 2, 3, 4, 6);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 2, 3, 4, 5, 5);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 2, 3, 4, 5, 6);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 2, 3, 4, 4, 5);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 1, 2, 2, 3, 6);
    checkit_int(calcSmallStraight(dice), 0);
    
    makeFiveDice(dice, 1, 2, 4, 5, 6);
    checkit_int(calcSmallStraight(dice), 0);
    
    makeFiveDice(dice, 1, 2, 3, 5, 6);
    checkit_int(calcSmallStraight(dice), 0);
    
    makeFiveDice(dice, 2, 3, 3, 4, 5);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 2, 3, 4, 4, 5);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 1, 3, 4, 5, 6);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 2, 2, 3, 4, 5);
    checkit_int(calcSmallStraight(dice), 30);
    
    /* calcEvens */
    
    checkit_section("calcEvens");
    
    makeFiveDice(dice, 2, 2, 4, 6, 2);
    checkit_int(calcEvens(dice), 20);
    
    makeFiveDice(dice, 2, 2, 2, 2, 2);
    checkit_int(calcEvens(dice), 20);
    
    makeFiveDice(dice, 2, 2, 2, 2, 1);
    checkit_int(calcEvens(dice), 0);
    
    makeFiveDice(dice, 1, 2, 2, 2, 2);
    checkit_int(calcEvens(dice), 0);
    
    makeFiveDice(dice, 2, 2, 1, 2, 2);
    checkit_int(calcEvens(dice), 0);
    
    /* calcOdds */
    
    checkit_section("calcOdds");
    
    makeFiveDice(dice, 1, 3, 1, 5, 3);
    checkit_int(calcOdds(dice), 20);
    
    makeFiveDice(dice, 1, 1, 1, 1, 1);
    checkit_int(calcOdds(dice), 20);
    
    makeFiveDice(dice, 3, 3, 3, 3, 2);
    checkit_int(calcOdds(dice), 0);
    
    makeFiveDice(dice, 2, 3, 3, 3, 3);
    checkit_int(calcOdds(dice), 0);
    
    makeFiveDice(dice, 3, 3, 2, 3, 3);
    checkit_int(calcOdds(dice), 0);
    
    /* calcYahtzee */
    
    checkit_section("calcYahtzee");
    
    makeFiveDice(dice, 5, 5, 5, 5, 5);
    checkit_int(calcYahtzee(dice), 50);
    
    makeFiveDice(dice, 2, 2, 2, 2, 2);
    checkit_int(calcYahtzee(dice), 50);
    
    makeFiveDice(dice, 2, 2, 2, 2, 1);
    checkit_int(calcYahtzee(dice), 0);
    
    makeFiveDice(dice, 1, 2, 2, 2, 2);
    checkit_int(calcYahtzee(dice), 0);
    
    /* calcChance */
    
    checkit_section("calcChance");
    
    makeFiveDice(dice, 1, 3, 4, 6, 2);
    checkit_int(calcChance(dice), 16);
    
    makeFiveDice(dice, 1, 1, 1, 1, 1);
    checkit_int(calcChance(dice), 5);
    
    makeFiveDice(dice, 5, 5, 5, 5, 5);
    checkit_int(calcChance(dice), 25);
    
    printf("\n");
    exit(1);
}

