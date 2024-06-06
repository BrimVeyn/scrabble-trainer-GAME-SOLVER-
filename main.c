/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:21 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/06 10:30:30 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/hashTableDefine.h"
#include "include/struct.h"
#include "lib/raylib/include/raylib.h"
#include <stdlib.h>
#include <string.h>

TextVar getTextVar(char *text, int font_size) {
	return (TextVar) {
		.text_size = MeasureText(text, font_size),
		.font_size = font_size,
		.text = text,
	};
}

TextVar getText(int c, int font_size) {
	switch (c) {
		case TWORD:
			return getTextVar("MT", font_size);
		case DWORD:
			return getTextVar("MD", font_size);
		case DLETTER:
			return getTextVar("LD", font_size);
		case TLETTER:
			return getTextVar("LT", font_size);
		default:
			return (TextVar) {0};
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
	int posX, posY, fontSize;
	Color modifierColor;

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {

			posX = (DRAW_OFFSET_X + (j * CELL_SIZE));
			posY = (DRAW_OFFSET_Y + (i * CELL_SIZE));
			fontSize = (CELL_SIZE / 2);
			TextVar modifierText = getText(grid->modifier[i][j], fontSize);
			modifierColor = getColor(grid->modifier[i][j]);

			if (grid->modifier[i][j] != 0 && grid->grid[i][j] == 0) {
				DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, modifierColor);
				DrawText(modifierText.text, posX + (CELL_SIZE / 2) - modifierText.text_size / 2, posY + CELL_SIZE / 2 - fontSize / 2, fontSize, GRAY);
			}
			else {
				if (grid->grid[i][j] != 0)
					DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, WHITE);
				else
					DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, EMPTY_CELL_GRAY);
				TextVar cellText = getTextVar((char[]) { grid->grid[i][j], 0 }, fontSize);
				DrawText(cellText.text, posX + (CELL_SIZE / 2) - cellText.text_size / 2, posY + CELL_SIZE / 2 - fontSize / 2, fontSize, BLACK);
				if (grid->modifier[i][j] != 0)
					DrawRectangleLinesEx((Rectangle) {.x = posX + 1, .y = posY + 1, .width = CELL_SIZE - 2, .height = CELL_SIZE - 1}, 2, modifierColor);
			}
			DrawRectangleLines(posX, posY, CELL_SIZE, CELL_SIZE, BLACK);
		}
	}
	int thickness = CELL_SIZE / 16;
	Rectangle rec = {.x = DRAW_OFFSET_X -  thickness, .y = DRAW_OFFSET_Y - thickness, .width = (CELL_SIZE * 15) + (thickness * 2), .height = (CELL_SIZE * 15) + (thickness * 2)};
	DrawRectangleLinesEx(rec, thickness, BLACK);
}

void drawRulerRect(Ruler *ruler, int i) {
	int fontSize = (CELL_SIZE / 2);
	int fontOffsetX = (CELL_SIZE / 3), fontOffsetY = (CELL_SIZE / 3.5);
	if (ruler->value[i] == 0) {
		DrawRectangleRec(ruler->rect[i], C_WHITE);
	}
	else {
		DrawRectangleRec(ruler->rect[i], WHITE);
		DrawRectangleLines(ruler->rect[i].x, ruler->rect[i].y, ruler->rect[i].width, ruler->rect[i].height, BLACK);
		DrawText((char[]) { ruler->value[i], '\0'}, ruler->rect[i].x + fontOffsetX, ruler->rect[i].y + fontOffsetY, fontSize, BLACK);
		DrawRectangleLinesEx((Rectangle) {.x = ruler->rect[i].x + 1, .y = ruler->rect[i].y + 1, .width = CELL_SIZE - 2, .height = CELL_SIZE - 1}, 2, getColor(ruler->modifier[i]));
	}
	DrawRectangleLinesEx(ruler->base_rect[i], 2, BLACK);
}

