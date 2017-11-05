#include "board.h"
#include "gameio.h"

int* generatePositions(const int width, const int height, const int mines) {
	int maxPos = width * height;
	int* positions = malloc(sizeof(int) * mines);
	char* flags = malloc(sizeof(char) * maxPos);

	memset(positions, 0, sizeof(int) * mines);
	memset(flags, 0, sizeof(char) * maxPos);
	srand((unsigned)time(0));

	for (int i = 0, pos; i < mines; i++) {

		do {
			pos = (int)((double)rand() / (RAND_MAX + 1) * maxPos);
		} while (flags[pos]);

		flags[pos] = 1;
		positions[i] = pos;
	}

	free(flags);
	return positions;
}

int openBlock(struct Board *board, const int x, const int y) {
	int status = board->blocks[x][y].status;
	int width = board->width;
	int height = board->height;
	int mines = board->mines;

	if (isFlag(status)) return 0;
	if (isMine(status)) {
		unvealAllBlocks(board);
		return LOSE;
	}
	if (isUnvealed(status)) {
		if (getNumber(status)) {
			int flags = 0;
			for (int deltaX = -1; deltaX <= 1; deltaX++) {
				for (int deltaY = -1; deltaY <= 1; deltaY++) {
					int numX = x + deltaX;
					int numY = y + deltaY;
					if (withinBorder(board, numX, numY))
						if (isFlag(board->blocks[numX][numY].status)) {
							flags++;
						}
				}
			}

			if (flags != (getNumber(status))) return 0;

			for (int deltaX = -1; deltaX <= 1; deltaX++) {
				for (int deltaY = -1; deltaY <= 1; deltaY++) {
					int numX = x + deltaX;
					int numY = y + deltaY;
					if (withinBorder(board, numX, numY))
						if (canOpen(board->blocks[numX][numY].status)) {
							int result = openBlock(board, numX, numY);
							if (result) return LOSE;
						}
				}
			}

		}
		else return 0;
	}

	setUnveal(board->blocks[x][y]);

	int *stack = malloc(sizeof(int) * width * height * 20);
	memset(stack, 0, sizeof(int) * width * height * 20);
	int top = 0;

	stack[top++] = x;
	stack[top++] = y;

	while (top) {
		// beware of the scope of y, x, status !!
		int sy = stack[--top];
		int sx = stack[--top];
		int sstatus = board->blocks[sx][sy].status;

		for (int deltaX = -1; deltaX <= 1; deltaX++) {
			for (int deltaY = -1; deltaY <= 1; deltaY++) {
				if (abs(deltaX) + abs(deltaY) == 2) continue;
				int numX = sx + deltaX;
				int numY = sy + deltaY;
				int shouldOpen = 0;
				if (withinBorder(board, numX, numY) && canOpen(board->blocks[numX][numY].status)) {
					if (getNumber(sstatus)) {
						if (!getNumber(board->blocks[numX][numY].status))
							shouldOpen = 1;
					}
					else {
						shouldOpen = 1;
					}
				}

				if (shouldOpen) {
					stack[top++] = numX;
					stack[top++] = numY;
					setUnveal(board->blocks[numX][numY]);
				}
			}
		}
	}
	
	free(stack);

	return 0;
}

void unvealAllBlocks(struct Board *board) {
	for (int x = 0; x < board->width; x++) {
		for (int y = 0; y < board->height; y++) {
			setUnveal(board->blocks[x][y]);
		}
	}
}

void flagBlock(struct Board *board, const int x, const int y) {
	int status = board->blocks[x][y].status;

	if (isUnvealed(status)) return;

	if (isFlag(status))
		unsetFlag(board->blocks[x][y]);
	else
		setFlag(board->blocks[x][y]);
}

inline int canOpen(int status) {
	return isHidden(status) && (!isFlag(status)) && (!isMine(status));
}

int checkWin(struct Board *board) {
	int width = board->width;
	int height = board->height;
	int mines = board->mines;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int status = board->blocks[x][y].status;
			if (isMine(status)) {
				mines--;
				for (int deltaX = -1; deltaX <= 1; deltaX++) {
					for (int deltaY = -1; deltaY <= 1; deltaY++) {
						int numX = x + deltaX;
						int numY = y + deltaY;
						if (withinBorder(board, numX, numY))
							if (!(isUnvealed(board->blocks[numX][numY].status)) && !(isMine(board->blocks[numX][numY].status)))
								return 0;
					}
				}
			}
			if (!mines) {
				unvealAllBlocks(board);
				return WIN;
			}
		}
	}

	return 0;
}

void initBoard(struct Board *board, const int width, const int height, const int mines) {
	memset(board, 0, sizeof(*board));
	int *positions = generatePositions(width, height, mines);
	board->width = width;
	board->height = height;
	board->mines = mines;

	setMines(board, positions);
	free(positions);
}

void setMines(struct Board *board, const int *positions) {
	int width = board->width;
	int height = board->height;
	int mines = board->mines;

	for (int i = 0; i < mines; i++) {
		int x = positions[i] % width;
		int y = positions[i] / width;
		board->blocks[x][y].status = MINE;

		for (int deltaX = -1; deltaX <= 1; deltaX++) {
			for (int deltaY = -1; deltaY <= 1; deltaY++) {
				int numX = x + deltaX;
				int numY = y + deltaY;
				if (withinBorder(board, numX, numY))
					if (board->blocks[numX][numY].status != MINE)
						board->blocks[numX][numY].status++;
			}
		}
	}
}

inline int withinBorder(struct Board *board, const int x, const int y) {
	return !(x < 0) && !(y < 0) && x < board->width && y < board->height;
}