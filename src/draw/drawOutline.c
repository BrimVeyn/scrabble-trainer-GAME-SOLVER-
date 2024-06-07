#include "../../include/hashTableDefine.h"


void drawTourWordOutline(GameData * game_data) {
	//Draw red or green outlines depending if the word is in the hashtable or not
	for (int i = 0; game_data->tour.word_list[i].dir != 0; i++) {
		// printf("outline drawn for word %s\n", game_data->tour.word_list[i].word);
		if (game_data->tour.word_list[i].validated) {
			DrawRectangleLinesEx(game_data->tour.word_list[i].match_rect, 5, (Color){ 0, 228, 48, 150 });
		} else {
			DrawRectangleLinesEx(game_data->tour.word_list[i].match_rect, 5, (Color){ 230, 41, 55, 150 });
		}
	}
}
