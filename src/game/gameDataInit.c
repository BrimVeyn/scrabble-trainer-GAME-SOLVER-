
#include "../../include/hashTableDefine.h"
#include "struct.h"

GameData gameDataInit( void ) {
	struct GameData game_data = {
		.grid = gridInit(),
		.purse = purseInit(),
	};

	hashedWordInit(&game_data);
	asciiOrderedTableInit(&game_data);

	game_data.grid.grid_rect = (Rectangle) {
		.x = DRAW_OFFSET_X,
		.y = DRAW_OFFSET_Y,
		.height = CELL_SIZE * 15,
		.width = CELL_SIZE * 15,
	};

	game_data.ruler_rect = (Rectangle) {
		.x = 100 + 4 * CELL_SIZE,
		.y = screenHeight - CELL_SIZE - CELL_SIZE,
		.width = CELL_SIZE * 7,
		.height = CELL_SIZE,
	};

	game_data.ruler = rulerInit(&game_data.purse);
	game_data.isMainMenu = true;
	game_data.shouldBeClosed = false;
	game_data.tour.tourIsDone = false;
	game_data.tour.canValidate = false;

	game_data.tour.validate_rect = (Rectangle) {
		.x = DRAW_OFFSET_X + CELL_SIZE * 10,
		.y = DRAW_OFFSET_Y + CELL_SIZE * 15 + 5 + 0.25 * CELL_SIZE, 
 		.width = CELL_SIZE * 5,
		.height = CELL_SIZE * 2.5,
	};

	game_data.score = (Score) {
		.total_score = 0,
		.prev_scores = vector_construct(INT_TYPE),
	};

	return game_data;
}
