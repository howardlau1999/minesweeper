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

	return positions;
}

int openBlock(struct Board *board, const int x, const int y) {
	int status = board->blocks[x][y].status;
	int width = board->width;
	int height = board->height;
	int mines = board->mines;

	if (isFlag(status)) return 0;
	if (status & UNVEAL_MASK) {
		if (status & NUMBER_MASK) {
			int flags = 0;
			for (int deltaX = -1; deltaX <= 1; deltaX++) {
				for (int deltaY = -1; deltaY <= 1; deltaY++) {
					int numX = x + deltaX;
					int numY = y + deltaY;
					if (!(numX < 0) && !(numY < 0) && numX < width && numY < height)
						if (board->blocks[numX][numY].status & FLAG_MASK) {
							flags++;
						}
				}
			}

			if (flags != (status & NUMBER_MASK)) return 0;

			for (int deltaX = -1; deltaX <= 1; deltaX++) {
				for (int deltaY = -1; deltaY <= 1; deltaY++) {
					int numX = x + deltaX;
					int numY = y + deltaY;
					if (!(numX < 0) && !(numY < 0) && numX < width && numY < height)
						if (canOpen(board->blocks[numX][numY].status)) {
							int result = openBlock(board, numX, numY);
							if (result) return LOSE;
						}
				}
			}

		}
		else return 0;
	}
	if (status & MINE_MASK) {
		unvealAllBlocks(board);
		return LOSE;
	}

	board->blocks[x][y].status |= UNVEAL_MASK;

	for (int deltaX = -1; deltaX <= 1; deltaX++) {
		for (int deltaY = -1; deltaY <= 1; deltaY++) {
			if (abs(deltaX) + abs(deltaY) == 2) continue;
			int numX = x + deltaX;
			int numY = y + deltaY;
			if (!(numX < 0) && !(numY < 0) && numX < width && numY < height)
				if (canOpen(board->blocks[numX][numY].status))
					if (status & NUMBER_MASK) {
						if (!(board->blocks[numX][numY].status & NUMBER_MASK))
							openBlock(board, numX, numY);
					}
					else {
						openBlock(board, numX, numY);
					}
		}
	}

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
						if (!(numX < 0) && !(numY < 0) && numX < width && numY < height)
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
				if (!(numX < 0) && !(numY < 0) && numX < width && numY < height)
					if (board->blocks[numX][numY].status != MINE)
						board->blocks[numX][numY].status++;
			}
		}
	}
}