void drawRuler(Ruler *ruler) {
	if (ruler->access == false) {
		int last_to_draw;
		for (int j = 0; j < 7; j++) {
			if (ruler->dragging[j] == true)
				last_to_draw = j;
		}
		for (int i = 0; i < 7; i++) {
			if (i != last_to_draw)
				drawRulerRect(ruler, i);
		}
		drawRulerRect(ruler, last_to_draw);
	} else {
		for (int i = 0; i < 7; i++) {
			drawRulerRect(ruler, i);
		}
	}
}

Point findClosestEmpty(Point cell, Grid grid) {
	for (int i = 0; i < 15; i++) {
		if (grid.grid[cell.y + i][cell.x] == 0 && grid.tour_grid[cell.y + i][cell.x] == 0)
			return (Point) {cell.x, cell.y + i};
		if (grid.grid[cell.y][cell.x + i] == 0 && grid.tour_grid[cell.y][cell.x + i] == 0)
			return (Point) {cell.x + i, cell.y};
		if (grid.grid[cell.y - i][cell.x] == 0 && grid.tour_grid[cell.y - i][cell.x] == 0)
			return (Point) {cell.x, cell.y - i};
		if (grid.grid[cell.y][cell.x - i] == 0 && grid.tour_grid[cell.y][cell.x - i] == 0)
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

Neighbor contactPoint(Point c, GameData *game_data) {
	Neighbor new = {
		.dir = {0, 0, 0, 0},
		.valid = false,
	};
	if (c.y + 1 < 15 && (game_data->grid.grid[c.y + 1][c.x]  != 0 || game_data->grid.tour_grid[c.y + 1][c.x])) {
		if (game_data->grid.grid[c.y + 1][c.x] != 0)
			new.valid = true;
		new.dir[SOUTH] = 1;
	}
	if (c.x + 1 < 15 && (game_data->grid.grid[c.y][c.x + 1]  != 0 || game_data->grid.tour_grid[c.y][c.x + 1])) {
		if (game_data->grid.grid[c.y][c.x + 1] != 0)
			new.valid = true;
		new.dir[EAST] = 1;
	}
	if (c.y - 1 > 0 && (game_data->grid.grid[c.y - 1][c.x]  != 0 || game_data->grid.tour_grid[c.y - 1][c.x])) {
		if (game_data->grid.grid[c.y - 1][c.x] != 0)
			new.valid = true;
		new.dir[NORTH] = 1;
	}
	if (c.x - 1 > 0 && (game_data->grid.grid[c.y][c.x - 1]  != 0 || game_data->grid.tour_grid[c.y][c.x - 1])) {
		if (game_data->grid.grid[c.y][c.x - 1] != 0)
			new.valid = true;
		new.dir[WEST] = 1;
	}
	return new;
}

Match getWordMatch(Point ruler_cell, GameData *game_data, int direction) {
	Match m = {
		.word = {0},
		.start = direction == VERTICAL ? ruler_cell.y : ruler_cell.x,
		.end = 0,
		.save_coord = direction == VERTICAL ? ruler_cell.x : ruler_cell.y,
		.dir = direction,
		.validated = false,
	};

	//Set x and y on direction
	int *x = m.dir == VERTICAL ? &m.save_coord : &m.start;
	int *y = m.dir == VERTICAL ? &m.start : &m.save_coord;

	//Find start of the word
	while (game_data->grid.grid[*y][*x] != 0 || game_data->grid.tour_grid[*y][*x] != 0) {
		m.start--;
	}
	//Iterate on more time to match the actual start of the word
	m.start++;

	//Reset x and y on direction
	m.end = m.start;
	x = m.dir == VERTICAL ? &m.save_coord : &m.end;
	y = m.dir == VERTICAL ? &m.end : &m.save_coord;

	//Fill and find the end coord of the word
	int i = 0;
	while (game_data->grid.grid[*y][*x] != 0 || game_data->grid.tour_grid[*y][*x] != 0) {
		m.word[i] += game_data->grid.grid[*y][*x];
		m.word[i] += game_data->grid.tour_grid[*y][*x]; 
		m.end++;
		i++;
	}
	m.end--;

	return m;
}

int countRulerOnGrid(Ruler ruler) {
	int x = 0;

	for (int i = 0; i < 7; i++) {
		if (ruler.cell[i].x != -1)
			x++;
	}
	return x;
}

int checkAlignment(GameData *game_data) {
	int x_ref = -1, y_ref = -1;
	int l_x = 16, l_y = 16;
	int h_x = -1, h_y = -1;
	bool ref = true;

	//If there are no rulers on the grid return false
	if (countRulerOnGrid(game_data->ruler) == 1)
		return 0;

	else {
		for (int i = 0; i < 7; i++) {
			//Skip ruler evaluation if its not on the grid
			if (game_data->ruler.cell[i].x == -1)
				continue;
			//Set coordinate reference to the first ruler on the grid.
			if (ref) {
				x_ref = game_data->ruler.cell[i].x;
				y_ref = game_data->ruler.cell[i].y;
				ref = false;
				continue;
			}

			//Update coordinate reference to match the farthest right and down
			if (game_data->ruler.cell[i].x > h_x) h_x = game_data->ruler.cell[i].x;
			if (game_data->ruler.cell[i].y > h_y) h_y = game_data->ruler.cell[i].y;
			if (game_data->ruler.cell[i].x < l_x) l_x = game_data->ruler.cell[i].x;
			if (game_data->ruler.cell[i].y < l_y) l_y = game_data->ruler.cell[i].y;

			//Reset ruler's ref if a ruler is not the same axis
			if (game_data->ruler.cell[i].x != x_ref)
				x_ref = -1;
			if (game_data->ruler.cell[i].y != y_ref)
				y_ref = -1;
		}

		//If rulers aren't aligned return false
		if (x_ref == -1 && y_ref == -1)
			return -1;

		//If they're aligned on x axis but there's en empty cell in between them return false
		if (x_ref != -1) {
			for (; l_y <= h_y; l_y++) {
				if (game_data->grid.grid[l_y][x_ref] == 0 && game_data->grid.tour_grid[l_y][x_ref] == 0)
					return -1;
			}
		}

		//If they're aligned on y axis but there's en empty cell in between them return false
		if (y_ref != -1) {
			for (; l_x <= h_x; l_x++) {
				if (game_data->grid.grid[y_ref][l_x] == 0 && game_data->grid.tour_grid[y_ref][l_x] == 0)
					return -1;
			}
		}
	}
	return 0;
}

bool findWordList(Match word_list[30], Match m) {
	//return true if a Match m is a duplicate of any in the word list
	for (int i = 0; word_list[i].dir != 0; i++) {
		if (!strcmp(word_list[i].word, m.word) && word_list[i].start == m.start && word_list[i].end == m.end && word_list[i].dir == m.dir)
			return true;
	}
	return false;
}

bool areWordsValid(TourManager tour) {
	//return true if all words in the list are found in the hash_table
	if (tour.word_list[0].dir == 0)
		return false;
	for (int i = 0; tour.word_list[i].dir != 0; i++) {
		if (tour.word_list[i].validated == false)
			return false;
	}
	return true;
}

int k_points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2, 1, 1, 3, 8, 1, 1, 1, 1, 4, 10, 10, 10, 10};

void updateTourHighest(TourManager * tour, int x, int y) {
	if (x > tour->highest_x || (x == tour->highest_x && y > tour->highest_y))
    {
		tour->highest_x = x;
		tour->highest_y = y;
    }
}

int calcWordScore(Match word, int g_modifier[15][15], int g_tour[15][15], TourManager *tour) {
	int modifier;
	int letter_score;
	int letter_multiplier; 
	int word_score = 0;
	int word_multiplier = 1;

	for (int i = word.start; i <= word.end; i++) {
		modifier = -1;
		letter_multiplier = 1;
		letter_score = k_points[word.word[i - word.start] - 'A'];

		//update x and y from direction
		int x = (word.dir == VERTICAL) ? word.save_coord : i;
		int y = (word.dir == VERTICAL) ? i : word.save_coord;

		updateTourHighest(tour, x, y);
		
		if (g_tour[y][x] != 0)
			modifier = g_modifier[y][x];

		//Update word and letter multiplier dependending on the modifier_grid value
		switch (modifier) {
			case DLETTER:
				letter_multiplier *= 2;
				break;
			case TLETTER:
				letter_multiplier *= 3;
				break;
			case DWORD:
				word_multiplier *= 2;
				break;
			case TWORD:
				word_multiplier *= 3;
				break;
		}
		word_score += (letter_score * letter_multiplier);
	}
	// printf("word [%s] score = %d\n", word.word, word_score * word_multiplier);
	return word_score * word_multiplier;
}

int calcWordListScore(GameData * game_data) {
	game_data->tour.highest_x = 0;
	game_data->tour.highest_y = 0;
	int total_score = 0;
	for (int i = 0; game_data->tour.word_list[i].dir != 0; i++) {
		total_score += calcWordScore(game_data->tour.word_list[i], game_data->grid.modifier, game_data->grid.tour_grid, &game_data->tour);
	}
	return total_score;
}

void drawTourWordOutline(GameData * game_data) {
	//Draw red or green outlines depending if the word is in the hashtable or not
	for (int i = 0; game_data->tour.word_list[i].dir != 0; i++) {
		// printf("outline drawn for word %s\n", game_data->tour.word_list[i].word);
		if (game_data->tour.word_list[i].validated) {
			DrawRectangleLinesEx(game_data->tour.word_list[i].match_rect, 5, (Color){ 0, 228, 48, 150 });
		} else {
			DrawRectangleLinesEx(game_data->tour.word_list[i].match_rect, 5, (Color){ 230, 41, 55, 150 });
		}
	}
}

void processWord(GameData *game_data, int *j, Match m) {
	if (findWordList(game_data->tour.word_list, m) == false) {
		game_data->tour.word_list[(*j)++] = m;
		if (hashTableFind(game_data->hashTable, m.word) != -1) {
			game_data->tour.word_list[(*j) - 1].validated = true;
		}
	}
}

Rectangle getMatchRect(Match m, int dir, Ruler ruler, int i) {
	int x = (dir == VERTICAL) ? ruler.rect[i].x : DRAW_OFFSET_X + (m.start * CELL_SIZE);
	int y = (dir == VERTICAL) ? DRAW_OFFSET_Y + (m.start * CELL_SIZE) : ruler.rect[i].y;
	int width = (dir == VERTICAL) ? CELL_SIZE + 1 : (m.end - m.start + 1) * CELL_SIZE;
	int height = (dir == VERTICAL) ? (m.end - m.start + 1) * CELL_SIZE : CELL_SIZE + 1;

	return (Rectangle) { x, y, width, height };
}

void checkTourWord(GameData *game_data) {
	int j = 0;
	game_data->tour.canValidate = false;
	bzero(game_data->tour.word_list, sizeof(struct Match) * 30);

	//Check Alignement and return if ruler's cell are not on the same axis
	if (checkAlignment(game_data) == -1)
		return ;

	for (int i = 0; i < 7; i++) {
		//If rulers not on the grid continue
		if (game_data->ruler.cell[i].x == -1)
			continue;

		//Inspect neighborhood
		Neighbor neighbors = contactPoint(game_data->ruler.cell[i], game_data);

		//If the evaluated ruler has no neighbors on the grid, continue;
		if (!neighbors.valid)
			continue;

		//Find VERTICAL word
		if (neighbors.dir[NORTH] || neighbors.dir[SOUTH]) {
			Match m = getWordMatch(game_data->ruler.cell[i], game_data, VERTICAL);
			m.match_rect = getMatchRect(m, VERTICAL, game_data->ruler, i);
			processWord(game_data, &j, m);
		}
		//Find HORIZONTAL word
		if (neighbors.dir[EAST] || neighbors.dir[WEST]) {
			Match m = getWordMatch(game_data->ruler.cell[i], game_data, HORIZONTAL);
			m.match_rect = getMatchRect(m, HORIZONTAL, game_data->ruler, i);
			processWord(game_data, &j, m);
		}
	}
	//Compute the addition of all words found
	game_data->tour.tour_score = calcWordListScore(game_data);
	// printf("tour score = %d\n", game_data->tour.tour_score);
	// printf("hy = %d hx = %d\n", game_data->tour.highest_y, game_data->tour.highest_x);
	
	//If all words are valid, you can validate the tour
	if (areWordsValid(game_data->tour) == true)
		game_data->tour.canValidate = true;
}

void dragRuler(GameData *game_data, Vector2 mouseP) {
	for (int i = 0; i < 7; i++) {
		if(CheckCollisionPointRec(mouseP, game_data->ruler.rect[i]) && game_data->ruler.access == true)
		{
			game_data->ruler.access = false;
			game_data->ruler.dragging[i] = true;
		}
		if (game_data->ruler.dragging[i]) {
			game_data->ruler.rect[i].x = mouseP.x -  (int)(CELL_SIZE / 2);
			game_data->ruler.rect[i].y = mouseP.y - (int)(CELL_SIZE / 2);
			game_data->ruler.modifier[i] = 0;
			if (game_data->ruler.cell[i].x != -1)
				game_data->grid.tour_grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] = 0;
			game_data->ruler.cell[i] = (Point) {-1, -1};
		}
	}
}

