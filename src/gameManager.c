/*
    Tetris Chaos, tetris with some special options
    Copyright (C) 2021 NattCaat (Email: nattcaat@yahoo.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    The full notice can be found there https://www.gnu.org/licenses/old-licenses/gpl-2.0.html
*/

#include <stdlib.h>
#include "../include/globals.h"
#include "../include/gameFuncs.h"
#include "../include/gameManager.h"

#define curTetri tetriminos[current.id][current.rot]


u_char gameBoard[24][14];
u_char tetriminos[7][4][4][4];

RotSystem rotSystems[5] = {
    {initOriginal, rotateSimple, " Original "},
    {initDTET, rotateDTET, "   DTET   "},
    {initNES, rotateSimple, "    NES   "},
    {initGB, rotateSimple, "    GB    "},
    {initSega, rotateSimple, "   Sega   "}
};
int rotSystemsLen = len(rotSystems);
int startRot;

int tetriOrder[7] = {0, 1, 2, 3, 4, 5, 6};
int order;

Tetrimino current;
Tetrimino next;
Tetrimino hold;

int tetriPlaced;
int isHeld;

int combo;
int clears;
int gameLevel;
int gameScore;


// Swap values of a and b
void swap(int *a, int *b) 
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Shuffle order of tetriminos
void shuffle()
{
    for (int i = 6; i > 0; i--) {
        swap(&tetriOrder[i], &tetriOrder[rand()%(i+1)]);
    }
}

// Check if tetrimino does not overlap already placed tetrimino
// Return 1 on success and 0 on failure
int checkTetri(int yPos, int xPos)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gameBoard[yPos+i][xPos+j] != 0 && curTetri[i][j] != 0) {
                return 0;
            }
        }
    }
    return 1;
}

// Place tetrimino into gameboard
// Argument "isPermanent" tells if it should be clearable by "clearCurTetri()"
// Return 1 on success and 0 on failure
int placeTetri(int isPermanent)
{
    // Check if tetrimino can be placed
    if (!checkTetri(current.pos[0], current.pos[1])) {
        return 0;
    }
    // Make tetrimino not clearable by "clearCurTetri()" if it should not
    isPermanent = (isPermanent) ? 8 : 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (curTetri[i][j] != 0) {
                gameBoard[current.pos[0]+i][current.pos[1]+j] = curTetri[i][j] + (u_char)isPermanent;
            }
        }
    }
    return 1;
}

// Remove currently moving tetrimino from board
void clearCurTetri()
{
    for (int i = current.pos[0]; i < current.pos[0]+4; i++) {
        for (int j = current.pos[1]; j < current.pos[1]+4; j++) {
            if (gameBoard[i][j] - 8 < 0) {
                gameBoard[i][j] = 0;
            }
        }
    }
}

// Change position of moving tetrimino if it does not overlap other tetriminos
// Return 1 on success and 0 on failure
int moveTetri(int yDelta, int xDelta)
{
    // Small check to make sure the next check does not go out of bounce
    if (current.pos[0]+yDelta < 0 || current.pos[0]+yDelta > 20
      || current.pos[1]+xDelta < 0 || current.pos[1]+xDelta > 10) {
        return 0;
    }

    // Check if tetrimino does not overlap an other one on new position
    if (checkTetri(current.pos[0]+yDelta, current.pos[1]+xDelta)) {
        current.pos[0] += yDelta;
        current.pos[1] += xDelta;
        return 1;
    }
    return 0;
}

// Move tetrimino down up until it touches another one, then place it permanently
void hardDrop()
{
    while (moveTetri(1, 0));
    placeTetri(1);
    tetriPlaced = 1;
}

// Function handling the hold feature
// Return 0 on failure, 1 on success and -1 if swap leads to game over
int holdTetri()
{
    // Do nothing if a tetrimino is currently held or if feature is disabled
    if (isHeld || !settingIsHoldOn) {
        placeTetri(0);
        return 0;
    }
    // Place next tetrimino if it is the first call
    if (hold.id == -1) {
        hold.id = current.id;
        hold.rot = current.rot;
        setNextId();
    // Otherwise, swap id and rotaion of hold and current
    } else {
        swap(&(current.id), &(hold.id));
        swap(&(current.rot), &(hold.rot));
    }
    // Place new tetrimino and 
    if (spawnTetri()) {
        isHeld = 1;
        return 1;
    }
    return -1;
}

