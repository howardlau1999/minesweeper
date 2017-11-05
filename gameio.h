#pragma once
#include <stdio.h>
#include "board.h"

#define OP_UNVEAL 'o'
#define OP_FLAG 'f'
#define OP_RESTART 'r'

#define WIN 1
#define LOSE 2

void initGame(struct Board *board);
int checkInputSize(const int width, const int height);
void inputSize(int *width, int *height, int *mines);
int game(struct Board *board);