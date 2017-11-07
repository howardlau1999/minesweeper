#include <stdio.h>

#include "board.h"
#include "display.h"
#include "gameio.h"
#include "savefile.h"

int main() {
	struct Board board;
	
	while (1) {
		system("cls");
		printf(HELP_TEXT);
		initGame(&board);

		int gameStatus;

		while (!(gameStatus = game(&board))) {
			saveBoard(&board, SAVEFILE);
		}

		printBoard(&board);

		switch (gameStatus) {
		case LOSE:
			printf("YOU LOSE!");
			break;
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
