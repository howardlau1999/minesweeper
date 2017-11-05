#include "gameio.h"
#include "display.h"

void initGame(struct Board *board) {
	int width, height, mines;

	inputSize(&width, &height, &mines);
	initBoard(board, width, height, mines);
}

int checkInputSize(const int width, const int height) {
	if (width < MIN_WIDTH || width > MAX_WIDTH || height < MIN_HEIGHT || height > MAX_HEIGHT)
		return 0;
	else
		return 1;
}

void inputSize(int *width, int *height, int *mines) {
	do {
		printf("Please input width and height, each within range [10, 99]: ");
		scanf_s("%d %d", width, height);
	} while (!checkInputSize(*width, *height));

	int minMines = (int)((*width) * (*height) * MIN_MINES_RATIO);
	int maxMines = (int)((*width) * (*height) * MAX_MINES_RATIO);

	do {
		printf("How many mines in this board? [%d, %d]: ", minMines, maxMines);
		scanf_s("%d", mines);
	} while (*mines < minMines || *mines > maxMines);
}

int checkInputOperation(const char op, const int x, const int y, const struct Board *board) {
	return x < 0 ||
		y < 0 ||
		x > board->width ||
		y > board->height ||
		(op != OP_UNVEAL && op != OP_RESTART && op != OP_FLAG);
}

int game(struct Board *board) {
	printBoard(board);

	int x, y, result;
	char op;
	do {
		getchar();
		op = getchar();
		scanf_s("%d,%d", &x, &y);
	} while (checkInputOperation(op, x, y, board));

	switch (op) {
	case OP_FLAG:
		flagBlock(board, x - 1, y - 1);
		result = 0;
		break;
	case OP_UNVEAL:
		result = openBlock(board, x - 1, y - 1);
		if (result == 0) result = checkWin(board);
		break;
	case OP_RESTART:
		system("cls");
		initGame(board);
		result = 0;
		break;
	}

	return result;
}