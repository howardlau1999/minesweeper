#pragma once

#include <stdio.h>
#include <memory.h>
#include "board.h"

#define SAVEFILE "minesweeper.sav"

int saveBoard(struct Board *board, const char *filename);
int loadBoard(struct Board *board, const char *filename);