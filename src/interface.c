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

#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "../include/globals.h"
#include "../include/interfuncs.h"
#include "../include/interface.h"

#define CLOCKS_PER_MSEC (clock_t)(CLOCKS_PER_SEC / 1000)


WINDOW *boardWin;
WINDOW *nextTetriWin;
WINDOW *holdTetriWin;
WINDOW *scoreWin;
WINDOW *levelWin;

int gravities[4] = {500, 300, 200, 100};
clock_t buffer = 0;
clock_t startTime;


int keyBinds[] = {
    0, 27, KEY_DOWN, ' ', KEY_RIGHT, KEY_LEFT, KEY_UP, 'x', 'c'
};
int tmpScore;
int isFirstFrame;
int isSmallWindow = 1;


// Clear content of win, it does not remove borders of win
void clearWin(WINDOW *win)
{
    for (int i = 1; i < getmaxy(win)-1; i++) {
        for (int j = 1; j < getmaxx(win)-1; j++) {
            mvwprintw(win, i, j, " ");
        }
    }
}

// Draw box 
int mvwdrawbox(WINDOW *win, int y, int x, char ch, int height, int width)
{
    int state;
    for (int i = y; i < y+height; i++) {
        state = mvwhline(win, i, x, ch, width);
        if (state == ERR) {
            return ERR;
        }
    }
    return state;
}

int mvwdrawdia(WINDOW *win, int y, int x, char ch, int height, int width)
{
    int state;
    for (int i = y; i < y+height; i++) {
        state = mvwhline(win, i, x+i-y, ch, width);
        if (state == ERR) {
            return ERR;
        }
    }
    return state;
}

