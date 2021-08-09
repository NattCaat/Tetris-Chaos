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
#include <stdlib.h>
#include "../include/globals.h"
#include "../include/interfuncs.h"
#include "../include/interface.h"


WINDOW *menuWin;
char options[][11] = {
    "   PLAY   ",
    " SETTINGS "
};
const int spawnPosStatesNum = 2;
char spawnPosStates[2][11] = {
    "    Top   ",
    "  Bottom  "
};
const int isHoldOnStatesNum = 2;
char isHoldOnStates[2][11] = {
    "    Off   ",
    "    On    "
};
const int difficultyStatesNum = 4;
char difficultyStates[4][11] = {
    "   Easy   ",
    "  Normal  ",
    "   Hard   ",
    "Impossible"
};

int settingRotSys = 0;
int settingOrientation = 0;
int settingIsHoldOn = 1;
int settingDifficult = 1;


void mainMenu()
{
    // Create window and initialize options
    menuWin = newwin(22, 45, 0, 0);
    keypad(menuWin, 1);
    box(menuWin, ACS_VLINE, ACS_HLINE);
    int maxX = getmaxx(menuWin);
    int choice = 0;
    int inp = -1;
    int optNum = 2;
    // Print a big "_TETRIS" on the top of the screen
    // Now, I can finally brag about the huge number of lines in my programs
    wattron(menuWin, COLOR_PAIR(I_TETRI));
    mvwdrawbox(menuWin, 2, 2, ' ', 1, 6);
    mvwdrawbox(menuWin, 3, 4, ' ', 4, 2);
    wattroff(menuWin, COLOR_PAIR(I_TETRI));
    wattron(menuWin, COLOR_PAIR(Z_TETRI));
    mvwdrawbox(menuWin, 2, 9, ' ', 5, 2);
    mvwdrawbox(menuWin, 2, 11, ' ', 1, 4);
    mvwdrawbox(menuWin, 4, 11, ' ', 1, 3);
    mvwdrawbox(menuWin, 6, 11, ' ', 1, 4);
    wattroff(menuWin, COLOR_PAIR(Z_TETRI));
    wattron(menuWin, COLOR_PAIR(L_TETRI));
    mvwdrawbox(menuWin, 2, 16, ' ', 1, 6);
    mvwdrawbox(menuWin, 3, 18, ' ', 4, 2);
    wattroff(menuWin, COLOR_PAIR(L_TETRI));
    wattron(menuWin, COLOR_PAIR(S_TETRI));
    mvwdrawbox(menuWin, 2, 23, ' ', 5, 2);
    mvwdrawbox(menuWin, 2, 27, ' ', 3, 2);
    mvwdrawbox(menuWin, 2, 25, ' ', 1, 2);
    mvwdrawbox(menuWin, 4, 25, ' ', 1, 2);
    mvwdrawbox(menuWin, 5, 26, ' ', 1, 2);
    mvwdrawbox(menuWin, 6, 27, ' ', 1, 2);
    wattroff(menuWin, COLOR_PAIR(S_TETRI));
    wattron(menuWin, COLOR_PAIR(J_TETRI));
    mvwdrawbox(menuWin, 2, 30, ' ', 1, 6);
    mvwdrawbox(menuWin, 6, 30, ' ', 1, 6);
    mvwdrawbox(menuWin, 3, 32, ' ', 3, 2);
    wattroff(menuWin, COLOR_PAIR(J_TETRI));
    wattron(menuWin, COLOR_PAIR(T_TETRI));
    mvwdrawbox(menuWin, 2, 37, ' ', 3, 2);
    mvwdrawbox(menuWin, 4, 41, ' ', 3, 2);
    mvwdrawbox(menuWin, 2, 39, ' ', 1, 4);
    mvwdrawbox(menuWin, 4, 39, ' ', 1, 2);
    mvwdrawbox(menuWin, 6, 37, ' ', 1, 4);
    wattroff(menuWin, COLOR_PAIR(T_TETRI));

    do {
        for (int i = 0; i < optNum; i++) {
            if (choice == i) {
                wattron(menuWin, A_STANDOUT);
            }
            mvwprintw(menuWin, 12+2*i, (maxX/2)-5, *(options+i));
            wattroff(menuWin, A_STANDOUT);
        }

        wrefresh(menuWin);
        wtimeout(menuWin, -1);
        inp = wgetch(menuWin);
        if (inp == KEY_UP) {
            choice = (choice - 1 + optNum) % optNum;
        } else if (inp == KEY_DOWN) {
            choice = (choice + 1) % optNum;
        }
    } while (inp != 10);
    // Enter setting menu, if user want to
    if (choice == 1) {
        settingMenu();
    }
    // Display controls
    clearWin(menuWin);
    box(menuWin, ACS_VLINE, ACS_HLINE);
    wattron(menuWin, A_BOLD | A_STANDOUT);
    mvwprintw(menuWin, 0, (maxX/2)-5, " CONTROLS ");
    wattroff(menuWin, A_BOLD | A_STANDOUT);
    mvwprintw(menuWin, 4, 2, "Pause                      ESCAPE");
    mvwprintw(menuWin, 6, 2, "Move left                  LEFT ARROW");
    mvwprintw(menuWin, 8, 2, "Move right                 RIGHT ARROW");
    mvwprintw(menuWin, 10, 2, "Rotate clockwise           UP ARROW");
    mvwprintw(menuWin, 12, 2, "Rotate counterclockwise    X");
    mvwprintw(menuWin, 14, 2, "Soft drop                  DOWN ARROW");
    mvwprintw(menuWin, 16, 2, "Hard drop                  SPACE");
    mvwprintw(menuWin, 18, 2, "Hold                       C");
    wrefresh(menuWin);
    wgetch(menuWin);
    // Clear window completely and delete it
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 45; j++) {
            mvwprintw(menuWin, i, j, " ");
        }
    }
    wrefresh(menuWin);
    delwin(menuWin);
}


