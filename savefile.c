#include "savefile.h"

void writeInt(FILE* fp, int num) {
	fwrite(&num, sizeof(int), 1, fp);
}

int readInt(FILE* fp) {
	int num;
	fread_s(&num, sizeof(int), sizeof(int), 1, fp);
	return num;
}

int saveBoard(struct Board *board, const char* filename) {
	int width = board->width;
	int height = board->height;
	int mines = board->mines;

	FILE* sav;
	fopen_s(&sav, filename, "wb");
	writeInt(sav, width);
	writeInt(sav, height);
	writeInt(sav, mines);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			writeInt(sav, board->blocks[x][y].status);
		}
	}
	fclose(sav);

	return 0;
}

int loadBoard(struct Board *board, const char* filename) {
	FILE* sav;
	fopen_s(&sav, filename, "rb");
	board->width = readInt(sav);
	board->height = readInt(sav);
	board->mines = readInt(sav);

	for (int x = 0; x < board->width; x++) {
		for (int y = 0; y < board->height; y++) {
			board->blocks[x][y].status = readInt(sav);
		}
	}

	fclose(sav);
	return 0;
}