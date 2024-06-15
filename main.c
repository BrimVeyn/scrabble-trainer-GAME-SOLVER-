/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:21 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/15 11:50:48 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/hashTableDefine.h"
#include "include/struct.h"
#include "lib/raylib/include/raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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

bool isLetter(char c) {
	return (c >= 'A' && c <= 'Z');
}



typedef struct Range {
	int s;
	int e;
} Range; 

typedef struct Placement {
	char c[15];
	int pos[15];
} Placement;

typedef struct Constraints {
	struct Range *range;
	struct Placement *pos;
	size_t size;
	size_t capacity;
} Constraints;

#define GRID_SIZE 15

void printConstraint(const Constraints c) {
	if (c.size == 0)
		return ;
	printf("-----Constraint----\n");
	if (c.size == 0)
		printf("X\n");
	for (size_t i = 0; i < c.size; i++) {
		printf("RANGE = %d-%d\n", c.range[i].s, c.range[i].e);
		for (size_t j = 0; c.pos[i].c[j]; j++) {
			printf("POS[%zu] = %c %d\n", i, c.pos[i].c[j], c.pos[i].pos[j]);
        }
		printf("~~~~~~~~~~\n");
	}
	printf("--------------\n");
}

Constraints constraintsInit( void ) {
	Constraints new;
	new.pos = calloc(10, sizeof(struct Placement));
	new.range = calloc(10, sizeof(struct Range));
	new.size = 0;
	new.capacity = 10;
	return new;
}

void fillPlacementAndRange(Constraints * curr, int *pos, char *c, int min, int max) {
	if (curr->size == curr->capacity)
		curr->pos = realloc(curr->pos, curr->capacity * 2);

	curr->range[curr->size].s = min;
	curr->range[curr->size].e = max;

	// for (int i = 0; c[i]; i++) {
	// 	printf("C = %c, POS = %d\n", c[i], pos[i]);
	// }
	for (int i = 0; c[i]; i++) {
		curr->pos[curr->size].pos[i] = pos[i];
		curr->pos[curr->size].c[i] = c[i];
	}

	curr->size++;
}

bool isLetterAboveOrBelow(GameData * gd, Point p) {
	if (p.y > 0 && isLetter(gd->grid.copy[p.y - 1][p.x]))
		return true;
	if (p.y < 14 && isLetter(gd->grid.copy[p.y + 1][p.x]))
		return true;
	return false;
}

#define ABS(x) x < 0 ? -x : x
#define P(x, y) (Point){x, y}
#define DELTA(size1, size2) ABS((int)(size1 - size2))

