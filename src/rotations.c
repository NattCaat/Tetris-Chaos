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


// Functions which should only by accessable by this file

void copyTetri(u_char target[4][4], u_char source[4][4]);
void rotBorder(u_char tetri[4][4], int yMin, int xMin, int yMax, int xMax);
int cmpRow(u_char tetri[4][4], u_char row[4], int index);
void translate(u_char tetri[4][4], int y, int x);
void initMathRot(u_char tetriminos[7][4][4][4], int *startRot);


u_char baseTetriminos[7][4][4] = {
    // I Tetrimino
    {{0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0}},
    // O Tetrimino
    {{0, 2, 2, 0},
    {0, 2, 2, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}},
    // J Tetrimino
    {{0, 0, 0, 0},
    {3, 3, 3, 0},
    {0, 0, 3, 0},
    {0, 0, 0, 0}},
    // L Tetrimino
    {{0, 0, 0, 0},
    {4, 4, 4, 0},
    {4, 0, 0, 0},
    {0, 0, 0, 0}},
    // S Tetrimino
    {{0, 5, 5, 0},
    {5, 5, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}},
    // Z Tetrimino
    {{6, 6, 0, 0},
    {0, 6, 6, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}},
    // T Tetrimino
    {{0, 0, 0, 0},
    {7, 7, 7, 0},
    {0, 7, 0, 0},
    {0, 0, 0, 0}}
};

int wallKicksDTET[6][2] = {
    { 0,  0}, { 0,  1}, { 0, -1}, { 1,  0}, { 1,  1}, { 1, -1}
};


// Copy tetrimino from source into target
void copyTetri(u_char target[4][4], u_char source[4][4])
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            target[i][j] = source[i][j];
        }
    }
}

// Rotate nxn ring of array clockwise
void rotBorder(u_char tetri[4][4], int yMin, int xMin, int yMax, int xMax)
{
    u_char (*tmpTetri)[4];
    tmpTetri = calloc(4, sizeof(u_char(*)[4]));
    copyTetri(tmpTetri, tetri);
    for (int i = xMin; i < xMax+1; i++) {
        tetri[yMin+i-xMin][xMax] = tmpTetri[yMin][i];
        tetri[yMin+i-xMin][xMin] = tmpTetri[yMax][i];
    }
    for (int i = yMin; i < xMax+1; i++) {
        tetri[yMin][xMax-i+yMin] = tmpTetri[i][xMin];
        tetri[yMax][xMax-i+yMin] = tmpTetri[i][xMax];
    }
    free(tmpTetri);
}

// Compare the row "index" of "tetri" with "row"
// Return 1 if the rows have identical values, overwise return 0
int cmpRow(u_char tetri[4][4], u_char row[4], int index) 
{
    for (int i = 0; i < 4; i++) {
        if (tetri[index][i] != row[i]) {
            return 0;
        }
    }
    return 1;
}

// Translate values of "tetri" with help of parameters "y" and "x"
void translate(u_char tetri[4][4], int y, int x)
{
    u_char (*tmpTetri)[4];
    tmpTetri = calloc(4, sizeof(u_char(*)[4]));
    copyTetri(tmpTetri, tetri);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tetri[i][j] = tmpTetri[(i-y+4)%4][(j-x+4)%4];
        }
    }
    free(tmpTetri);
}


/********************* INITIALIZE ROTATION SYSTEMS *********************/

// Base rotation of each tetrimino
void initMathRot(u_char tetriminos[7][4][4][4], int *startRot)
{
    *startRot = 0;

    for (int i = 0; i < 7; i++) {
        copyTetri(tetriminos[i][0], baseTetriminos[i]);
        for (int j = 1; j < 4; j++) {
            copyTetri(tetriminos[i][j], tetriminos[i][j-1]);
            if (i != 1) {
                if (i == 0) {
                    rotBorder(tetriminos[i][j], 0, 0, 3, 3);
                    rotBorder(tetriminos[i][j], 1, 1, 2, 2);
                } else {
                    rotBorder(tetriminos[i][j], 0, 0, 2, 2);
                }
            }
        }
    }
}

