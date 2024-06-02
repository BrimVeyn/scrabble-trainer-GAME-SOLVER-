
#include "../../include/hashTableDefine.h"

struct Ruler rulerInit(Purse *purse) {
	struct Ruler new;

	SetRandomSeed(time(0));
	for (int i = 0; i < 7; i++) {
		int val = GetRandomValue(4, 8);
		if (purse->purse[val] != 0)
			purse->purse[val] -= 1;
		else {
			i--;
			continue;
        }
		new.value[i] = val + 'A';
		new.rect[i] = (Rectangle) {
			.x = DRAW_OFFSET_X + 4 * CELL_SIZE + (i * CELL_SIZE),
			.y = screenHeight - (CELL_SIZE * 2),
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