void rGetConstraints(GameData *game_data, Constraints *curr, Point p, char *c, int *pos, size_t max_len, int i, int it, size_t holes, int itp, int iterator) {
    int j = i;
    bool letterFound = false;

	if (!isLetter(game_data->grid.copy[p.y][p.x]) && iterator == 0) {
		int dist_letter = 0;
		for (; j <= 14 && !isLetter(game_data->grid.copy[p.y][j]) && ++holes <= max_len;) {
			dprintf(2, "coord = %d\n", j);
			if (isLetterAboveOrBelow(game_data, P(j, p.y))) {
				if (!letterFound)
					dist_letter = j - i + 1;
				letterFound = true;
				dprintf(2, "j = %d, i = %d\n", j, i);
			}
			j++;
		}
		int max_range = j - i;
		if (j <= 14 && isLetter(game_data->grid.copy[p.y][j]))
			max_range--;
		dist_letter = dist_letter < 2 ? 2 : dist_letter;
			
		holes = 0;
		if (letterFound && max_range >= 2) {
			if (max_range >= 2)
				fillPlacementAndRange(curr, pos, c, dist_letter, max_range);
		}
		rGetConstraints(game_data, curr, p, c, pos, max_len, p.x, 0, 0, 0, ++iterator);
		return;
	}

	if (isLetter(game_data->grid.copy[p.y][p.x]) && iterator == 0) {
		for (; j <= 14 && isLetter(game_data->grid.copy[p.y][j]) && it < 15; j++, it++) {
			pos[it] = j - p.x;
			c[it] = game_data->grid.copy[p.y][j];
		}
		// printf("ICI %d %d\n", p.y, j);
		if (j == 14 || (j + 1 <= 14 && !isLetter(game_data->grid.copy[p.y][j + 1])))
			goto inContact;
		for (; j <= 14 && !isLetter(game_data->grid.copy[p.y][j]) && ++holes < max_len; j++);
		for (; j <= 14 && isLetter(game_data->grid.copy[p.y][j]) && it < 15; j++, it++) {
			// dprintf(2, "lettre coord %d\n", j);
			pos[it] = j - p.x;
			c[it] = game_data->grid.copy[p.y][j];
		}
		if (!isLetter(game_data->grid.copy[p.y][j]))
			j--;
	} else {
		// dprintf(2, "coord arrive %d\n", j + 1);
		for (; j + 1 <= 14 && !isLetter(game_data->grid.copy[p.y][j + 1]) && ++holes < max_len; j++) {
			// dprintf(2, "coord pas lettre %d\n", j + 1);
		}
		for (; j + 1 <= 14 && isLetter(game_data->grid.copy[p.y][j + 1]); j++, it++) {
			// dprintf(2, "lettre coord 2 %d\n", j + 1);
			pos[it] = j - p.x + 1;
			c[it] = game_data->grid.copy[p.y][j + 1];
		}
	}

inContact: {
    int min = j;
    int max = j;


    for (; max + 1 <= 14 && !isLetter(game_data->grid.copy[p.y][max + 1]) && ++holes < max_len; max++) {
		// printf("PAS LETTRE BAS\n");
	}

    if (max + 1 <= 14 && isLetter(game_data->grid.copy[p.y][max + 1])) {
		// printf("LETTRE BAS\n");
		--holes;
        max -= 1;
    }

    min = min + 1 - p.x;
    max = max + 1 - p.x;

	// printf("min = %d, max = %d\n", min, max);

    if ((letterFound || c[0] != 0) && min <= max) {
		// printf("PUSH\n");
        fillPlacementAndRange(curr, pos, c, min, max);
    }

	// dprintf(2, "holes = %zu\n", holes);
    if (holes >= max_len || min == 14 || max == 14 || p.x + max - 1 == 14) {
		if (strlen(c) == (size_t) max) {
			curr->size--;
		}

        return;
	}

    rGetConstraints(game_data, curr, p, c, pos, max_len, p.x + max - 1, it, holes, itp, ++iterator);
    }
}

//Don't ask how it works
Constraints getConstraints(GameData * game_data, Point p, size_t max_len) {
	Constraints curr = constraintsInit();

	if (p.x > 0 && isLetter(game_data->grid.copy[p.y][p.x - 1]))
		return curr;

	char c[15] = {0};

	int pos[15] = {0};

	rGetConstraints(game_data, &curr, p, c, pos, max_len, p.x, 0, 0, 0, 0);


	return curr;
}

void freeConstraints(Constraints cell_c) {
	free(cell_c.pos);
	free(cell_c.range);
}

void findPermutations(char *chevalet, Vector *vect, size_t i, char *buf, size_t min, size_t max) {
	if (chevalet && i >= strlen(chevalet)) {
		size_t size_buf = strlen(buf);
		if (size_buf >= min && size_buf <= max) {
			char *tmp = sort_ascii(buf);
			if (IT_EQ(vector_find(vect, tmp), it_end(vect)))
				vector_push_back(vect, tmp);
			free(tmp);
        }
		return;
    }
	buf = str_append(buf, chevalet[i]);

	findPermutations(chevalet, vect, i + 1, buf, min, max);

	buf = str_remove(buf);

	findPermutations(chevalet, vect, i + 1, buf, min, max);
}

void applyPosConstraints(Vector * vect, Constraints cell_c, size_t i) {

	if (!cell_c.pos[i].c[0]) {
		printf("HERE\n");
		return;
	}
	for (Iterator it = it_begin(vect); IT_NEQ(it, it_end(vect)); it_pp(&it)) {

		char * word = vect->data[it.index];

		for (size_t j = 0; cell_c.pos[i].c[j]; j++) {

			if (!word[cell_c.pos[i].pos[j]] || word[cell_c.pos[i].pos[j]] != cell_c.pos[i].c[j]) {
				it_erase(&it);
				break;
            }
		}
	}

}

Vector purgeWrongPermutations(GameData * game_data, Vector * try) {

	Vector result = vector_construct(STR_TYPE);

	for (size_t i = 0; i < vector_get_size(try); i++) {
		Vector *tmp = asciiOrderedFind(game_data->asciiTable, try->data[i]);
		if (tmp)
			vector_push_vector(&result, tmp);
		else if (strlen(try->data[i]) == 1)
			vector_push_back(&result, try->data[i]);
	}
	vector_destruct(try);

	return result;
}