// Initialize environment to create single windows
void initNcurses()
{
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    start_color();

    init_pair(I_TETRI, COLOR_BLACK, COLOR_CYAN);
    init_pair(O_TETRI, COLOR_BLACK, COLOR_WHITE);
    init_pair(J_TETRI, COLOR_BLACK, COLOR_BLUE);
    init_pair(L_TETRI, COLOR_BLACK, COLOR_YELLOW);
    init_pair(S_TETRI, COLOR_BLACK, COLOR_GREEN);
    init_pair(Z_TETRI, COLOR_BLACK, COLOR_RED);
    init_pair(T_TETRI, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(BLOCKED, COLOR_RED, COLOR_BLACK);
}

// End ncurses
void endNcurses()
{
    endwin();
}

// Create needed windows
void initInterface()
{
    tmpScore = -1;
    isFirstFrame = 1;
    if (isSmallWindow) {
        boardWin = newwin(22, 22, 0, 0);
        nextTetriWin = newwin(6, 10, 6, 23);
        holdTetriWin = newwin(6, 10, 6, 34);
        levelWin = newwin(3, 10, 3, 23);
        scoreWin = newwin(3, 21, 0, 23);
    } else {
        boardWin = newwin(42, 42, 0, 0);
        nextTetriWin = newwin(10, 18, 4, 43);
        holdTetriWin = newwin(10, 18, 4, 62);
    }
    keypad(boardWin, 1);
    box(boardWin, ACS_VLINE, ACS_HLINE);
    box(nextTetriWin, ACS_VLINE, ACS_HLINE);
    box(holdTetriWin, ACS_VLINE, ACS_HLINE);
    box(levelWin, ACS_VLINE, ACS_HLINE);
    box(scoreWin, ACS_VLINE, ACS_HLINE);
    mvwprintw(nextTetriWin, 0, 2, "NEXT");
    mvwprintw(holdTetriWin, 0, 2, "HOLD");
    mvwprintw(levelWin, 0, 2, "LEVEL");
    mvwprintw(scoreWin, 0, 2, "SCORE");
}

// Destroy windows
void destroyInterface()
{
    delwin(boardWin);
    delwin(nextTetriWin);
    delwin(holdTetriWin);
    delwin(scoreWin);
    delwin(levelWin);
}

// TODO: This function should no longer exist if game is finished
void wait()
{
    wtimeout(boardWin, -1);
    wgetch(boardWin);
}

// Get user input
// This function also controls time management to imitate the gravity
// Return value indicates what should be done in next update call (values can be found in include/globals.h)
int getInput()
{
    // Set start time on first call
    if (isFirstFrame) {
        isFirstFrame = 0;
        startTime = clock();
    }

    // Get input up until a valid key has been pressed
    int inp = 0;
    int deltaTime;
    while (1) {
        // Get delta time and make sure "wtimeout()" does not get a negative argument
        deltaTime = (int)((clock()+buffer-startTime) / CLOCKS_PER_MSEC);
        if (deltaTime > *(gravities+settingDifficult)) {
            startTime = clock();
            buffer = 0;
            return SOFT_DROP;
        }
        // Apply gravity using "wtimeout()" and get player input
        wtimeout(boardWin, *(gravities+settingDifficult) - deltaTime);
        inp = wgetch(boardWin);
        // Small correction to avoid time freeze by spamming a key
        buffer += 45 * CLOCKS_PER_MSEC;

        // Reset start time and return SOFT_DROP on soft drop or timeout
        if (inp == ERR || inp == keyBinds[SOFT_DROP]) {
            startTime = clock();
            buffer = 0;
            return SOFT_DROP;
        // Reset start time on hard drop
        } else if (inp == keyBinds[HARD_DROP]) {
            startTime = clock();
            buffer = 0;
            return HARD_DROP;
        }
        // Return state defined in "include/globals.h"
        for (int i = 0; i < 9; i++) {
            if (inp == keyBinds[i]) {
                return i;
            }
        }
    }
}

// Show game changes when game has been updated
void showOnUpdate(u_char board[24][14])
{
    int rowIndex;
    int height = (isSmallWindow) ? 1 : 2;
    for (int i = 2; i < 22; i++) {
        rowIndex = settingOrientation ? (23-i) : i;
        wmove(boardWin, (i-1)*height, 1);
        for (int j = 2; j < 12; j++) {
            wattron(boardWin, COLOR_PAIR(board[rowIndex][j] % 8));
            wprintw(boardWin, "  ");
            if (!isSmallWindow) {
                wprintw(boardWin, "  ");
                mvwprintw(boardWin, 2*i-3, 4*j-7, "    ");
                wmove(boardWin, 2*i-2, 4*j-3);
            }
            wattroff(boardWin, COLOR_PAIR(board[rowIndex][j] % 8));
        }
    }
    wrefresh(boardWin);
}

// Show game changes when a tetrimino has been cleared
void showOnPlaced(u_char nTetri[4][4], u_char holdTetri[4][4], int hold)
{
    if (hold || !settingIsHoldOn) {
        wattron(holdTetriWin, COLOR_PAIR(BLOCKED));
        box(holdTetriWin, ACS_VLINE, ACS_HLINE);
        mvwprintw(holdTetriWin, 0, 2, "HOLD");
        wattroff(holdTetriWin, COLOR_PAIR(BLOCKED));
    } else {
        box(holdTetriWin, ACS_VLINE, ACS_HLINE);
        mvwprintw(holdTetriWin, 0, 2, "HOLD");
    }

    int rowIndex;
    int height = (isSmallWindow) ? 1 : 2;
    for (int i = 0; i < 4; i++) {
        rowIndex = settingOrientation ? (3-i) : i;
        wmove(nextTetriWin, (i+1)*height, 1);
        wmove(holdTetriWin, (i+1)*height, 1);
        for (int j = 0; j < 4; j++) {
            wattron(nextTetriWin, COLOR_PAIR(nTetri[rowIndex][j] % 8));
            wprintw(nextTetriWin, "  ");

            wattron(holdTetriWin, COLOR_PAIR(holdTetri[rowIndex][j] % 8));
            wprintw(holdTetriWin, "  ");

            if (!isSmallWindow) {
                wprintw(nextTetriWin, "  ");
                mvwprintw(nextTetriWin, 2*i+1, 4*j+1, "    ");
                wmove(nextTetriWin, 2*i+2, 4*j+5);

                wprintw(holdTetriWin, "  ");
                mvwprintw(holdTetriWin, 2*i+1, 4*j+1, "    ");
                wmove(holdTetriWin, 2*i+2, 4*j+5);
            }
            wattroff(nextTetriWin, COLOR_PAIR(nTetri[rowIndex][j] % 8)); 
            wattroff(holdTetriWin, COLOR_PAIR(holdTetri[rowIndex][j] % 8)); 
        }
    }
    wrefresh(nextTetriWin);
    wrefresh(holdTetriWin);
}

// Show game changes if at least one line has been cleared
void showOnClear(int level, int score)
{
    mvwprintw(levelWin, 1, 1, "%*d", 7, level);
    while (tmpScore != score) {
        mvwprintw(scoreWin, 1, 1, "%*d", 18, ++tmpScore);
        wrefresh(scoreWin);
    }
    wrefresh(levelWin);
    wrefresh(scoreWin);
}
