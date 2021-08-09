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
#include "../include/gameManager.h"
#include "../include/interface.h"


int main(int argc, char **argv)
{
    u_char (*board)[14];
    u_char (*nTetri)[4];
    u_char (*holdTetri)[4];
    board = calloc(24, sizeof(u_char[14]));
    nTetri = calloc(4, sizeof(u_char[4]));
    holdTetri = calloc(4, sizeof(u_char[4]));
    int hold;
    int level;
    int score;
    int inp;
    int state;

    initNcurses();
    mainMenu();

    initInterface(); 
    initGame();

    getOnClear(&level, &score);
    showOnClear(level, score);

    getOnPlaced(nTetri, holdTetri, &hold);
    showOnPlaced(nTetri, holdTetri, hold);

    update(0);
    do {
        inp = getInput();
        if (inp == ESCAPE) {
            // TODO: Pause menu
            wait();
        }
        state = update(inp); 
        getOnUpdate(board);
        showOnUpdate(board);
        if (state > 0) {
            getOnPlaced(nTetri, holdTetri, &hold);
            showOnPlaced(nTetri, holdTetri, hold);
            if (state == 2) {
                getOnClear(&level, &score);
                showOnClear(level, score);
            }
        }
    } while (state != -1);

    // TODO: Game Over screen
    wait();

    free(board);
    free(nTetri);
    free(holdTetri);
    destroyInterface();
    endNcurses();

    return 0;
}