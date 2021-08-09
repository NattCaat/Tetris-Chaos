#ifndef GLOBALS_H
#define GLOBALS_H

/******************************** MACROS *******************************/
#define u_char unsigned char
#define len(a) sizeof(a)/sizeof(*a)

#define ESCAPE 1
#define SOFT_DROP 2
#define HARD_DROP 3
#define MOV_R 4
#define MOV_L 5
#define ROT_R 6
#define ROT_L 7
#define HOLD 8

/****************************** STRUCTURES *****************************/
typedef struct tetrimino {
    int pos[2];
    int id;
    int rot;
} Tetrimino;

typedef struct rotSystem {
    void (*initFunc)(u_char tetriminos[7][4][4][4], int *startRot);
    void (*rotFunc)(Tetrimino *current, int rotDelta);
    char name[11];
} RotSystem;

/******************** GLOBAL VALUES USED FOR SETTINGS ******************/
extern int settingRotSys;
extern int settingOrientation;
extern int settingIsHoldOn;
extern int settingDifficult;

extern int rotSystemsLen;
extern RotSystem rotSystems[];

#endif