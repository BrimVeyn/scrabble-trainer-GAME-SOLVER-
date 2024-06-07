#include "../../include/hashTableDefine.h"


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
