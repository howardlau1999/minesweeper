#pragma once
#include <stdio.h>
#include "board.h"

#define CHAR_MINE 'X'
#define CHAR_FLAG '#'
#define CHAR_HIDDEN '?'
#define CHAR_VERTICAL_LINE '|'
#define CHAR_HORIZONTAL_LINE '-'
#define CHAR_EMPTY ' '

void printBoard(struct Board *board);
