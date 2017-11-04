#include <stdio.h>
#include <memory.h>
#include <stdlib.h>      
#include <time.h> 

#define CHAR_MINE 'X'
#define CHAR_FLAG '#'
#define CHAR_HIDDEN '?'
#define CHAR_VERTICAL_LINE '|'
#define CHAR_HORIZONTAL_LINE '-'
#define CHAR_EMPTY ' '

#define MINE 0x1000
#define MINE_MASK 0x1000
#define FLAG_MASK 0x0100
#define UNVEAL_MASK 0x0010
#define NUMBER_MASK 0x000F

#define OP_UNVEAL 1
#define OP_FLAG 2

#define WIN 1
#define LOSE 2

#define MAX_MINES_RATIO 0.8
#define MIN_MINES_RATIO 0.05 

struct Block {
	int status;
};

struct Board {
	struct Block blocks[99][99];
	int width, height;
	unsigned int mines, flags;

};

int checkSize(const int width, const int height) {
	if (width < 10 || width > 99 || height < 10 || height > 99)
		return 0;
	else
		return 1;
}

void inputSize(int *width, int *height, int *mines) {
	do {
		printf("Please input width and height, each within range [10, 99]: ");
		scanf_s("%d %d", width, height);
	} while (!checkSize(*width, *height));
	
	int minMines = (*width) * (*height) * MIN_MINES_RATIO;
	int maxMines = (*width) * (*height) * MAX_MINES_RATIO;
	
	do {
		printf("How many mines in this board? [%d, %d]: ", minMines, maxMines);
		scanf_s("%d", mines); 
	} while (*mines < minMines || *mines > maxMines);
}

int* generatePositions(const int width, const int height, const int mines) {
	int maxPos = width * height;
	int* positions = malloc(sizeof(int) * mines);
	char* flags = malloc(sizeof(char) * maxPos);
	
	memset(positions, 0, sizeof(int) * mines);
	memset(flags, 0, sizeof(char) * maxPos);
	srand((unsigned)time(0));
		
	for (int i = 0, pos; i < mines; i++) {
		
		do {
			pos = (double)rand() / (RAND_MAX + 1) * maxPos;
		} while(flags[pos]);
		
		flags[pos] = 1;
		positions[i] = pos;
	}
	
	return positions;
}

void printBoard(struct Board *board) {
	system("cls");
	int width = board->width;
	int height = board->height;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width * 2 + 1; x++) {
			putchar(CHAR_HORIZONTAL_LINE);
		}

		putchar('\n');

		for (int x = 0; x < width; x++) {
			char c;
			int status = board->blocks[x][y].status;

			int isMine = status & MINE_MASK;
			int isHidden = !(status & UNVEAL_MASK);
			int isFlag = status & FLAG_MASK;

			c = '0' + (status & NUMBER_MASK);
			if (!(status & NUMBER_MASK)) c = CHAR_EMPTY;
			if (isMine) c = CHAR_MINE;
			if (isHidden) c = CHAR_HIDDEN;
			if (isFlag) c = CHAR_FLAG;
			putchar(CHAR_VERTICAL_LINE);
			putchar(c);
		}

		putchar(CHAR_VERTICAL_LINE);
		putchar('\n');
	}

	for (int x = 0; x < width * 2 + 1; x++) {
		putchar(CHAR_HORIZONTAL_LINE);
	}

	putchar('\n');
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

void initBoard(struct Board *board, const int width, const int height, const int mines) {
	memset(board, 0, sizeof(*board));
	int *positions = generatePositions(width, height, mines);
	board->width = width;
	board->height = height;
	board->mines = mines;
	
	setMines(board, positions);
	free(positions);
}

void initGame(struct Board *board) {
	int width, height, mines;
	
	inputSize(&width, &height, &mines);
	initBoard(board, width, height, mines);
}

int canOpen(int status) {
	int isMine = status & MINE_MASK;
	int isHidden = !(status & UNVEAL_MASK);
	int isFlag = status & FLAG_MASK;

	return isHidden && !isFlag && !isMine;
}

void flagBlock(struct Board *board, const int x, const int y) {
	int status = board->blocks[x][y].status;

	if (status & UNVEAL_MASK) return;

	if (status & FLAG_MASK)
		board->blocks[x][y].status &= (~FLAG_MASK);
	else
		board->blocks[x][y].status |= FLAG_MASK;
}

void unvealAllBlocks(struct Board *board) {
	for (int x = 0; x < board->width; x++) {
		for (int y = 0; y < board->height; y++) {
			board->blocks[x][y].status |= UNVEAL_MASK;
		}
	}
}

int openBlock(struct Board *board, const int x, const int y) {
	int status = board->blocks[x][y].status;
	int width = board->width;
	int height = board->height;
	int mines = board->mines;

	if (status & FLAG_MASK) return 0;
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
						if (canOpen(board->blocks[numX][numY].status)){
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



int checkWin(struct Board *board) {
	int width = board->width;
	int height = board->height;
	int mines = board->mines;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			int status = board->blocks[x][y].status;
			if (status & MINE_MASK) {
				mines--;
				for (int deltaX = -1; deltaX <= 1; deltaX++) {
					for (int deltaY = -1; deltaY <= 1; deltaY++) {
						int numX = x + deltaX;
						int numY = y + deltaY;
						if (!(numX < 0) && !(numY < 0) && numX < width && numY < height)
							if (!(board->blocks[numX][numY].status & UNVEAL_MASK) && !(board->blocks[numX][numY].status & MINE_MASK))
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
}

int game(struct Board *board) {
	printBoard(board);
	int x, y, result;
	char op;
	do {
		getchar();
		op = getchar();
		scanf_s("%d,%d", &x, &y);
	} while (x < 0 || y < 0 || x > board->width || y > board->height || (op != 'f' && op != 'o' && op != 'r'));

	switch (op) {
	case 'f':
		flagBlock(board, x - 1, y - 1);
		result = 0;
		break;
	case 'o':
		result = openBlock(board, x - 1, y - 1);
		if (result == 0) result = checkWin(board);
		break;
	case 'r':
		system("cls");
		initGame(board);
		result = 0;
		break;
	}
	return result;
}

int main() {
	struct Board board;
	while (1) {
		system("cls");
		initGame(&board);

		int gameStatus = 1;

		while (!(gameStatus = game(&board)));

		printBoard(&board);

		switch (gameStatus) {
		case LOSE:
			printf("YOU LOSE!");
			break;// printStatistics();
		case WIN:
			printf("YOU WIN!");
			break;
		}

		char ch = getchar();
		ch = getchar();
		if (ch == 'q') return 0;
	}
	

	
	return 0;
}
