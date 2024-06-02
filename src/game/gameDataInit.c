
#include "../../include/hashTableDefine.h"

GameData gameDataInit( void ) {
	struct GameData game_data = {
		.grid = gridInit(),
		.purse = purseInit(),
	};

	hashedWordInit(&game_data);
	Rectangle new_rect = {
		.x = 100 + 4 * CELL_SIZE,
		.y = screenHeight - CELL_SIZE - CELL_SIZE,
		.width = CELL_SIZE * 7,
		.height = CELL_SIZE,
	};

	Rectangle grid_rect = {
		.x = DRAW_OFFSET_X,
		.y = DRAW_OFFSET_Y,
		.height = CELL_SIZE * 15,
		.width = CELL_SIZE * 15,
	};

	game_data.grid.grid_rect = grid_rect;
	game_data.ruler_rect = new_rect;
	game_data.ruler = rulerInit(&game_data.purse);
  return game_data;
}