// Check if any lines should be cleared and clear them. Increases on clear counters
void checkClears()
{
    int fullRow;
    combo = 0;
    // Check for every row if it does not contain a 0
    for (int i = 2; i < 22; i++) {
        fullRow = 1;
        for (int j = 2; j < 12; j++) {
            if (gameBoard[i][j] == 0) {
                fullRow = 0;
                break;
            }
        }
        // If row has no 0s, increment counters and clear line
        if (fullRow) {
            combo++;
            clears++;
            clearLine(i);
        }
    }
}

// Clear line with coordinate "y"
void clearLine(int y)
{
    // Store values in line into the line below
    for (int i = y; i > 2; i--) { 
        for (int j = 2; j < 12; j++) {
            gameBoard[i][j] = gameBoard[i-1][j];
        }
    }
    // Set top line to an empty line
    for (int j = 2; j < 12; j++) {
        gameBoard[2][j] = 0;
    }
}

// Increment "gameScore" using "combo"
// TODO: IMPLEMENT LEVEL SYSTEM
void addScore()
{
    switch (combo) {
        case 1:
            gameScore += 40 * (gameLevel + 1) * (settingDifficult + 1);
            break;
        case 2:
            gameScore += 100 * (gameLevel + 1) * (settingDifficult + 1);
            break;
        case 3:
            gameScore += 300 * (gameLevel + 1) * (settingDifficult + 1);
            break;
        case 4:
            gameScore += 1200 * (gameLevel + 1) * (settingDifficult + 1);
            break;
        default:
            break;
    }
    gameLevel = 1 + (int)(clears / 10);
}

// Store "next.id" into "current.id" and store id of next tetrimino into "next.id"
void setNextId()
{
    current.rot = startRot;
    current.id = next.id;
    if (order == 6) {
        order = -1;
        shuffle();
    }
    next.id = tetriOrder[++order];
}

// Spawn new tetrimimino on top of game board
// Return 1 on success and 0 on failure
int spawnTetri()
{
    current.pos[1] = 5;
    for (int i = 0; i < 3; i++) {
        current.pos[0] = i;
        if (placeTetri(0)) {
            return 1;
        }
    }
    return 0;
}


// Initialize game
void initGame()
{
    combo = 0;
    clears = 0;
    gameLevel = 1;
    gameScore = 0;
    // Create tetriminos with all their rotation states
    ((rotSystems+settingRotSys)->initFunc)(tetriminos, &startRot);
    // Shuffle tetri order
    shuffle();
    hold.id = -1;
    isHeld = 0;
    order = -1;
    next.id = tetriOrder[0];
    setNextId();

    // Create game game board
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 14; j++) {
            if (i < 2 || 21 < i || j < 2 || 11 < j) {
                gameBoard[i][j] = 255;
            } else {
                gameBoard[i][j] = 0;
            }
        }
    }
}

// Get data which is altered on each update call
void getOnUpdate(u_char board[24][14])
{
    // Copy gameboard into "board"
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 14; j++) {
            board[i][j] = gameBoard[i][j];
        }
    }
}

// Get data which is altered each time a tetrimino is placed
void getOnPlaced(u_char nTetri[4][4], u_char holdTetri[4][4], int *held) {
    // Copy next tetrimino into "nTetri"
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            nTetri[i][j] = tetriminos[next.id][startRot][i][j];
            if (hold.id != -1) {
                holdTetri[i][j] = tetriminos[hold.id][hold.rot][i][j];
            }
        }
    }
    *held = isHeld;
}

// Get data which is altered each time at least one row is cleared
void getOnClear(int *level, int *score)
{
    *level = gameLevel;
    *score = gameScore;
}

// Update the game and return its current state
// -1: Game Over
//  0: Updated game without major events
//  1: A tetrimino has been placed without line clears
//  2: A tetrimino has been placed and some lines have been cleared
int update(int input)
{
    clearCurTetri();
    combo = 0;
    switch(input) {
        case SOFT_DROP:
            tetriPlaced = !moveTetri(1, 0);
            break;
        case HARD_DROP:
            hardDrop();
            break;
        case MOV_R:
            moveTetri(0, 1);
            break;
        case MOV_L:
            moveTetri(0, -1);
            break;
        case ROT_R:
            ((rotSystems+settingRotSys)->rotFunc)(&current, 1);
            break;
        case ROT_L:
            ((rotSystems+settingRotSys)->rotFunc)(&current, -1);
            break;
        case HOLD:
            return holdTetri();
        default:
            break;
    }

    placeTetri(tetriPlaced);

    if (tetriPlaced) {
        tetriPlaced = 0;
        isHeld = 0;
        checkClears();
        setNextId();
        if (!spawnTetri()) {
            return -1;
        }
        if (combo) {
            addScore();
            return 2;
        }
        return 1;
    }
    return 0;
}