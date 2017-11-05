#pragma once
#include <memory.h>
#include <stdlib.h>      
#include <time.h>

struct Block {
	int status;
};

struct Board {
	struct Block blocks[99][99];
	int width, height;
	unsigned int mines, flags;
};

#define MINE 0x1000
#define MINE_MASK 0x1000
#define FLAG_MASK 0x0100
#define UNVEAL_MASK 0x0010
#define NUMBER_MASK 0x000F

#define MAX_MINES_RATIO 0.8
#define MIN_MINES_RATIO 0.1 
#define MAX_WIDTH 99
#define MIN_WIDTH 10
#define MAX_HEIGHT 99
#define MIN_HEIGHT 10

// MASK OPERATIONS

#define isMine(status) (status & MINE_MASK)
#define isHidden(status) (!(status & UNVEAL_MASK))
#define isFlag(status) (status & FLAG_MASK)
#define isUnvealed(status) (status & UNVEAL_MASK)

#define setUnveal(block) (block.status |= UNVEAL_MASK)
#define setFlag(block) (block.status |= FLAG_MASK)
#define unsetFlag(block) (block.status &= (~FLAG_MASK))

#define getNumber(status) (status & NUMBER_MASK)
inline int canOpen(int status);

int* generatePositions(const int width, const int height, const int mines);
void setMines(struct Board *board, const int *positions);
void initBoard(struct Board *board, const int width, const int height, const int mines);

void unvealAllBlocks(struct Board *board);
void flagBlock(struct Board *board, const int x, const int y);
int openBlock(struct Board *board, const int x, const int y);
int checkWin(struct Board *board);