void dropRuler(GameData *game_data, Vector2 mouseP) {
	for (int i = 0; i < 7; i++) {
		if (game_data->ruler.dragging[i] == true) {
			if (CheckCollisionPointRec(mouseP, game_data->grid.grid_rect)) {
				game_data->ruler.cell[i].x = (mouseP.x - DRAW_OFFSET_X) / CELL_SIZE;
				game_data->ruler.cell[i].y = (mouseP.y - DRAW_OFFSET_Y) / CELL_SIZE;
				if (game_data->grid.grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] != 0)
					game_data->ruler.cell[i] = findClosestEmpty(game_data->ruler.cell[i], game_data->grid);
				else if (game_data->grid.tour_grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] != 0) {
					//Could be better is ruler cells were swappable
					game_data->ruler.cell[i] = findClosestEmpty(game_data->ruler.cell[i], game_data->grid);
				}
				game_data->ruler.rect[i].x = game_data->ruler.cell[i].x * CELL_SIZE + DRAW_OFFSET_X;
				game_data->ruler.rect[i].y = game_data->ruler.cell[i].y * CELL_SIZE + DRAW_OFFSET_Y;
				game_data->grid.tour_grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] = game_data->ruler.value[i];
				game_data->ruler.modifier[i] = game_data->grid.modifier[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x];
				// printGrid(game_data->grid.tour_grid);
			}
			else {
				game_data->grid.tour_grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] = 0;
				game_data->ruler.rect[i].x = game_data->ruler.base_rect[i].x;
				game_data->ruler.rect[i].y = game_data->ruler.base_rect[i].y;
			}
		}
		game_data->ruler.dragging[i] = false;
	}
	game_data->ruler.access = true;
}

