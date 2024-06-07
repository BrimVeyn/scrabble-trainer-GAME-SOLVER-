#include "../../include/hashTableDefine.h"


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
