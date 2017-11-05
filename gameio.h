#pragma once
#include <stdio.h>
#include "board.h"

#define OP_UNVEAL 'o'
#define OP_FLAG 'f'
#define OP_RESTART 'r'

#define WIN 1
#define LOSE 2

#define HELP_TEXT "Welcome to MineSweeper!\nOperations: \n\no x,y = Open a block on x,y\n\nPerform it on a number which neighbours \nhave been correctly flagged \ncan open all its closed neighbours\n\nf x,y = Flag a block\n\nHave Fun!\n\n"

void initGame(struct Board *board);
int checkInputSize(const int width, const int height);
void inputSize(int *width, int *height, int *mines);
int game(struct Board *board);