void resetRuler(GameData *game_data, Vector2 mouseP) {
	for (int i = 0; i < 7; i++) {
		if (CheckCollisionPointRec(mouseP, game_data->ruler.rect[i])) {
			game_data->grid.tour_grid[game_data->ruler.cell[i].y][game_data->ruler.cell[i].x] = 0;
			game_data->ruler.rect[i].x = game_data->ruler.base_rect[i].x;
			game_data->ruler.rect[i].y = game_data->ruler.base_rect[i].y;
			game_data->ruler.cell[i] = (Point) {-1, -1};
		}
	}
}

void refillRuler(GameData *game_data) {
	Ruler *ptr = &game_data->ruler;
	for (int i = 0; i < 7; i++) {
		if (ptr->cell[i].x == -1)
			continue;
		int index = GetRandomValue(0, game_data->purse.purse_vect.size - 1);
	 	int *val = vector_at(&game_data->purse.purse_vect, index);
		ptr->value[i] = *val;
		vector_erase_index(&game_data->purse.purse_vect, index);
		ptr->rect[i] = (Rectangle) {
			.x = DRAW_OFFSET_X + CELL_SIZE + (i * CELL_SIZE),
			.y = DRAW_OFFSET_Y + (16 * CELL_SIZE),
			.height = CELL_SIZE,
			.width =CELL_SIZE,
		};
		ptr->base_rect[i] = ptr->rect[i];
		ptr->dragging[i] = false;
		ptr->modifier[i] = 0;
		ptr->cell[i] = (Point) {-1, -1};
	}
}

