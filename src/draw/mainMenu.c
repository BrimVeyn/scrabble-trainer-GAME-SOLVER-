#include "../../include/hashTableDefine.h"

extern int screenHeight;
extern int screenWidth;
extern int windowWidth;
extern int windowHeight;

void mainMenu(GameData *game_data) {
	ClearBackground(BEIGE);
	int buttonHeight = windowHeight / 5;
	int buttonWidth = windowWidth / 2;
	int buttonSpacing = windowHeight / 16;
	int mainOffsetY = (windowHeight - (buttonHeight * 3 + buttonSpacing * 2)) / 2;
	int mainOffsetX = windowWidth / 4;
	int textHeight = 100;
	int textWidthPlay = MeasureText("PLAY", textHeight);
	int textWidthQuit = MeasureText("QUIT", textHeight);

	Rectangle play_rect = {
		.x = mainOffsetX, 
		.y = mainOffsetY,
		.width = buttonWidth,
		.height = buttonHeight,
	};

	Rectangle option_rect = {
		.x = mainOffsetX, 
		.y = mainOffsetY + buttonHeight + buttonSpacing,
		.width = buttonWidth,
		.height = buttonHeight,
	};

	Rectangle quit_rect = {
		.x = mainOffsetX, 
		.y = mainOffsetY + (buttonHeight * 2) + buttonSpacing * 2,
		.width = buttonWidth,
		.height = buttonHeight,
	};

	Vector2 mouseP = { .x = GetMouseX(), .y = GetMouseY() };

	//PLAY button
	DrawRectangleRec(play_rect, BROWN);
	DrawText("PLAY", mainOffsetX + buttonWidth / 2 - textWidthPlay / 2, mainOffsetY + buttonHeight / 2 - textHeight / 2, textHeight, BLACK);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouseP, play_rect))
		game_data->isMainMenu = false;

	//Pending option button
	DrawRectangleRec(option_rect, BROWN);
	DrawRectangle(mainOffsetX, mainOffsetY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight, BROWN);
	
	//QUIT button
	DrawRectangleRec(quit_rect, BROWN);
	DrawText("QUIT", mainOffsetX + buttonWidth / 2 - textWidthQuit / 2, mainOffsetY + (buttonHeight * 2) + buttonSpacing * 2 + buttonHeight / 2 - textHeight / 2, textHeight, BLACK);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouseP, quit_rect)) {
		game_data->shouldBeClosed = true;
	}
}
