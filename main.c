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
  int posX, posY, fontOffsetX, fontOffsetY, fontSize;
  char *modifierText;
  Color modifierColor;

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {

      posX = (DRAW_OFFSET_X + (j * CELL_SIZE));
      posY = (DRAW_OFFSET_Y + (i * CELL_SIZE));
      fontOffsetX = (CELL_SIZE / 3), fontOffsetY = (CELL_SIZE / 3.5);
      fontSize = (CELL_SIZE / 2);
      modifierText = getText(grid->modifier[i][j]);
      modifierColor = getColor(grid->modifier[i][j]);

      if (grid->modifier[i][j] != 0 && grid->grid[i][j] == 0) {
        DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, modifierColor);
        DrawText(modifierText, posX + fontOffsetX, posY + fontOffsetY, fontSize, C_WHITE);
      }
      else {
        if (grid->grid[i][j] != 0)
          DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, WHITE);
        else
          DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, EMPTY_CELL_GRAY);
        DrawText((char[]) { grid->grid[i][j], '\0'}, posX + fontOffsetX, posY + fontOffsetY, fontSize, BLACK);
        if (grid->modifier[i][j] != 0)
          DrawRectangleLinesEx((Rectangle) {.x = posX + 1, .y = posY + 1, .width = CELL_SIZE - 2, .height = CELL_SIZE - 1}, 2, modifierColor);
      }
      DrawRectangleLines(posX, posY, CELL_SIZE, CELL_SIZE, BLACK);
    }
  }
  int thickness = CELL_SIZE / 6;
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

Neighbor contactPoint(Point c, Grid *grid) {
	Neighbor new = {
		.dir = {0, 0, 0, 0},
		.valid = true,
	};
	if (grid->grid[c.y + 1][c.x]  != 0) {
		new.dir[SOUTH] = 1;
    }
	if (grid->grid[c.y][c.x + 1]  != 0) {
		new.dir[EAST] = 1;
    }
	if (grid->grid[c.y - 1][c.x]  != 0) {
		new.dir[NORTH] = 1;
    }
	if (grid->grid[c.y][c.x - 1]  != 0) {
		new.dir[WEST] = 1;
    }
	if (new.dir[NORTH] == 0 && new.dir[SOUTH] == 0 && new.dir[EAST] == 0 && new.dir[WEST] == 0)
		new.valid = false;
	return new;
}

Match getVertWord(Point ruler_cell, GameData *game_data) {
  Match m = {
    .word = {0},
    .start = ruler_cell.y,
    .end = ruler_cell.y,
  };

  while (game_data->grid.grid[m.start][ruler_cell.x] != 0 || game_data->grid.tour_grid[m.start][ruler_cell.x] != 0) {
    m.start--;
  }
  m.start++;
  while (game_data->grid.grid[m.end][ruler_cell.x] != 0 || game_data->grid.tour_grid[m.end][ruler_cell.x] != 0) {
    m.end++;
  }
  m.end--;
  if (m.start == m.end)
    return m;
  else {
    int i = 0, start = m.start;
    while (start <= m.end) {
      m.word[i] += game_data->grid.grid[start][ruler_cell.x];
      m.word[i] += game_data->grid.tour_grid[start][ruler_cell.x];
      i++, start++; 
    }
    m.word[i] = 0;
    return m;
  }
}

Match getHoriWord(Point ruler_cell, GameData *game_data) {
  Match m = {
    .word = {0},
    .start = ruler_cell.x,
    .end = ruler_cell.x,
  };

  while (game_data->grid.grid[ruler_cell.y][m.start] != 0 || game_data->grid.tour_grid[ruler_cell.y][m.start] != 0) {
    m.start--;
  }
  m.start++;
  while (game_data->grid.grid[ruler_cell.y][m.end] != 0 || game_data->grid.tour_grid[ruler_cell.y][m.end] != 0) {
    m.end++;
  }
  m.end--;
  if (m.start == m.end)
    return m;
  else {
    int i = 0, start = m.start;
    while (start <= m.end) {
      m.word[i] += game_data->grid.grid[ruler_cell.y][start];
      m.word[i] += game_data->grid.tour_grid[ruler_cell.y][start];
      i++, start++; 
    }
    m.word[i] = 0;
    return m;
  }
}