void matrixAdd(GameData *game_data) {
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			game_data->grid.grid[i][j] += game_data->grid.tour_grid[i][j];
			game_data->grid.tour_grid[i][j] = 0;
		}
	}
}

void scoreAdd(GameData * game_data) {
	game_data->score.total_score += game_data->tour.tour_score;
	vector_push_back(&game_data->score.prev_scores, INT_L(game_data->tour.tour_score));
}

void validateTour(GameData *game_data, Vector2 mouseP) {
	if (game_data->ruler.access == true && game_data->tour.canValidate == true && CheckCollisionPointRec(mouseP, game_data->tour.validate_rect)) {
		game_data->tour.canValidate = false;
		matrixAdd(game_data);
		refillRuler(game_data);
		scoreAdd(game_data);
	}
}

void eventListener(GameData *game_data) {

	Vector2 mouseP = {GetMouseX(), GetMouseY()};
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		dragRuler(game_data, mouseP); 
		validateTour(game_data, mouseP);
	} else {
		dropRuler(game_data, mouseP);
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		resetRuler(game_data, mouseP);
	}
}

void mainMenu(GameData *game_data) {
	ClearBackground(BEIGE);
	int buttonHeight = screenHeight / 5;
	int buttonWidth = screenWidth / 2;
	int buttonSpacing = screenHeight / 16;
	int mainOffsetY = (screenHeight - (buttonHeight * 3 + buttonSpacing * 2)) / 2;
	int mainOffsetX = screenWidth / 4;
	int textHeight = 100;
	int textWidthPlay = MeasureText("PLAY", textHeight);
	int textWidthQuit = MeasureText("QUIT", textHeight);

	Rectangle play_rect = {
		.x = mainOffsetX, 
		.y = mainOffsetY,
		.width = buttonWidth,
		.height = buttonHeight,
	};

	Rectangle option_rect = {
		.x = mainOffsetX, 
		.y = mainOffsetY + buttonHeight + buttonSpacing,
		.width = buttonWidth,
		.height = buttonHeight,
	};

	Rectangle quit_rect = {
		.x = mainOffsetX, 
		.y = mainOffsetY + (buttonHeight * 2) + buttonSpacing * 2,
		.width = buttonWidth,
		.height = buttonHeight,
	};

	Vector2 mouseP = { .x = GetMouseX(), .y = GetMouseY() };

	//PLAY button
	DrawRectangleRec(play_rect, BROWN);
	DrawText("PLAY", mainOffsetX + buttonWidth / 2 - textWidthPlay / 2, mainOffsetY + buttonHeight / 2 - textHeight / 2, textHeight, BLACK);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouseP, play_rect))
		game_data->isMainMenu = false;

	//Pending option button
	DrawRectangleRec(option_rect, BROWN);
	DrawRectangle(mainOffsetX, mainOffsetY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight, BROWN);
	
	//QUIT button
	DrawRectangleRec(quit_rect, BROWN);
	DrawText("QUIT", mainOffsetX + buttonWidth / 2 - textWidthQuit / 2, mainOffsetY + (buttonHeight * 2) + buttonSpacing * 2 + buttonHeight / 2 - textHeight / 2, textHeight, BLACK);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouseP, quit_rect)) {
		game_data->shouldBeClosed = true;
	}
}