int j_points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2, 1, 1, 3, 8, 1, 1, 1, 1, 4, 10, 10, 10, 10};

int getWMultiplier(int c) {
	int m = 1;

	if (c == TWORD)
		m = 3;
	else if (c == DWORD)
		m = 2;
	return m;
}

int getLMultiplier(int c) {
	int m = 1;

	if (c == TLETTER)
		m = 3;
	else if (c == DLETTER)
		m = 2;
	return m;
}


bool isBanned(GameData * game_data, Point p) {
	if (game_data->grid.copy[p.y][p.x] == '!')
		return true;
	return false;
}

int computeScore(Point p, GameData * game_data, char current_letter) {
	int score = 0;
	char buffer[15];
	memset(buffer, 0x0, 15 * sizeof(char));

	int word_multiplier = 1;

	int start = p.y;

	for (; start - 1 >= 0 && isLetter(game_data->grid.copy[start - 1][p.x]); start--) ;

	int end = p.y;

	for (; end + 1 <= 14 && isLetter(game_data->grid.copy[end + 1][p.x]); end++) ;


	for (int it = start; it <= end;  it++) {
		if (it == p.y) {
			buffer[it - start] = current_letter;
			score += j_points[current_letter - 'A'] * getLMultiplier(game_data->grid.modifier[it][p.x]);
			word_multiplier = getWMultiplier(game_data->grid.modifier[it][p.x]);
        } else {
			buffer[it - start] = game_data->grid.copy[it][p.x];
			score += j_points[game_data->grid.copy[it][p.x] - 'A'];
		}
	}

	if (strlen(buffer) < 2)
		score = 0;
	else if (hashTableFind(game_data->hashTable, buffer) == -1)
		score = -1;

	dprintf(2, "word = |%s|, score = %d\n", buffer, score);

	return score * word_multiplier;
}


void tryMatch(Match *match, GameData * game_data) {
	// printf("------- %s---------\n", match->word);
	int tmp_score;
	for (int i = 0; match->start + i <= match->end; i++) {
		// printf("IT %d, match->start = %d, match->end = %d\n", i, match->start, match->end);
		if (game_data->grid.copy[match->save_coord][match->start + i] == 0) {
			tmp_score = computeScore((Point) {.y = match->save_coord, .x = match->start + i}, game_data, match->word[i]);
			// printf("it = %d, score = %d, lettre = %c\n", i, tmp_score, match->word[i]);
			if (tmp_score < 0) {
				match->score = -1;
				return;
			}
			
			match->score += tmp_score;
		}
	}
	// printf("word = %s match score = %d\n", match->word, match->score);
	// printf("--------END--------\n");

	match->score += findWordScore(*match, game_data->grid.modifier, game_data->grid.copy);
}

MatchVector *computeMatchs(GameData * game_data, Vector * vect, Point p, char *mandatory_letters) {

	MatchVector *try = matchVectorInit();
	
	for (size_t it = 0; it < vect->size; it++) {
		Match current = {
			.dir = game_data->iterator == 0 ? HORIZONTAL : VERTICAL,
			.validated = true,
			.start = p.x,
			.end = p.x + strlen(vect->data[it]) - 1,
			.save_coord = p.y,
		};
		memcpy(current.word, vect->data[it], strlen(vect->data[it]));

		tryMatch(&current, game_data);
		if (current.score == -1)
			continue;

		if (strlen(current.word) - strlen(mandatory_letters) == 7)
			current.score += SCRABBLE;

		/*printf("word = %s match score = %d\n", current.word, current.score);*/
		matchVectorPushBack(try, current);
	}
	return try;
}

MatchVector *computeCellWords(GameData * game_data, Point p, Constraints cell_c, char * chevalet) {
	
	MatchVector *match_result = matchVectorInit();

	for (size_t i = 0; i < cell_c.size; i++) {
		Vector try;

		try = vector_construct(STR_TYPE);

		char mandatory_letters[16] = {0};

		for (size_t j = 0; cell_c.pos[i].c[j] ; j++)
			mandatory_letters[j] = cell_c.pos[i].c[j];

		//Find all possible permutations given mandatory letters
		findPermutations(chevalet, &try, 0, mandatory_letters, cell_c.range[i].s, cell_c.range[i].e);

		/*vector_print(&try);*/

		//Eliminate impossible permutations and returns a new vector that has all possible words given possible permutations;
		try = purgeWrongPermutations(game_data, &try);

		//Elimate impossible words given pos constraints
		applyPosConstraints(&try, cell_c, i);

		MatchVector *match_try = computeMatchs(game_data, &try, p, mandatory_letters);

		// matchVectorPrint(match_try);

		matchVectorPushVector(match_result, match_try);
		vector_destruct(&try);
		matchVectorDestruct(match_try);

	}

	// matchVectorQuickSort(&match_result);
	// matchVectorPrint(&match_result);

	return match_result;
}

