#include "../../include/hashTableDefine.h"

//Draw ruler rectangle on the grid or on their base rec
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

//Simply goes through all ruler's cells and draw them
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
