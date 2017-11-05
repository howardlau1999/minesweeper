#include "display.h"

// get the character corresponding to the status
char getDisplayChar(int status) {
	char c;

	c = '0' + getNumber(status);
	if (getNumber(status) == 0) c = CHAR_EMPTY;
	if (isMine(status)) c = CHAR_MINE;
	if (isHidden(status)) c = CHAR_HIDDEN;
	if (isFlag(status)) c = CHAR_FLAG;

	return c;
}

// print a horizontal line splitting lines
void printHorizontalLine(int boardWidth) {
	for (int x = 0; x < boardWidth * 2 + 1; x++) {
		putchar(CHAR_HORIZONTAL_LINE);
	}
	putchar('\n');
}

// clear the screen and print the board out
void printBoard(struct Board *board) {
	system("cls");
	int width = board->width;
	int height = board->height;

	for (int y = 0; y < height; y++) {
		printHorizontalLine(width);
		for (int x = 0; x < width; x++) {
			char c = getDisplayChar(board->blocks[x][y].status);
			putchar(CHAR_VERTICAL_LINE);
			putchar(c);
		}
		putchar(CHAR_VERTICAL_LINE);
		putchar('\n');
	}
	printHorizontalLine(width);
}

