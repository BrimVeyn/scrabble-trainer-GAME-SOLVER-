#include "../../include/hashTableDefine.h"

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
