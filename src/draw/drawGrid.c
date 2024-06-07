#include "../../include/hashTableDefine.h"


void drawGrid(Grid *grid) {
	int posX, posY, fontSize;
	Color modifierColor;

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {

			posX = (DRAW_OFFSET_X + (j * CELL_SIZE));
			posY = (DRAW_OFFSET_Y + (i * CELL_SIZE));
			fontSize = (CELL_SIZE / 2);
			TextVar modifierText = getText(grid->modifier[i][j], fontSize);
			modifierColor = getColor(grid->modifier[i][j]);

			if (grid->modifier[i][j] != 0 && grid->grid[i][j] == 0) {
				DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, modifierColor);
				DrawText(modifierText.text, posX + (CELL_SIZE / 2) - modifierText.text_size / 2, posY + CELL_SIZE / 2 - fontSize / 2, fontSize, GRAY);
			}
			else {
				if (grid->grid[i][j] != 0)
					DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, WHITE);
				else
					DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, EMPTY_CELL_GRAY);
				TextVar cellText = getTextVar((char[]) { grid->grid[i][j], 0 }, fontSize);
				DrawText(cellText.text, posX + (CELL_SIZE / 2) - cellText.text_size / 2, posY + CELL_SIZE / 2 - fontSize / 2, fontSize, BLACK);
				if (grid->modifier[i][j] != 0)
					DrawRectangleLinesEx((Rectangle) {.x = posX + 1, .y = posY + 1, .width = CELL_SIZE - 2, .height = CELL_SIZE - 1}, 2, modifierColor);
			}
			DrawRectangleLines(posX, posY, CELL_SIZE, CELL_SIZE, BLACK);
		}
	}
	int thickness = CELL_SIZE / 16;
	Rectangle rec = {.x = DRAW_OFFSET_X -  thickness, .y = DRAW_OFFSET_Y - thickness, .width = (CELL_SIZE * 15) + (thickness * 2), .height = (CELL_SIZE * 15) + (thickness * 2)};
	DrawRectangleLinesEx(rec, thickness, BLACK);
}