int countRulerOnGrid(Ruler ruler) {
  int x = 0;
  for (int i = 0; i < 7; i++) {
    if (ruler.cell[i].x != -1)
      x++;
  }
  return x;
}

void checkAlignment(GameData *game_data) {
  int x_ref = -1, y_ref = -1;
  bool ref = true;
  if (countRulerOnGrid(game_data->ruler) != 1) {
    for (int i = 0; i < 7; i++) {
      if (game_data->ruler.cell[i].x != -1 && ref) {
        x_ref = game_data->ruler.cell[i].x;
        y_ref = game_data->ruler.cell[i].y;
        ref = false;
        continue;
      }
      if (game_data->ruler.cell[i].x != -1 && game_data->ruler.cell[i].x != x_ref)
        x_ref = -1;
      if (game_data->ruler.cell[i].x != -1 && game_data->ruler.cell[i].y != y_ref)
        y_ref = -1;
    }
    // if (x_ref != -1)
    //   printf("found x\n");
    // if (y_ref != -1)
    //   printf("found y\n");
    // if (x_ref == -1 && y_ref == -1)
    //   printf("found nothing\n");
  }
}

void checkTourWord(GameData *game_data) {
  checkAlignment(game_data); //check if ruler's cell are on the same axis;
  for (int i = 0; i < 7; i++) {
    if (game_data->ruler.cell[i].x != -1) {
      Neighbor neighbors = contactPoint(game_data->ruler.cell[i], &game_data->grid);
      if (!neighbors.valid)
        continue;
      else
      {
        if (neighbors.dir[NORTH] || neighbors.dir[SOUTH])
        {
          Match m = getVertWord(game_data->ruler.cell[i], game_data);
          Rectangle match_rect = {
            .x = game_data->ruler.rect[i].x,
            .y = DRAW_OFFSET_Y + (m.start * CELL_SIZE),
            .width = CELL_SIZE + 1,
            .height = (m.end - m.start + 1) * CELL_SIZE,
          };
          if (hashTableFind(game_data->hashTable, m.word) != -1) {
            DrawRectangleLinesEx(match_rect, 5, GREEN);
          } else {
            DrawRectangleLinesEx(match_rect, 5, RED);
         }
        }
        if (neighbors.dir[EAST] || neighbors.dir[WEST])
        {
          Match m = getHoriWord(game_data->ruler.cell[i], game_data);
          Rectangle match_rect = {
            .x = DRAW_OFFSET_X + (m.start * CELL_SIZE),
            .y = game_data->ruler.rect[i].y,
            .width = (m.end - m.start + 1) * CELL_SIZE,
            .height = CELL_SIZE + 1,
          };
          if (hashTableFind(game_data->hashTable, m.word) != -1) {
            DrawRectangleLinesEx(match_rect, 5, GREEN);
          } else {
            DrawRectangleLinesEx(match_rect, 5, RED);
          }
        }
      }
    }
  }
}

void dragRuler(GameData *game_data, Vector2 mouseP) {
  for (int i = 0; i < 7; i++) {
    if(CheckCollisionPointRec(mouseP, game_data->ruler.rect[i]) && game_data->ruler.access == true)
    {
      game_data->ruler.access = false;
      game_data->ruler.dragging[i] = true;
    }
    if (game_data->ruler.dragging[i]) {
      game_data->ruler.rect[i].x = mouseP.x -  (CELL_SIZE / 2);
      game_data->ruler.rect[i].y = mouseP.y - (CELL_SIZE / 2);
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

void eventListener(GameData *game_data) {

  Vector2 mouseP = {GetMouseX(), GetMouseY()};
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    dragRuler(game_data, mouseP); 
  } else {
    dropRuler(game_data, mouseP);
  }
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    resetRuler(game_data, mouseP);
  }
}

void RayLoop(GameData *game_data) {
	while (!WindowShouldClose()) {

    BeginDrawing();
		ClearBackground(GRAY);

		drawGrid(&game_data->grid);
		drawRuler(&game_data->ruler);

    eventListener(game_data);

		checkTourWord(game_data);

		EndDrawing();
	}
    CloseWindow();
}

int main(void) {

  GameData game_data = gameDataInit();

	InitWindow(screenWidth, screenHeight, "[Scrabble Trainer]");

	SetTargetFPS(60);

	RayLoop(&game_data);

	hashTableClear(game_data.hashTable);
}
