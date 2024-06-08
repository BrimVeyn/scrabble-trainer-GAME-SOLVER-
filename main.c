/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:21 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/06 17:12:25 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/hashTableDefine.h"
#include "include/struct.h"
#include "lib/raylib/include/raylib.h"
#include <stdlib.h>
#include <string.h>

void eventListener(GameData *game_data) {
	Vector2 mouseP = {GetMouseX(), GetMouseY()};

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		dragRuler(game_data, mouseP); 
		validateTour(game_data, mouseP);
	} else {
		dropRuler(game_data, mouseP);
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		resetRuler(game_data, mouseP);
	}
}

void drawManager(GameData * game_data) {
    drawGrid(&game_data->grid);
    drawValidate(game_data);
    drawRuler(&game_data->ruler);
    drawPurse(game_data);
    drawTourWordOutline(game_data);
    drawTourScore(game_data);
    drawTotalScore(game_data);
}

void RayLoop(GameData *game_data) {
	while (!WindowShouldClose()) {
		if (game_data->shouldBeClosed == true)
			break;
		BeginDrawing();
		if (game_data->isMainMenu == true) { 
			mainMenu(game_data);
		} else {
			ClearBackground(BEIGE);

            drawManager(game_data);

			eventListener(game_data);

			checkTourWord(game_data);

		}
		EndDrawing();
	}
	CloseWindow();
}

void findPermutations(char *str, Vector *vect, size_t i, char *buf) {
	if (i >= strlen(str)) {
		if (strlen(buf) > 1) {
			char *tmp = sort_ascii(buf);
			if (IT_EQ(vector_find(vect, tmp), it_end(vect)))
				vector_push_back(vect, tmp);
			free(tmp);
        }
		return;
    }
	buf = str_append(buf, str[i]);

	findPermutations(str, vect, i + 1, buf);

	buf = str_remove(buf);

	findPermutations(str, vect, i + 1, buf);
}

//Find if constraint is part of the permutatiomn
bool isSubset(const char *str1, const char *str2) {
	if (!str1)
		return true;

    int len1 = strlen(str1);
    int len2 = strlen(str2);
    
    for (int i = 0; i < len1; i++) {
        bool found = false;
        for (int j = 0; j < len2; j++) {
            if (str1[i] == str2[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            return false;
        }
    }
    
    return true;
}

Vector findPossibleWords(char *letters, GameData * game_data, char *mandatory) {

	// int count = 0;
	char buffer[10] = {0};

	//Init vector of possible permutations
	Vector vect = vector_construct(STR_TYPE);
	findPermutations(letters, &vect, 0, buffer);

	//Init vector of all possible words with these permutations
	Vector result = vector_construct(STR_TYPE);

	vector_quick_sort(&vect);

	//Fill result vector if the permutation is a valid one
	for (size_t i = 0; i < vector_get_size(&vect); i++) {
		if (!isSubset(mandatory, vect.data[i]))
			continue;
		Vector *tmp = asciiOrderedFind(game_data->asciiTable, vect.data[i]);
		if (tmp)
			vector_push_vector(&result, tmp);
	}
	vector_destruct(&vect);

	//Not necessary but for debug readeability
	// vector_quick_sort(&result);
	// count += vector_print(&vect);
	// printf("Possible word count = %d\n", count);
	
	return result;
}


FindMatch findBestWordMatch(GameData * game_data, char * word, Point p, Vector indexs, int scrabble) {

	FindMatch best_match = {0};

	for (size_t i = 0; i < indexs.size; i++) {
		//try placement
		FindMatch try = {
			.word = {0},
			.dir = VERTICAL,
			.start = p.y - *((int *) indexs.data[i]),
			.end = p.y - *((int *) indexs.data[i]) + strlen(word) - 1,
			.save_coord = p.x,
		};
		strcat(try.word, word);
		try.score = calcMatchWordScore(try, game_data->grid.modifier, game_data->grid.grid, scrabble);

		if (try.score > best_match.score)
			best_match = try;
		
	}
	return best_match;
}

//Find all repetition of the target letter, meaning how many different placement we can evaluate
Vector getPossibleIndexs(char *str, char c) {
	Vector new = vector_construct(INT_TYPE);

	for (int i = 0; str[i]; i++) {
		if (str[i] == c) {
			vector_push_back(&new, INT_L(i));
		}
	}
	return new;
}


void evaluateACell(GameData * game_data, Point point, char *chevalet) {
	//temporary
	char list[15];
	char *letter_ptr = (char *) &game_data->grid.grid[point.y][point.x];

	bzero(list, 15);
	strcat(list, chevalet);
	strcat(list, letter_ptr);
	//end

	printf("list = %s\n", list);
	Vector possible_words = findPossibleWords(list, game_data, letter_ptr);


	//debug
	// vector_print(&possible_words);

	FindMatch overall_best_word = {.score = 0};

	for (size_t i = 0; i < possible_words.size; i++) {
		Vector tmp = getPossibleIndexs(possible_words.data[i], *letter_ptr);
		int scrabble = 0;

		if (strlen(chevalet) + strlen(letter_ptr) == strlen(possible_words.data[i]))
			scrabble = 50;
		
		FindMatch bwm = findBestWordMatch(game_data, possible_words.data[i], point, tmp, scrabble);
		if (bwm.score > overall_best_word.score)
        {
			overall_best_word = (FindMatch) { 
				.start = bwm.start,
				.end = bwm.end,
				.dir = bwm.dir,
				.score = bwm.score,
				.save_coord = bwm.save_coord };
			strcat(overall_best_word.word, bwm.word);
        }
		vector_destruct(&tmp);
	}


	printf("Count = %zu\n", possible_words.size);
	findMatchPrint(overall_best_word);
	vector_destruct(&possible_words);
}

char *rulerToStr(GameData * game_data) {
    char *ptr = calloc(7, sizeof(char));

    for (int i = 0; i < 7; i++) {
        ptr[i] = game_data->ruler.value[i];
    }
    return ptr;
}


//simply makes a copy of grid.grid to grid.copy to work on it
void copyGrid(GameData * game_data) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            game_data->grid.copy[i][j] = game_data->grid.grid[i][j];
        }
    }
}

