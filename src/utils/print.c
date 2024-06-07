#include "../../include/hashTableDefine.h"


void findMatchPrint(FindMatch match) {
	printf("Best word = { .word = %s\n .start = %d,\n .end = %d,\n .saved_coord = %d,\n .score = %d }\n", match.word, match.start, match.end, match.save_coord, match.score);
}

void printGrid(int grid[15][15]) {
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15;  j++) {
			if (grid[i][j] != 0)
				printf("\033[0;91m%c\033[0;39m", grid[i][j]);
			else
				printf("%d", grid[i][j]);
		}
		printf("\n");
	}
}

void  printPoints(Point *points) {
	for (int i = 0; points[i].x; i++) {
		printf("P[%d] = { .y = %d, .x = %d }\n", i, points[i].y, points[i].x);
	}
}