#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b

MatchVector *evaluateGrid(GameData * game_data, char *chevalet) {

	//Max len is equal to the number of letter in the chevalet
	size_t max_len = strlen(chevalet);
	//Init matchVector
	MatchVector *result = matchVectorInit();

	// Vector perm_chevalet = vector_construct(STR_TYPE);

	// char buffer[16] = {0};

	// // Find permutatiosn with given chevalet
	// findPermutations(chevalet, &perm_chevalet, 0, buffer, 2, strlen(chevalet));
	// // Clean all impossible permutations
	// Vector cleaned_perm_chevalet = purgeWrongPermutations(game_data, &perm_chevalet);


	//This function will be called twice with a matrix rotation 90 in between
	for (int Y = 0; Y < GRID_SIZE; Y++) {
		for (int X = 0; X < GRID_SIZE; X++) {

			//Create a point to the current cell;
			Point p = P(X, Y);
			printf("@@@ P = .y %d .x %d @@@\n", Y, X);


			//Compute all constraints on this grid
			Constraints cell_c = getConstraints(game_data, p, max_len);
			// printConstraint(cell_c);
			// freeConstraints(cell_c);
			// continue;
			if (cell_c.size == 0) {
				freeConstraints(cell_c);
				continue;
			}
			//Compute all possible words given cell_constraints
			MatchVector *cell_result = computeCellWords(game_data, p, cell_c, chevalet); 
			// printConstraint(cell_c);
			//Free constraints
			freeConstraints(cell_c);

			//Save words to the result vector
			matchVectorPushVector(result, cell_result);

			//Destroy temporary cell_vector
			matchVectorDestruct(cell_result);

		}
	}
	// vector_destruct(&cleaned_perm_chevalet);
	game_data->iterator = 1;
	return result;
}

void rotateMatrix(GameData * game_data) {
	int new[15][15];

	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			new[y][x] = game_data->grid.copy[x][y];
		}
	}

	for (int y = 0; y < GRID_SIZE; y++) {
		for (int x = 0; x < GRID_SIZE; x++) {
			game_data->grid.copy[x][y] = new[x][y];
		}
	}
}


int main(void) {

	GameData game_data = gameDataInit();

	/*InitWindow(screenWidth, screenHeight, "[Scrabble Trainer]");*/
	/**/
	/*SetTargetFPS(60);*/
	/**/
	/* RayLoop(&game_data);*/

	//Algo start//

	copyGrid(&game_data);
	// rotateMatrix(&game_data);

	char chevalet[] = "DECALES";

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

	MatchVector * result = evaluateGrid(&game_data, chevalet);

	// printf("------GRID COPY-----\n");
	// printGrid(game_data.grid.copy);
	// printf("--------------------\n");

	rotateMatrix(&game_data);

	MatchVector * result_transposed = evaluateGrid(&game_data, chevalet);

	matchVectorPushVector(result, result_transposed);
	matchVectorDestruct(result_transposed);

    clock_gettime(CLOCK_MONOTONIC, &end);

    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = seconds * 1e3 + nanoseconds * 1e-6; // milliseconds
    double elapsed_us = seconds * 1e6 + nanoseconds * 1e-3; // microseconds

	matchVectorQuickSort(result);
	matchVectorPrint(result, 0);
	matchVectorDestruct(result);

	printf("----CHEVALET------\n");
	printf("LETTERS = "YELLOW_F"%s\n"RESET_COLOR, chevalet);
	printf("------------------\n");

	printf("------GRID-----\n");
	printGrid(game_data.grid.grid);
	printf("---------------\n");

    printf("-----TIME TO COMPUTE----\n");
    printf("Elapsed time: "DARK_GREENF"%.3f ms\n"RESET_COLOR, elapsed);
    printf("Elapsed time: "DARK_GREENF"%.3f µs\n"RESET_COLOR, elapsed_us);
    printf("-------------------------\n");


	hashTableClear(game_data.hashTable);
	asciiOrderedClear(game_data.asciiTable);
	vector_destruct(&game_data.purse.purse_vect);
	vector_destruct(&game_data.score.prev_scores);
	free(game_data.hashTable);
	free(game_data.asciiTable);
}
