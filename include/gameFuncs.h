#ifndef GAMEFUNCS_H
#define GAMEFUNCS_H

#include "globals.h"


// Defined in "src/gameManager.c"

void swap(int *a, int *b);
void shuffle();
int checkTetri(int yPos, int xPos); 
int placeTetri(int isPermanent);
void clearCurTetri();
int moveTetri(int yDelta, int xDelta);
void hardDrop();
int holdTetri();
void checkClears();
void clearLine(int y);
void addScore();
void setNextId();
int spawnTetri();


// Defined in "src/rotations.c"

void initDTET(u_char tetriminos[7][4][4][4], int *startRot);
void initNES(u_char tetriminos[7][4][4][4], int *startRot);
void initGB(u_char tetriminos[7][4][4][4], int *startRot);
void initSega(u_char tetriminos[7][4][4][4], int *startRot);
void initOriginal(u_char tetriminos[7][4][4][4], int *startRot);

void rotateSimple(Tetrimino *current, int rotDelta);
void rotateDTET(Tetrimino *current, int rotDelta);


#endif