/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:21 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/31 17:28:37 by bvan-pae         ###   ########.fr       */
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

Grid gridInit(void) {
	struct Grid my_grid = {
		.grid =		{ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 'L', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 'O', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 'B', 'E', 'R', 'C', 'E', 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, },
		.tour_grid =  {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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

void drawGrid(Grid *grid) {
	int size = 40;
	int offsetX = 100;
	int offsetY = 70;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			int posX = (offsetX + (j * size));
			int posY = (offsetY + (i * size));
			int fontOffsetX = (size / 3), fontOffsetY = (size / 3.5);
			int fontSize = (size / 2);
			char *modifierText = getText(grid->modifier[i][j]);
			Color modifierColor = getColor(grid->modifier[i][j]);
			if (grid->modifier[i][j] != 0 && grid->grid[i][j] == 0)
            {
				DrawRectangle(posX, posY, size, size, modifierColor);
				DrawText(modifierText, posX + fontOffsetX, posY + fontOffsetY, fontSize, C_WHITE);
            }
			else
            {
				if (grid->grid[i][j] != 0)
					DrawRectangle(posX, posY, size, size, (Color) {255, 255, 255, 255});
				else
					DrawRectangle(posX, posY, size, size, (Color) {.r = 210, .g = 210, .b = 210, .a = 255});
				DrawText((char[]) { grid->grid[i][j], '\0'}, posX + fontOffsetX, posY + fontOffsetY, fontSize, BLACK);
				if (grid->modifier[i][j] != 0)
					DrawRectangleLinesEx((Rectangle) {.x = posX + 1, .y = posY + 1, .width = size - 2, .height = size - 2}, 2, modifierColor);
            }
			DrawRectangleLines(posX, posY, size, size, BLACK);
		}
	}
	int thickness = size / 5;
	Rectangle rec = {.x = offsetX -  thickness, .y = offsetY - thickness, .width = (size * 15) + (thickness * 2), .height = (size * 15) + (thickness * 2)};
	DrawRectangleLinesEx(rec, thickness, BROWN);
}

void putLetter(Grid *grid, int x, int y, char c) {
	grid->grid[x][y] = c;
}

void drawRuler(Ruler *ruler) {
	int size = 40;
	int fontSize = (size / 2);
	int fontOffsetX = (size / 3), fontOffsetY = (size / 3.5);
	for (int i = 0; i < 7; i++) {

		if (ruler->value[i] == 0) {
			DrawRectangleRec(ruler->rect[i], C_WHITE);
		}
		else {
			DrawRectangleRec(ruler->rect[i], WHITE);
			DrawRectangleLines(ruler->rect[i].x, ruler->rect[i].y, ruler->rect[i].width, ruler->rect[i].height, BLACK);
			DrawText((char[]) { ruler->value[i], '\0'}, ruler->rect[i].x + fontOffsetX, ruler->rect[i].y + fontOffsetY, fontSize, BLACK);
			DrawRectangleLinesEx((Rectangle) {.x = ruler->rect[i].x + 1, .y = ruler->rect[i].y + 1, .width = size - 2, .height = size - 2}, 2, getColor(ruler->modifier[i]));
		}

		DrawRectangleLinesEx(ruler->base_rect[i], 2, BLACK);
	}
}

Point findClosestEmpty(Point cell, GameData *game_data) {
	for (int i = 0; i < 7; i++) {
		if (game_data->grid.grid[cell.y + i][cell.x] == 0)
			return (Point) {cell.x, cell.y + i};
		if (game_data->grid.grid[cell.y][cell.x + i] == 0)
			return (Point) {cell.x + i, cell.y};
		if (game_data->grid.grid[cell.y - i][cell.x] == 0)
			return (Point) {cell.x, cell.y - i};
		if (game_data->grid.grid[cell.y][cell.x - i] == 0)
			return (Point) {cell.x - i, cell.y};
	}
	return (Point) {cell.x, cell.y};
}

void printGrid(int grid[15][15]) {
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15;  j++) {
			if (grid[i][j] != 0)
				printf("%c", grid[i][j]);
			else
				printf("%d", grid[i][j]);
		}
		printf("\n");
	}
}