void settingMenu()
{
    int choice = 0;
    int inp = -1;
    int optNum = 4;

    int **settings = calloc(optNum, sizeof(int(*)));
    *settings = &settingRotSys;
    *(settings+1) = &settingOrientation;
    *(settings+2) = &settingIsHoldOn;
    *(settings+3) = &settingDifficult;

    int *settingLens = calloc(optNum, sizeof(int));
    *settingLens = rotSystemsLen;
    *(settingLens+1) = spawnPosStatesNum;
    *(settingLens+2) = isHoldOnStatesNum;
    *(settingLens+3) = difficultyStatesNum;

    clearWin(menuWin);
    wattron(menuWin, A_BOLD | A_STANDOUT);
    mvwprintw(menuWin, 0, (getmaxx(menuWin)/2)-5, " SETTINGS ");
    wattroff(menuWin, A_BOLD | A_STANDOUT);

    do {
        clearWin(menuWin);
        mvwprintw(menuWin, 4, 2, "Rotation System");
        mvwprintw(menuWin, 6, 2, "Spawn position");
        mvwprintw(menuWin, 8, 2, "Hold Tetriminos");
        mvwprintw(menuWin, 10, 2, "Difficulty");
        mvwprintw(menuWin, 4, 27, "%.*s", 11, (rotSystems+settingRotSys)->name);
        mvwprintw(menuWin, 8, 27, "%.*s", 11, *(isHoldOnStates+settingIsHoldOn));
        mvwprintw(menuWin, 6, 27, "%.*s", 11, *(spawnPosStates+settingOrientation));
        mvwprintw(menuWin, 10, 27, "%.*s", 11, *(difficultyStates+settingDifficult));

        for (int i = 0; i < optNum; i++) {
            if (i == choice) {
                mvwprintw(menuWin, 4+2*i, 25, "<");
                mvwprintw(menuWin, 4+2*i, 38, ">");
            }
        }
        wrefresh(menuWin);

        inp = wgetch(menuWin);

        switch(inp) {
            // Change value of selected setting option
            case KEY_RIGHT:
                **(settings+choice) = 
                    (**(settings+choice) + 1) % *(settingLens+choice);
                break;
            case KEY_LEFT:
                **(settings+choice) = 
                    (**(settings+choice) - 1 + *(settingLens+choice)) % *(settingLens+choice);
                break;
            // Change selected setting option
            case KEY_DOWN:
                choice = (choice + 1) % optNum;
                break;
            case KEY_UP:
                choice = (choice - 1 + optNum) % optNum;
                break;
            // Do nothing
            default:
                break;
        }
    } while (inp != 10);

    free(settings);
    free(settingLens);


    /*mvwprintw(menuWin, 12, 2, "%i columns", getmaxx(stdscr));
    mvwprintw(menuWin, 14, 2, "%i lines", getmaxy(stdscr));
    wrefresh(menuWin);
    wgetch(menuWin);*/
}