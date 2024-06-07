#include "../../include/hashTableDefine.h"


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