Neighbor contactPoint(Point c, Grid *grid) {
	Neighbor new = {
		.dir = {0, 0, 0, 0},
		.valid = true,
		.points = { (Point) {.x = 0, .y = 0},
					(Point) {.x = 0, .y = 0},
					(Point) {.x = 0, .y = 0},
					(Point) {.x = 0, .y = 0}},
	};
	if (grid->grid[c.y + 1][c.x]  != 0) {
		new.points[SOUTH].y = 1;
		new.dir[SOUTH] = 1;
    }
	if (grid->grid[c.y][c.x + 1]  != 0) {
		new.points[EAST].x = 1;
		new.dir[EAST] = 1;
    }
	if (grid->grid[c.y - 1][c.x]  != 0) {
		new.points[NORTH].y = -1;
		new.dir[NORTH] = 1;
    }
	if (grid->grid[c.y][c.x - 1]  != 0) {
		new.points[WEST].x = -1;
		new.dir[WEST] = 1;
    }
	if (new.dir[NORTH] == 0 && new.dir[SOUTH] == 0 && new.dir[EAST] == 0 && new.dir[WEST] == 0)
		new.valid = false;
	return new;
}

char *getWord(Point p, Point mod, GameData *game_data, int dir) {
	(void) dir;
	int i = 0;
	char word[15];
	while (game_data->grid.grid[p.y][p.x] != 0 || game_data->grid.tour_grid[p.y][p.x] != 0) {
		if (game_data->grid.grid[p.y][p.x] != 0)
			word[i] = game_data->grid.grid[p.y][p.x];
		else
			word[i] = game_data->grid.tour_grid[p.y][p.x];
		p.x += mod.x;
		p.y += mod.y;
		i++;
	}
	word[i] = 0;
	if (dir == NORTH || dir == EAST) {
		char rev[15];
		int i = strlen(word) - 1;
		int j = 0;
		while (i >= 0)
			rev[j++] = word[i--];
		rev[j] = 0;
		return strdup(rev);
	}
	return strdup(word);
}

void checkTourWord(GameData *game_data) {
	for (int i = 0; i < 7; i++) {
		if (game_data->ruler.cell[i].x != -1) {
			Neighbor neighbors = contactPoint(game_data->ruler.cell[i], &game_data->grid);
			if (!neighbors.valid)
				continue;
			else
            {
				if (neighbors.dir[NORTH])
                {
					char *word = getWord(game_data->ruler.cell[i], neighbors.points[NORTH], game_data, NORTH);
					printf("FOUND WORD %s\n", word);
					printf("CORRECT = %d\n", hashTableFind(game_data->hashTable, word));
					free(word);
                }
            }
		}
	}
}