void drawValidate(GameData *game_data) {
	TourManager *ptr = &game_data->tour;
	int text_size = MeasureText("JOUER", 40);
	DrawRectangleRec(ptr->validate_rect, EMPTY_CELL_GRAY);

	//outline green if all words are correct
	if (game_data->tour.canValidate == true)
		DrawRectangleLinesEx(ptr->validate_rect, 2, GREEN);
	else
		DrawRectangleLinesEx(ptr->validate_rect, 2, BLACK);
	DrawText("JOUER", ptr->validate_rect.x + ptr->validate_rect.width / 2 - text_size / 2, ptr->validate_rect.y + ptr->validate_rect.height / 2 - 20, 40, BLACK);
}

void drawPurse(GameData * game_data) {
	char counter[10] = { 'S', 'A', 'C', ':', ' ', 0 };
	char *ptr = ft_itoa(game_data->purse.purse_vect.size);
	strcat(counter, ptr);
	DrawText(counter, game_data->tour.validate_rect.x, game_data->tour.validate_rect.y + game_data->tour.validate_rect.height + CELL_SIZE / 5, 20, BLACK);
	free(ptr);
}

void drawTourScore(GameData * game_data) {
	if (game_data->tour.word_list[0].dir == 0)
		return;
	Rectangle score_rect = {
		.x = DRAW_OFFSET_X + CELL_SIZE * game_data->tour.highest_x + CELL_SIZE - (CELL_SIZE / 3),
		.y = DRAW_OFFSET_Y + CELL_SIZE * game_data->tour.highest_y + CELL_SIZE - (CELL_SIZE / 4),
		.width =  (int)(CELL_SIZE / 1.5),
		.height = (int)(CELL_SIZE / 2),
	};
	if (game_data->tour.canValidate)
		DrawRectangleRounded(score_rect, 0.5, 15, LIME);
	else
		DrawRectangleRounded(score_rect, 0.5, 15, RED);
	char *text = ft_itoa(game_data->tour.tour_score);
	int text_size = MeasureText(text, 20);
	DrawText(text, score_rect.x + score_rect.width / 2 - (text_size / 2), score_rect.y + score_rect.height / 2 - (10), 20, BLACK);
	free(text);
	DrawRectangleRoundedLinesEx(score_rect, 0.5, 15, 2, BLACK);
}

