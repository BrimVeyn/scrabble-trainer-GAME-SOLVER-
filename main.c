/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:21 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/30 17:30:14 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/hashTableDefine.h"
#include "include/struct.h"
#include "lib/raylib/include/raylib.h"
#include <string.h>

int s_points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2, 1, 1, 3, 8, 1, 1, 1, 1, 4, 10, 10, 10, 10};

char* getRawData(int fd) {
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t bytes_read;
    char temp_buffer[BUFFER_SIZE];

    while ((bytes_read = read(fd, temp_buffer, BUFFER_SIZE)) > 0) {
        char *new_buffer = realloc(buffer, buffer_size + bytes_read + 1);
        if (new_buffer == NULL) {
            free(buffer);
            return NULL;
        }
        buffer = new_buffer;
        for (int i = 0; i < bytes_read; i++) {
            buffer[buffer_size + i] = temp_buffer[i];
        }
        buffer_size += bytes_read;
    }

    if (buffer != NULL) {
        buffer[buffer_size] = '\0';
    }

    return buffer;
}

size_t getScore(char *word){
	size_t score = 0;
	for (int i = 0; word[i]; i++){
		score += s_points[word[i] - 'A'];
	}
	return score;
}

void fillHashTable(word **hashTable, char *raw_data) {
	int i = 0, j;
	char buffer[64];
	while (raw_data[i]) {
		j = 0;
		while (raw_data[i] && (raw_data[i] >= 'A' && raw_data[i] <= 'Z')) {
			buffer[j++] = raw_data[i++];
		}
		if (!raw_data[i]) return;
		buffer[j] = 0;
		struct word *ptr = wordNew(buffer, getScore(buffer));
		hashTableAdd(hashTable, ptr, getHash(ptr->word));
		i++;
	}
}

grid gridInit(void) {
	struct grid my_grid = {
		.grid =		{ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 'P', 0, 'P', 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 'I', 0, 'O', 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 'A', 'L', 'O', 'R', 'S', 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 'E', 0, 'C', 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, },
		.modifier = { {TWORD, 0, 0, DLETTER, 0, 0, 0, TWORD, 0, 0, 0, DLETTER, 0, 0, TWORD},
			       	  {0, DWORD, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, DWORD, 0},
					  {0, 0, DWORD, 0, 0, 0, DLETTER, 0, DLETTER, 0, 0, 0, DWORD, 0, 0},
					  {DLETTER, 0, 0, DWORD, 0, 0, 0, DLETTER, 0, 0, 0, DWORD, 0, 0, DLETTER},
					  {0, 0, 0, 0, DWORD, 0, 0, 0, 0, 0, DWORD, 0, 0, 0, 0},
					  {0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0},
					  {0, 0, DLETTER, 0, 0, 0, DLETTER, 0, DLETTER, 0, 0, 0, DLETTER, 0, 0},
					  {TWORD, 0, 0, DLETTER, 0, 0, 0, DWORD, 0, 0, 0, DLETTER, 0, 0, TWORD},
					  {0, 0, DLETTER, 0, 0, 0, DLETTER, 0, DLETTER, 0, 0, 0, DLETTER, 0, 0},
					  {0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0},
					  {0, 0, 0, 0, DWORD, 0, 0, 0, 0, 0, DWORD, 0, 0, 0, 0},
					  {DLETTER, 0, 0, DWORD, 0, 0, 0, DLETTER, 0, 0, 0, DWORD, 0, 0, DLETTER},
					  {0, 0, DWORD, 0, 0, 0, DLETTER, 0, DLETTER, 0, 0, 0, DWORD, 0, 0},
			       	  {0, DWORD, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, DWORD, 0},
					  {TWORD, 0, 0, DLETTER, 0, 0, 0, TWORD, 0, 0, 0, DLETTER, 0, 0, TWORD} },
	};
	return my_grid;
}

char *getText(int c) {
	switch (c) {
		case TWORD:
			return "MT";
		case DWORD:
			return "MD";
		case DLETTER:
			return "LD";
		case TLETTER:
			return "LT";
		default:
			return "";
	}
}