void RayLoop(GameData *game_data) {
	while (!WindowShouldClose()) {

        BeginDrawing();
		ClearBackground(GRAY);

		drawGrid(&game_data->grid);
		drawRuler(&game_data->ruler);

		if (IsKeyPressed(KEY_A)) {
			putLetter(&game_data->grid, GetRandomValue(0, 14), GetRandomValue(0, 14), (GetRandomValue(0, 25) + 'A'));
		}

		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			for (int i = 0; i < 7; i++) {
				Vector2 mouseP = {GetMouseX(), GetMouseY()};
				if(CheckCollisionPointRec(mouseP, game_data->ruler.rect[i]) && game_data->ruler.access == true)
				{
						game_data->ruler.access = false;
						game_data->ruler.dragging[i] = true;
				}
				if (game_data->ruler.dragging[i]) {
					game_data->ruler.rect[i].x = mouseP.x -  (B_SIZE / 2);
					game_data->ruler.rect[i].y = mouseP.y - (B_SIZE / 2);
					game_data->ruler.modifier[i] = 0;
					if (game_data->ruler.cell[i].x != -1)
						game_data->grid.tour_grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] = 0;
				}
			}
		} else {
			int offsetX = 100;
			int offsetY = 70;
			Vector2 mouseP = {GetMouseX(), GetMouseY()};
			for (int i = 0; i < 7; i++) {
				if (game_data->ruler.dragging[i] == true) {
					if (CheckCollisionPointRec(mouseP, game_data->grid.grid_rect)) {
						game_data->ruler.cell[i].x = (mouseP.x - offsetX) / B_SIZE;
						game_data->ruler.cell[i].y = (mouseP.y - offsetY) / B_SIZE;
						if (game_data->grid.grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] != 0)
							game_data->ruler.cell[i] = findClosestEmpty(game_data->ruler.cell[i], game_data);
						else if (game_data->grid.tour_grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] != 0)
							printf("SWAP\n");
						game_data->ruler.rect[i].x = game_data->ruler.cell[i].x * B_SIZE + offsetX;
						game_data->ruler.rect[i].y = game_data->ruler.cell[i].y * B_SIZE + offsetY;
						game_data->grid.tour_grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] = game_data->ruler.value[i];
						game_data->ruler.modifier[i] = game_data->grid.modifier[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x];
						printGrid(game_data->grid.tour_grid);
					}
					else {
						game_data->ruler.rect[i].x = game_data->ruler.base_rect[i].x;
						game_data->ruler.rect[i].y = game_data->ruler.base_rect[i].y;
					}
                }
				game_data->ruler.dragging[i] = false;
			}
			game_data->ruler.access = true;
		}

		checkTourWord(game_data);

		EndDrawing();
	}
    CloseWindow();
}


struct Ruler rulerInit(Purse *purse) {
	struct Ruler new;
	int offsetX = 100;
	int size = 40;

	SetRandomSeed(time(0));
	for (int i = 0; i < 7; i++) {
		int val = GetRandomValue(4, 8);
		if (purse->purse[val] != 0)
			purse->purse[val] -= 1;
		else {
			i--;
			continue;
        }
		new.value[i] = val + 'A';
		new.rect[i] = (Rectangle) {
			.x = offsetX + 4 * size + (i * size),
			.y = screenHeight - (size * 2),
			.height = size,
			.width =size,
		};
		new.base_rect[i] = new.rect[i];
		new.dragging[i] = false;
		new.modifier[i] = 0;
		new.cell[i] = (Point) {-1, -1};
	}
	new.access = true;
	return new;
}

struct Purse purseInit( void ) {
	struct Purse new = {
		.purse = { 9, 2, 2, 3, 15, 2, 2, 2, 8, 1, 1, 5, 3, 6, 6, 2, 1, 6, 6, 6, 6, 2, 1, 1, 1, 1 },
	};

	return new;
}

void fillData(GameData *game_data) {
	hashTableInit(game_data->hashTable);

	int fd = open(ORDERED_WORD_LIST_PATH, O_RDONLY);
	if (fd == -1)
		exit(EXIT_FAILURE);

	char *raw_file = getRawData(fd);

	fillHashTable(game_data->hashTable, raw_file);
	free(raw_file);
	close(fd);
}


int main(void) {

	struct GameData game_data = {
		.grid = gridInit(),
		.purse = purseInit(),
	};
	fillData(&game_data);
	int size = 40;
	Rectangle new_rect = {
		.x = 100 + 4 * size,
		.y = screenHeight - size - size,
		.width = size * 7,
		.height = size,
	};

	int offsetX = 100;
	int offsetY = 70;
	Rectangle grid_rect = {
		.x = offsetX,
		.y = offsetY,
		.height = B_SIZE * 15,
		.width = B_SIZE * 15,
	};

	game_data.grid.grid_rect = grid_rect;
	game_data.ruler_rect = new_rect;
	game_data.ruler = rulerInit(&game_data.purse);

	InitWindow(screenWidth, screenHeight, "[Scrabble Trainer]");

	SetTargetFPS(60);

	RayLoop(&game_data);

	hashTableClear(game_data.hashTable);
}
