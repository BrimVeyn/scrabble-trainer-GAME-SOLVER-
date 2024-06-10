#include "../../include/hashTableDefine.h"

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

//Calculate score for the bestWordFinding algortihm
int calcMatchWordScore(Match word, int g_modifier[15][15], int g_tour[15][15], int scrabble) {
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

		
		if (g_tour[y][x] == 0)
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
	return word_score * word_multiplier + scrabble;
}

int findWordScore(Match word, int g_modifier[15][15], int g_grid[15][15]) {
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

		if (g_grid[y][x] == 0)
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