Color getColor(int c) {
	switch (c) {
		case TWORD:
			return RED;
		case DWORD:
			return BEIGE;
		case DLETTER:
			return SKYBLUE;
		case TLETTER:
			return DARKBLUE;
		default:
			return WHITE;
	}
}

void gridDisplay(grid *grid) {
	int size = 40;
	int offsetX = 125;
	int offsetY = 125;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			int posX = (offsetX + (j * size));
			int posY = (offsetY + (i * size));
			int fontOffsetX = (size / 6), fontOffsetY = (size / 3.5);
			int fontSize = (size / 2);
			char *modifierText = getText(grid->modifier[i][j]);
			Color modifierColor = getColor(grid->modifier[i][j]);
			if (grid->modifier[i][j] != 0 && grid->grid[i][j] == 0)
            {
				DrawRectangle(posX, posY, size, size, modifierColor);
				DrawText(modifierText, posX + fontOffsetX, posY + fontOffsetY, fontSize, (Color){ 200, 200, 200, 255 });
            }
			else
            {
				if (grid->grid[i][j] != 0)
					DrawRectangle(posX, posY, size, size, (Color) {255, 255, 255, 255});
				else
					DrawRectangle(posX, posY, size, size, (Color) {.r = 210, .g = 210, .b = 210, .a = 255});
				DrawText((char[]) { grid->grid[i][j], '\0'}, posX + fontOffsetX + 10, posY + fontOffsetY, fontSize, BLACK);
				if (grid->modifier[i][j] != 0)
					DrawRectangleLinesEx((Rectangle) {.x = posX + 1, .y = posY + 1, .width = size - 2, .height = size - 2}, 2, modifierColor);
            }
			DrawRectangleLines(posX, posY, size, size, BLACK);
		}
	}
	int thickness = size / 4;
	Rectangle rec = {.x = offsetX -  thickness, .y = offsetY - thickness, .width = (size * 15) + (thickness * 2), .height = (size * 15) + (thickness * 2)};
	DrawRectangleLinesEx(rec, thickness, BROWN);
}

void putLetter(grid *grid, int x, int y, char c) {
	grid->grid[x][y] = c;
}

void RayLoop(grid *grid) {
	while (!WindowShouldClose()) {

        BeginDrawing();
		gridDisplay(grid);
		if (IsKeyPressed(KEY_A)) {
			putLetter(grid, GetRandomValue(0, 15), GetRandomValue(0, 15), (GetRandomValue(0, 25) + 'A'));
		}
		EndDrawing();
	}
    CloseWindow();
}


int main(void) {


	struct grid s_grid = gridInit();

	InitWindow(screenWidth, screenHeight, "[Scrabble Trainer]");

    SetTargetFPS(60);

	RayLoop(&s_grid);
	//
	//
	//
	//
	//
	// word * hashTable[TABLE_SIZE];
	// hashTableInit(hashTable);
	//
	// int fd = open("data/Data.txt", O_RDONLY);
	// if (fd == -1)
	// 	return 1;
	//
	// char *raw_file = getRawData(fd);
	// 
	// char ***sorted_file = create_buffers(raw_file);
	// 
	// char mask[15] = "P****D";
	// list test = mask_word(sorted_file, mask);
	// (void)test;
	// // for (int i = 0; test.content[i]; i++){
	// // 	printf("%s\n", test.content[i]);
	// // }
	//
	// for (int i = 0; sorted_file[10][i]; i++){
	// 	printf("%s\n", sorted_file[10][i]);
	// }
	// (void) sorted_file;
	// free_3x_char(sorted_file);
	// 
	// // printf("%s", raw_file);
	//
	// fillHashTable(hashTable, raw_file);
	//
	// free(raw_file);
	// close(fd);
	//
	// printf("%d\n", hashTableFind(hashTable, "MANGERENT"));
	//
	// // hashTablePrint(hashTable);
	// hashTableClear(hashTable);
}
