/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:21 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/10 17:28:06 by bvan-pae         ###   ########.fr       */
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
	if (str && i >= strlen(str)) {
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

bool isCellHori(GameData * game_data, Point cell) {
	return (isLetter(game_data->grid.grid[cell.y + 1][cell.x])
	|| isLetter(game_data->grid.grid[cell.y - 1][cell.x]));
}

bool isCellVert(GameData * game_data, Point cell) {
	return (isLetter(game_data->grid.grid[cell.y][cell.x + 1])
	|| isLetter(game_data->grid.grid[cell.y][cell.x - 1]));
}

bool isCellVertHori(GameData * game_data, Point cell) {
	return (isCellVert(game_data, cell) && isCellHori(game_data, cell));
}


Match findBestWordMatch(GameData * game_data, char * word, Point p, Vector indexs, int scrabble) {

	Match best_match = {0};

	if (isCellVertHori(game_data, p))
		return best_match;
	else if (isCellVert(game_data, p)){		
		(void)p;
	}
	else if (isCellHori(game_data, p)) {
		(void)p;
	}

	for (size_t i = 0; i < indexs.size; i++) {
		//try placement
		Match try = {
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


MatchVector evaluateACell(GameData * game_data, Point point, char *chevalet) {
	MatchVector new = matchVectorInit();
	//temporary
	char list[15];
	char *letter_ptr = (char *) &game_data->grid.grid[point.y][point.x];

	bzero(list, 15);
	strcat(list, chevalet);
	strcat(list, letter_ptr);
	//end

	// printf("list = %s\n", list);
	Vector possible_words = findPossibleWords(list, game_data, letter_ptr);


	for (size_t i = 0; i < possible_words.size; i++) {
		Vector tmp = getPossibleIndexs(possible_words.data[i], *letter_ptr);
		int scrabble = 0;

		if (strlen(chevalet) + strlen(letter_ptr) == strlen(possible_words.data[i]))
			scrabble = 50;
		
		Match bwm = findBestWordMatch(game_data, possible_words.data[i], point, tmp, scrabble);
		if (bwm.dir == 0)
			continue;
		matchVectorPushBack(&new, bwm);
		vector_destruct(&tmp);
	}

	// printf("Count = %zu\n", possible_words.size);
	vector_destruct(&possible_words);
	return new;
}

char *rulerToStr(GameData * game_data) {
    char *ptr = calloc(8, sizeof(char));

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

void markFurther(GameData * game_data, int dir) {
	int i;
	int j;
	int *x = (dir == VERTICAL) ? &i : &j; 
	int *y = (dir == VERTICAL) ? &j : &i; 
	int count;
	for (i = 0; i < 15; i++) {
		count = 0;
		for (j = 0; j < 15; j++) {
			while (j < 15 && game_data->grid.copy[*y][*x] == '$')
            {
				j++;
				count++;
            }
			if (j < 15 && game_data->grid.copy[*y][*x] == '-' && count > 1)
            {
				game_data->grid.copy[(*y)++][*x] = ':';
				while (j < 15 && game_data->grid.copy[*y][*x] == 0 && count > 1)
					game_data->grid.copy[(*y)++][*x] = '|';
            }
			while (j < 15 && game_data->grid.copy[*y][*x] == 0 && count > 1)
				game_data->grid.copy[*y][(*x)++] = '-';
			count = 0;
		}
	}
}

MatchVector evaluateAdjacents(GameData * game_data, char *chevalet) {
	MatchVector vect = matchVectorInit();
	Vector word_list = findPossibleWords(chevalet, game_data, "");

	vector_print(&word_list);

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (game_data->grid.copy[i][j] == '$' || game_data->grid.copy[i][j] == '-' || game_data->grid.copy[i][j] == '|') {
				MatchVector tmp = computeAdjacent(game_data, &word_list, (Point) {.x = i, .y = j});
				matchVectorPushVector(&vect, &tmp);
				matchVectorDestruct(&tmp);
			}
		}
	}
	vector_destruct(&word_list);
	return vect;
}

bool isLetter(char c) {
	return (c >= 'A' && c <= 'Z');
}

//fill all adjacent cells with a marker, so we can evaluate them smartly
void fillAdjacentCells(GameData * game_data, char *chevalet) {
	MatchVector vect = matchVectorInit();
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
			if (isLetter(game_data->grid.copy[i][j]))
            {
				MatchVector tmp = evaluateACell(game_data, (Point) { .y = i, .x = j }, chevalet);
				matchVectorPushVector(&vect, &tmp);
				matchVectorDestruct(&tmp);
            }
            if (game_data->grid.copy[i][j] != 0 && game_data->grid.copy[i][j] != '$') {
                markAdjacentCells((Point) { .y = i, .x = j }, &game_data->grid);
            }
        }
    }


	// MatchVector tmp = evaluateAdjacents(game_data, chevalet);
	// matchVectorPushVector(&vect, &tmp);
	// matchVectorDestruct(&tmp);



	matchVectorQuickSort(&vect);
	matchVectorPrint(&vect);
	matchVectorDestruct(&vect);

	

	markFurther(game_data, HORIZONTAL);
	markFurther(game_data, VERTICAL);
	printGrid(game_data->grid.copy);
	// exit(1);
	// game_data->grid.copy[8][11] = '$';
	// game_data->grid.copy[8][12] = '$';
	// game_data->grid.copy[8][13] = '$';
	// game_data->grid.copy[8][14] = '$';
}


Match findWord(Point cell, int direction, GameData * game_data) {
	Match m = {
		.word = {0},
		.start = cell.y,
		.end = 0,
		.save_coord = cell.x,
		.dir = direction,
		.validated = false,
	};

	//Set x and y on direction
	int *x = (m.dir == VERTICAL) ? &m.save_coord : &m.start;
	int *y = (m.dir == VERTICAL) ? &m.start : &m.save_coord;

	printf("y = %d, x = %d\n", *y, *x);

	//Find start of the word
	while (isLetter(game_data->grid.copy[*y][*x])) {
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
	while (isLetter(game_data->grid.copy[*y][*x])) {
		m.word[i] += game_data->grid.copy[*y][*x]; 
		m.end++;
		i++;
	}
	m.end--;

	return m;
}

typedef struct MatchVar {
	int dx;
	int dy;
	int *x;
	int *y;
} MatchVar;

void saveGrid(int copy[15][15], char *save, Match match, MatchVar vars) {
	for (int i = 0; match.start + i <= match.end; i++) {
		save[i] = copy[*vars.y + i * vars.dy][*vars.x + i * vars.dx];
		copy[*vars.y + i * vars.dy][*vars.x + i * vars.dx] = match.word[i];
	}
}

void restoreGrid(int copy[15][15], char *save, Match match, MatchVar vars) {
	for (int i = 0; match.start + i <= match.end; i++) {
		copy[*vars.y + i * vars.dy][*vars.x + i * vars.dx] = save[i];
	}
}

int getScoreFind(int copy[15][15], Match match, MatchVar vars, GameData * game_data) {
	char save[strlen(match.word)];

	saveGrid(copy, save, match, vars);

	int score = 0;
	size_t passed = 0;
	Match word = {0};

	for (int i = 0; match.start + i <= match.end; i++) {
		int dir = (match.dir == VERTICAL) ? HORIZONTAL : VERTICAL;
		word = findWord((Point) {match.start + i, match.save_coord}, dir, game_data);
		//If its a letter, plot twist, its not a word
		printf("word.word = %s\n", word.word);
		if (strlen(word.word) < 2)
        {
			passed++;
			continue;
        }
		//If its not in the dictionnary, return error | impossible
		else if (hashTableFind(game_data->hashTable, word.word) == -1)
			return restoreGrid(copy, save, match, vars), -1;

		// printf("word = %s, coords = %d, coorde = %d\n", word.word, word.start, word.end);
		printf("MAISSSSSSSSSSSSSSSSSS\n");
		score += findWordScore(word, game_data->grid.modifier, game_data->grid.grid);
	}
	
	printGrid(copy);
	restoreGrid(copy, save, match, vars);
	if (passed == strlen(match.word))
		return -1;
	return score;
}

Match tryWord(GameData * game_data, char * word, Point cell, int dir) {
	Match match = {.score = 0};

	match.dir = (dir == HORIZONTAL) ? HORIZONTAL : VERTICAL,
	match.start = (dir == HORIZONTAL) ? cell.y - strlen(word) + 1 : cell.x -strlen(word) + 1,
	match.end = (dir == HORIZONTAL) ? cell.y : cell.x, 
	match.save_coord = (dir == HORIZONTAL) ? cell.x : cell.y,
	strcat(match.word, word);

	MatchVar vars = {
		.dx = (match.dir == VERTICAL) ? 0 : 1,
		.dy = (match.dir == VERTICAL) ? 1 : 0,
		.x = (match.dir == VERTICAL) ? &match.save_coord : &match.start,
		.y = (match.dir == VERTICAL) ?  &match.start : &match.save_coord,
	};

	if ((match.score = getScoreFind(game_data->grid.copy, match, vars, game_data)) == -1)
	{
		match.dir = -1;
		printf("failed\n");
		return match;
	}
	match.score += findWordScore(match, game_data->grid.modifier, game_data->grid.grid);
	printf("WORD = %s, SCORE = %d\n", match.word, match.score);
	return match;
}

bool isHori(GameData * game_data, Point cell) {
	if (game_data->grid.copy[cell.x][cell.y] == '-')
		return true;
	return (isLetter(game_data->grid.copy[cell.x + 1][cell.y])
	|| isLetter(game_data->grid.copy[cell.x - 1][cell.y]));
}

bool isVert(GameData * game_data, Point cell) {
	if (game_data->grid.copy[cell.x][cell.y] == '|')
		return true;
	return (isLetter(game_data->grid.copy[cell.x][cell.y + 1])
	|| isLetter(game_data->grid.copy[cell.x][cell.y - 1]));
}

bool isVertHori(GameData * game_data, Point cell) {
	return ((game_data->grid.copy[cell.x - 1][cell.y] != '$' && game_data->grid.copy[cell.x - 1][cell.y] != 0)
	|| (game_data->grid.copy[cell.x + 1][cell.y] != '$' && game_data->grid.copy[cell.x + 1][cell.y] != 0))
	&& ((game_data->grid.copy[cell.x][cell.y - 1] != '$' && game_data->grid.copy[cell.x][cell.y - 1] != 0)
	|| (game_data->grid.copy[cell.x][cell.y + 1] != '$' && game_data->grid.copy[cell.x][cell.y + 1] != 0));
}

MatchVector computeAdjacent(GameData * game_data, Vector * word_list, Point cell) {
	//try placing all words --> add to a vector
	MatchVector vect = matchVectorInit();
	Match try;
	for (Iterator it = it_begin(word_list); IT_NEQ(it, it_end(word_list)); it_pp(&it)) {
		if (isVertHori(game_data, cell)) {
			continue;
		}
		if (isHori(game_data, cell)) {
			try = tryWord(game_data, it.vector->data[it.index], cell, HORIZONTAL);
			if (try.score != -1)
				matchVectorPushBack(&vect, try);
		}
		else if (isVert(game_data, cell)){
			try = tryWord(game_data, it.vector->data[it.index], cell, VERTICAL);
			if (try.score != -1)
				matchVectorPushBack(&vect, try);
		}
	}
	// matchVectorPrint(&vect);
	return vect;
}


int main(void) {

	GameData game_data = gameDataInit();

	// InitWindow(screenWidth, screenHeight, "[Scrabble Trainer]");
	//
	// SetTargetFPS(60);
	//
	// Ra0000yLoop(&game_data);
	
    //Algo start//

    copyGrid(&game_data);
    // printGrid(game_data.grid.copy);

	// char *chevalet = rulerToStr(&game_data);
	char chevalet[] = "LSKSIIE";
	fillAdjacentCells(&game_data, chevalet);


	printf("LETTERS = %s\n", chevalet);
    printGrid(game_data.grid.copy);

	// free(chevalet);

	// InitWindow(screenWidth, screenHeight, "[Scrabble Trainer]");
	//
	// SetTargetFPS(60);
	//
	// RayLoop(&game_data);
	
	// for (int i = 0; obvious_cells[i].x; i++) {
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