void markAdjacentCells(Point c, Grid * grid) {
	if (c.y + 1 < 15 && grid->copy[c.y + 1][c.x] == 0)
        grid->copy[c.y + 1][c.x] = '$';
	if (c.x + 1 < 15 && grid->copy[c.y][c.x + 1] == 0)
        grid->copy[c.y][c.x + 1] = '$';
	if (c.y - 1 > 0 && grid->copy[c.y - 1][c.x] == 0)
        grid->copy[c.y - 1][c.x] = '$';
	if (c.x - 1 > 0 && grid->copy[c.y][c.x - 1] == 0)
        grid->copy[c.y][c.x - 1] = '$';
}

//fill all adjacent cells with a marker, so we can evaluate them smartly
void fillAdjacentCells(GameData * game_data) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (game_data->grid.copy[i][j] != 0 && game_data->grid.copy[i][j] != '$') {
                markAdjacentCells((Point) { .y = i, .x = j }, &game_data->grid);
            }
        }
    }
}

int main(void) {

	GameData game_data = gameDataInit();

	// InitWindow(screenWidth, screenHeight, "[Scrabble Trainer]");
	//
	// SetTargetFPS(60);
	//
	// RayLoop(&game_data);
	
    //Algo start//

    copyGrid(&game_data);
    fillAdjacentCells(&game_data);
    printGrid(game_data.grid.copy);
	char *chevalet = rulerToStr(&game_data);
    (void) chevalet;
	
	// for (int i = 0; obvious_cells[i].x; i++) {
	// 	evaluateACell(&game_data, obvious_cells[i], chevalet);
	// }	

    //Algo end//

	// for (int i = 0; i < 1; i++) {
	// 	evaluateACell(&game_data, obvious_cells[i], chevalet);
	// }	

    // printGrid(game_data.grid.grid);
	// vector_print(&vect);

	hashTableClear(game_data.hashTable);
	asciiOrderedClear(game_data.asciiTable);
	vector_destruct(&game_data.purse.purse_vect);
	vector_destruct(&game_data.score.prev_scores);
	free(game_data.hashTable);
	free(game_data.asciiTable);
}