void drawTotalScore(GameData * game_data) {
	int width = (screenWidth - (game_data->grid.grid_rect.width + (CELL_SIZE / 8)));
	Rectangle s = {
		.x = game_data->grid.grid_rect.width + (0.1 * width),
		.y = DRAW_OFFSET_Y, 
		.width = width * 0.9,
		.height = CELL_SIZE,
	};

	char *text = ft_itoa(game_data->score.total_score);
	int text_size = MeasureText(text, 20);
	DrawRectangleRec(s, WHITE);
	DrawRectangleLinesEx(s, 2, BLACK);
	DrawText(text, s.x + s.width / 2 - (text_size / 2), s.y + s.height / 2 - (10), 20, BLACK);
	free(text);
}

void RayLoop(GameData *game_data) {
	while (!WindowShouldClose()) {
		if (game_data->shouldBeClosed == true)
			break;
		BeginDrawing();
		if (game_data->isMainMenu == true) { 
			mainMenu(game_data);
		} else {
			ClearBackground(BEIGE);

			drawGrid(&game_data->grid);
			drawValidate(game_data);
			drawRuler(&game_data->ruler);
			drawPurse(game_data);
			drawTourWordOutline(game_data);
			drawTourScore(game_data);
			drawTotalScore(game_data);

			eventListener(game_data);

			checkTourWord(game_data);

		}
		EndDrawing();
	}
	CloseWindow();
}

int main(void) {

	GameData game_data = gameDataInit();

	// InitWindow(screenWidth, screenHeight, "[Scrabble Trainer]");

	// SetTargetFPS(60);

	// RayLoop(&game_data);

	//free allocated memory
	hashTableClear(game_data.hashTable);
	asciiOrderedClear(game_data.asciiTable);
	vector_destruct(&game_data.purse.purse_vect);
	vector_destruct(&game_data.score.prev_scores);
	free(game_data.hashTable);
	free(game_data.asciiTable);
}