// Rotation system used in DTET fan game
void initDTET(u_char tetriminos[7][4][4][4], int *startRot) 
{
    initMathRot(tetriminos, startRot);
    *startRot = 0;
    u_char emptyRow[4] = {0, 0, 0, 0};

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 4; j++) {
            if (cmpRow(tetriminos[i][j], emptyRow, 2) && i != 1) {
                translate(tetriminos[i][j], 1, 0);
            }
        }
    } 
}

 
// Rotation system used in the NES version
void initNES(u_char tetriminos[7][4][4][4], int *startRot)
{
    initMathRot(tetriminos, startRot);
    *startRot = 0;
    translate(tetriminos[0][0], 1, 0);
    translate(tetriminos[0][3], 0, 1);
    translate(tetriminos[4][0], 1, 0);
    translate(tetriminos[4][3], 0, 1);
    translate(tetriminos[5][0], 1, 0);
    translate(tetriminos[5][3], 0, 1);
}

// Rotation system used in the Gameboy version
void initGB(u_char tetriminos[7][4][4][4], int *startRot)
{
    initMathRot(tetriminos, startRot);
    *startRot = 0;
    translate(tetriminos[0][0], 1, 0);
    translate(tetriminos[0][1], 0, -1);
    translate(tetriminos[4][0], 1, 0);
    translate(tetriminos[4][1], 0, -1);
    translate(tetriminos[5][0], 1, 0);
    translate(tetriminos[5][1], 0, -1);
}

// Rotation system used by Sega
void initSega(u_char tetriminos[7][4][4][4], int *startRot)
{
    initDTET(tetriminos, startRot);
    translate(tetriminos[0][0], -1, 0);
    translate(tetriminos[0][2], -1, 0);
    translate(tetriminos[0][3], 0, 1);
    translate(tetriminos[4][1], 0, -1);
    translate(tetriminos[5][3], 0, 1);
}

// Rotation system used in the original Tetris games
void initOriginal(u_char tetriminos[7][4][4][4], int *startRot)
{
    initNES(tetriminos, startRot);
    translate(tetriminos[0][0], -1, 0);
    translate(tetriminos[0][2], -1, 0);
}

/****************** FUNCTIONS TO ROTATE THE _TETRIMINOS ******************/

// Perform a simple rotation without wallkicking
// Valid values for rotDelta: -1, 0, 1
void rotateSimple(Tetrimino *current, int rotDelta)
{
    // Small check to avoid weird bugs
    if (rotDelta < -1 || rotDelta > 1) {
        return;
    }

    int savRot = current->rot;
    current->rot = (current->rot + rotDelta + 4) % 4;
    if (!checkTetri(current->pos[0], current->pos[1])) {
        current->rot = savRot;
    }
}

// Perform a rotation and test for wallkick positions
// Valid values for rotDelta: -1, 0, 1
void rotateDTET(Tetrimino *current, int rotDelta)
{
    // Small check to avoid weird bugs
    if (rotDelta < -1 || rotDelta > 1) {
        return;
    }

    int success = 0;
    int yDelta = 0;
    int xDelta = 0;
    int savRot = current->rot;
    current->rot = (current->rot + rotDelta + 4) % 4;
    
    // Iterate through wall kick positions up until rotation was successfully
    for (int i = 0; i < 6; i++) {
        yDelta = wallKicksDTET[i][0];
        xDelta = wallKicksDTET[i][1] * rotDelta;
        if (checkTetri(current->pos[0]+yDelta, current->pos[1]+xDelta)) {
            current->pos[0] += yDelta;
            current->pos[1] += xDelta;
            success = 1;
            break;
        }
    }
    // If rotation failed, revert 
    if (!success) {
        current->rot = savRot;
    }
}