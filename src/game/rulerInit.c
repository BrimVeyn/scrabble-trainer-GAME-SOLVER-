
#include "../../include/hashTableDefine.h"

struct Ruler rulerInit(Purse *purse) {
	struct Ruler new;

	SetRandomSeed(time(0));
	for (int i = 0; i < 7; i++) {
		int index = GetRandomValue(0, purse->purse_vect.size - 1);
	 	int *val = vector_at(&purse->purse_vect, index);
		new.value[i] = *val;
		vector_erase_index(&purse->purse_vect, index);
		new.rect[i] = (Rectangle) {
			.x = DRAW_OFFSET_X + CELL_SIZE + (i * CELL_SIZE),
			.y = DRAW_OFFSET_Y + (16 * CELL_SIZE),
			.height = CELL_SIZE,
			.width =CELL_SIZE,
		};
		new.base_rect[i] = new.rect[i];
		new.dragging[i] = false;
		new.modifier[i] = 0;
		new.cell[i] = (Point) {-1, -1};
	}
	new.access = true;
	return new;
}
