#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "checkit.h"
#include "yahtzee.h"

int main(void)
{
    dice_t gameDice[5];
    play_t playerOneScores[12];
    play_t playerTwoScores[12];
    
    srand((unsigned int) time(0));

#ifndef NCHECKIT
    test_cases();
#endif
    
    initDice(gameDice);
    initScores(playerOneScores);
    initScores(playerTwoScores);
    
    throwDice(gameDice, 0);
    printDice(gameDice);
    printScoreCard(playerOneScores);
    
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
    
    sortDice(dice);
    
    for(i = 0; i < 5; i++)
    {
        if(i == 0)
        {
            continue;
        }
        else if(dice[i].value != (dice[i - 1].value + 1))
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
    
    sortDice(dice);
    
    for(i = 0; i < 5; i++)
    {
        if(i == 0)
        {
            continue;
        }
        else if(dice[i].value != dice[i - 1].value)
        {
            if(dice[i].value != (dice[i - 1].value + 1))
            {
                currentStreak = 0;
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
    int valueToCompare;
    
    for(i = 0; i < 5; i++)
    {
        if(i == 0)
        {
            valueToCompare = dice[i].value;
        }
        else if(dice[i].value != valueToCompare)
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
    
    for(i = 0; i < 5; i++)
    {
        dice[i].value = (rand() % 6) + 1;
        dice[i].hold = 0;
    }
}

void printScoreCard(play_t scores[12])
{
    int i, rowNumber;
    
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
            printf("(not used)\n");
        }
    }
}

void printDice(dice_t dice[5])
{
    int i;
    
    printf(" ---   ---   ---   ---   --- \n");
    
    for(i = 0; i < 5; i++)
    {
        printf("| %d | ", dice[i].value);
    }
    
    printf("\n ---   ---   ---   ---   --- \n");
}

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
 */
void makeFiveDice(dice_t dice[5], int value1, int value2, int value3,
                  int value4, int value5)
{
    dice[0] = makeDice(value1, 0);
    dice[1] = makeDice(value2, 0);
    dice[2] = makeDice(value3, 0);
    dice[3] = makeDice(value4, 0);
    dice[4] = makeDice(value5, 0);
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
    
    makeFiveDice(dice, 1, 2, 3, 4, 3);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 1, 2, 3, 4, 6);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 2, 3, 1, 5, 4);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 5, 2, 3, 4, 5);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 6, 2, 3, 4, 5);
    checkit_int(calcSmallStraight(dice), 30);
    
    makeFiveDice(dice, 2, 3, 4, 5, 4);
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
}

