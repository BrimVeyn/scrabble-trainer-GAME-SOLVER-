/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:21 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/13 19:49:27 by bvan-pae         ###   ########.fr       */
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

void fillPlacementAndRange(Constraints * curr, int *x, char *c, int min, int max) {
	if (curr->size == curr->capacity)
		curr->pos = realloc(curr->pos, curr->capacity * 2);

	curr->range[curr->size].s = min;
	curr->range[curr->size].e = max;
	for (int i = 0; c[i]; i++) {
		curr->pos[curr->size].pos[i] = x[i];
		curr->pos[curr->size].c[i] = c[i];
	}

	curr->size++;
}


//You should not try to read this
void rGetConstraints(GameData * game_data, Constraints * curr, Point p, char *c, int *pos, size_t max_len, int i, int it, size_t holes, int itp) {
	dprintf(2, "[I] = %d\n", i);

	size_t holes_save = holes;

	if (isLetter(game_data->grid.copy[p.y][p.x]) && itp == 0) {
		dprintf(2, "alos la %d\n", itp);
		for (int j = i + 1; !isLetter(game_data->grid.copy[p.y][j]); j++) {
			if (++holes == max_len) {
				if (it + 1 <= j - p.x)
					fillPlacementAndRange(curr, pos, c, it + 1, j - p.x);
				return;
			}
		}
    }

	holes = holes_save;

	while(i < GRID_SIZE && !isLetter(game_data->grid.copy[p.y][i])) {
		dprintf(2, "x = %d, holes = %zu\n", i, holes + 1);
		if (++holes == max_len) {
			if (i + 1 < GRID_SIZE && !isLetter(game_data->grid.copy[p.y][i])
				&& isLetter(game_data->grid.copy[p.y][i + 1])) {
				int k = i - p.x + 1;
				for (; k < GRID_SIZE && isLetter(game_data->grid.copy[p.y][k]); k++) {
					c[it] = game_data->grid.copy[p.y][k];
					pos[it++] = k - p.x;
				}
				dprintf(2, "POINTING to %c\n", game_data->grid.copy[p.y][k - 1]);
				dprintf(2, "@ K = %d, p.x = %d @\n", k, p.x);
				fillPlacementAndRange(curr, pos, c, k - p.x, k - p.x);
            }
			else if (curr->size != 0)
				curr->range[curr->size - 1].e = i + 1 - p.x;
			dprintf(2, "exited\n");
			return;
		}
		i++;
	}
	if (itp == 0 && it != 0 && isLetter(game_data->grid.copy[p.y][p.x])) {
		if (i == GRID_SIZE && i + 1 < i - p.x)
			fillPlacementAndRange(curr, pos, c, it + 1, i - p.x);
		else if (it + 1 <= i - 1 - p.x)
			fillPlacementAndRange(curr, pos, c, it + 1, i - 1- p.x);

	}

	if (i == GRID_SIZE)
		return;

	while(i < GRID_SIZE && isLetter(game_data->grid.copy[p.y][i])) {
		c[it] = game_data->grid.copy[p.y][i];
		pos[it++] = i - p.x;
		dprintf(2, "x = %d, letter = %c\n", i, game_data->grid.copy[p.y][i]);
		i++;
	}

	int min = i;
	int max = i;
	// size_t tmp_holes = holes;
	bool hole;

	while(max < GRID_SIZE && !isLetter(game_data->grid.copy[p.y][max])) {
		hole = true;
		max++;
	}
	if (hole == true && max != GRID_SIZE)
		max--;

	min -= p.x;
	max -= p.x;
	
	dprintf(2, "placing : min = %d, max = %d\n", min, max);
	if (min <= max)
		fillPlacementAndRange(curr, pos, c, min, max);
	rGetConstraints(game_data, curr, p, c, pos, max_len, i, it, holes, ++itp);
}


//Don't ask how it works
Constraints getConstraints(GameData * game_data, Point p, size_t max_len) {
	Constraints curr = constraintsInit();

	if (p.x > 0 && isLetter(game_data->grid.copy[p.y][p.x - 1]))
		return curr;

	char c[15];
	memset(c, 0x00, 15 * sizeof(char));

	int pos[15];
	memset(pos, 0x00, 15 * sizeof(int));

	int i = p.x;
	for (; i < GRID_SIZE && isLetter(game_data->grid.copy[p.y][i]); i++) {
		c[i - p.x] = game_data->grid.copy[p.y][i];
		pos[i - p.x] = i - p.x;
	}
	if (i != p.x)
    {
		dprintf(2, "found letter\n");
		rGetConstraints(game_data, &curr, p, c, pos, max_len, p.x + (i - p.x), i - p.x, 0, 0);
    }
	else
    {
		dprintf(2, "didnt find letter\n");
		rGetConstraints(game_data, &curr, p, c, pos, max_len, p.x, 0, 0, 0);
    }


	return curr;
}

void freeConstraints(Constraints cell_c) {
	free(cell_c.pos);
	free(cell_c.range);
}

void findPermutations2(char *chevalet, Vector *vect, size_t i, char *buf, size_t min, size_t max) {
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

	findPermutations2(chevalet, vect, i + 1, buf, min, max);

	buf = str_remove(buf);

	findPermutations2(chevalet, vect, i + 1, buf, min, max);
}

void applyPosConstraints(Vector * vect, Constraints cell_c, size_t i) {

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

#define ABS(x) x < 0 ? -x : x
#define P(x, y) (Point){x, y}
#define DELTA(size1, size2) ABS((int)(size1 - size2))

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

	if (start > 0 && start < GRID_SIZE) {
		start--;
		if (game_data->grid.copy[start][p.x] == 0) {
			start++;
		} else {
			while (start >= 0 && isLetter(game_data->grid.copy[start][p.x])) {
				start--;
			}
			start++;
		}
	}

	int end = p.y;

	if (end < GRID_SIZE && end > 0) {
		end++;
		if (game_data->grid.copy[end][p.x] == 0) {
			end--;
		} else {
			while (end <= GRID_SIZE && isLetter(game_data->grid.copy[end][p.x])) {
				end++;
			}
			end--;
		}
	}

	printf("start = %d, end = %d\n", start, end);

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

	printf("word = |%s|, score = %d\n", buffer, score);

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
			if (tmp_score == -1) {
				match->score = -1;
				return;
			}
			
			match->score += tmp_score;
		}
	}
	// printf("word = %s match score = %d\n", match->word, match->score);
	// printf("--------END--------\n");

	match->score += findWordScore(*match, game_data->grid.modifier, game_data->grid.grid);
}

MatchVector *computeMatchs(GameData * game_data, Vector * vect, Point p, char *mandatory_letters) {

	MatchVector *try = matchVectorInit();
	
	for (size_t it = 0; it < vect->size; it++) {
		Match current = {
			.dir = HORIZONTAL,
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
		Vector try = vector_construct(STR_TYPE);

		char mandatory_letters[16];
		memset(mandatory_letters, 0x0, 16 * sizeof(char));

		for (size_t j = 0; cell_c.pos[i].c[j] ; j++)
			mandatory_letters[j] = cell_c.pos[i].c[j];

		//Find all possible permutations given mandatory letters
		findPermutations2(chevalet, &try, 0, mandatory_letters, cell_c.range[i].s, cell_c.range[i].e);

		/*vector_print(&try);*/

		//Eliminate impossible permutations and returns a new vector that has all possible words given possible permutations;
		try = purgeWrongPermutations(game_data, &try);

		//Elimate impossible words given pos constraints
		applyPosConstraints(&try, cell_c, i);

		/*vector_print(&try);*/
		
		MatchVector *match_try = computeMatchs(game_data, &try, p, mandatory_letters);

    /*matchVectorPrint(match_try);*/

		matchVectorPushVector(match_result, match_try);

	}

	// matchVectorQuickSort(&match_result);
	// matchVectorPrint(&match_result);

	return match_result;
}


Range getHoleRange(GameData * game_data, Point p) {
	Range new = {
		.e = p.x,
		.s = p.x,
	};

	if (new.s > 0 && new.s < GRID_SIZE) {
		new.s--;
		if (new.s > 0 && isLetter(game_data->grid.copy[p.y][new.s])) {
			new.s++;
		} else {
			while (new.s > 0 && game_data->grid.copy[p.y][new.s] == 0) {
				new.s--;
			}
			new.s++;
		}
	}

	if (new.e < GRID_SIZE && new.e > 0) {
		new.e++;
		if (new.e < GRID_SIZE && isLetter(game_data->grid.copy[p.y][new.e])) {
			new.e--;
		} else {
			while (new.e < GRID_SIZE && game_data->grid.copy[p.y][new.e] == 0) {
				new.e++;
			}
			new.e--;
		}
	}

	new.s = new.s - p.x;
	new.e = new.e - p.x;

	return new;
}


bool isLetterAboveOrBelow(GameData * game_data, Point p) {
	if (p.y > 0 && p.y < 14) {
		if ((!isLetter(game_data->grid.copy[p.y - 1][p.x]))
			&& (!isLetter(game_data->grid.copy[p.y + 1][p.x])))
			return false;
	} else if (p.y == 0) {
		if (!isLetter(game_data->grid.copy[p.y + 1][p.x]))
			return false;
	} else if (p.y == 14) {
		if (!isLetter(game_data->grid.copy[p.y - 1][p.x]))
			return false;
	}
	return true;
}

size_t getRangeSize(Range range) {
	return (size_t)(range.e - range.s + 1);
}


MatchVector *tryCleanedWord(GameData * game_data, Point p, Range real_range, char *word) {
	MatchVector *try = matchVectorInit();

	size_t word_len = strlen(word);
	printf("word = xxx%s, range.s = %d, range.e = %d | P = y %d x %d\n", word, real_range.s, real_range.e, p.y, p.x);
	for (; getRangeSize(real_range) >= word_len; real_range.s++) {
		printf("trying p = %d\n", real_range.s + p.x);
		Match current = {
			.dir = HORIZONTAL,
			.validated = true,
			.start = real_range.s + p.x,
			.end = real_range.s + p.x + word_len - 1,
			.save_coord = p.y,
		};
		memcpy(current.word, word, word_len);

		tryMatch(&current, game_data);
		if (current.score == -1)
			continue;

		if (word_len == 7)
			current.score += SCRABBLE;

		printf("ADDING %s match score = %d\n", current.word, current.score);
		matchVectorPushBack(try, current);
	}
	return try;
}

#define MIN(a, b) a < b ? a : b
#define MAX(a, b) a > b ? a : b

Range getRealRange(Range hole_range, char *word) {
	Range new;

	new.s = MAX((int)(-(strlen(word) - 1)), hole_range.s);
	new.e = MIN((int)((strlen(word) - 1)), hole_range.e);

	return new;
}


MatchVector *computeVoidCells(GameData * game_data, Point p, Vector * cleaned_perm_chevalet) {

	if (isBanned(game_data, p))
		return (MatchVector *) NULL;

	if (!isLetterAboveOrBelow(game_data, p))
		return (MatchVector *) NULL;

	Range hole_range = getHoleRange(game_data, p);

	MatchVector *result = matchVectorInit();

	for (size_t it = 0; it < cleaned_perm_chevalet->size; it++) {
		if (strlen(cleaned_perm_chevalet->data[it]) > getRangeSize(hole_range)) {
			// printf("ntr %s\n", (char *)cleaned_perm_chevalet->data[it]);
			continue;
		}
		printf("WWW %s\n", (char *)cleaned_perm_chevalet->data[it]);

		Range real_range = getRealRange(hole_range, cleaned_perm_chevalet->data[it]);

		printf("WORD = %s, real_range = .s %d .e %d, hole_range = .s %d .e %d\n", (char *)cleaned_perm_chevalet->data[it], real_range.s, real_range.e, hole_range.s, hole_range.e);
		printf("POINT = .y %d .x %d\n", p.y, p.x);

		MatchVector *try = tryCleanedWord(game_data, p, real_range, cleaned_perm_chevalet->data[it]);

		matchVectorPushVector(result, try);
	}
	
	// matchVectorQuickSort(result);
	// matchVectorPrint(result);

	return result;
}

bool isAnyOfChevaletPossible(GameData * game_data, Point p, char *chevalet) {

  for (size_t i = 0; i < strlen(chevalet); i++) {
    if (computeScore(P(p.x, p.y), game_data, chevalet[i]) > 0)
      return true;
  }
  return false;
}

void markBannedCells(GameData * game_data, int Y, char * chevalet) {
	for (int x = 0; x < GRID_SIZE; x++) {
		if (isLetter(game_data->grid.copy[Y][x]) || isBanned(game_data, P(x, Y)))
			continue;

		if (!isLetterAboveOrBelow(game_data, P(x, Y)))
			continue;

		printf("@@@@@@@@@@@@@@@@@@ evaluating y = %d x = %d @@@@@@@@@@@@@@@\n", Y, x);
		if (!isAnyOfChevaletPossible(game_data, P(x, Y), chevalet))
		{
			printf("@@@@@@@@@@ BANNING y = %d x = %d @@@@@@@@@@@@@@\n", Y, x);
			game_data->grid.copy[Y][x] = '!';
		}
	}
}

MatchVector *evaluateGrid(GameData * game_data, char *chevalet) {

	//Max len is equal to the number of letter in the chevalet
	size_t max_len = strlen(chevalet);
	//Init matchVector
	MatchVector *result = matchVectorInit();

	Vector perm_chevalet = vector_construct(STR_TYPE);

	char buffer[16];
	memset(buffer, 0x0, 16 * sizeof(char));

	//Find permutatiosn with given chevalet
	findPermutations2(chevalet, &perm_chevalet, 0, buffer, 2, strlen(chevalet));
	//Clean all impossible permutations
	Vector cleaned_perm_chevalet = purgeWrongPermutations(game_data, &perm_chevalet);


	//This function will be called twice with a matrix rotation 90 in between
	for (int Y = 0; Y < GRID_SIZE; Y++) {
		//Ban all impossible vertical placement
		markBannedCells(game_data, Y, chevalet);
		// continue;
		for (int X = 0; X < GRID_SIZE; X++) {

			//Create a point to the current cell;
			Point p = P(X, Y);
			// printf("@@@ P = .y %d .x %d @@@\n", Y, X);


			//Compute all constraints on this grid
			Constraints cell_c = getConstraints(game_data, p, max_len);
			if (cell_c.size == 0) {
				freeConstraints(cell_c);
				(void) cleaned_perm_chevalet;

				MatchVector *cell_v_result = computeVoidCells(game_data, p, &cleaned_perm_chevalet);
				if (!cell_v_result)
					continue;

				matchVectorPushVector(result, cell_v_result);
				continue;
			}
			//Compute all possible words given cell_constraints
			MatchVector *cell_result = computeCellWords(game_data, p, cell_c, chevalet); 
			/*printConstraint(cell_c);*/

			//Save words to the result vector
			matchVectorPushVector(result, cell_result);

			//Destroy temporary cell_vector
			matchVectorDestruct(cell_result);

			//Free constraints
			freeConstraints(cell_c);
		}
	}
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
	rotateMatrix(&game_data);

	char chevalet[] = "FUCK";

	MatchVector * result = evaluateGrid(&game_data, chevalet);

	// rotateMatrix(&game_data);
	//
	// matchVectorPushVector(result, evaluateGrid(&game_data, chevalet));


	// printf("------GRID GRID-----\n");
	// printGrid(game_data.grid.grid);
	// printf("--------------------\n");
	printf("------GRID COPY-----\n");
	printGrid(game_data.grid.copy);
	printf("--------------------\n");

	matchVectorQuickSort(result);
	matchVectorPrint(result);
	matchVectorDestruct(result);

	printf("----CHEVALET------\n");
	printf("LETTERS = %s\n", chevalet);
	printf("------------------\n");

	printf("------GRID COPY-----\n");
	printGrid(game_data.grid.copy);
	printf("--------------------\n");


	hashTableClear(game_data.hashTable);
	asciiOrderedClear(game_data.asciiTable);
	vector_destruct(&game_data.purse.purse_vect);
	vector_destruct(&game_data.score.prev_scores);
	free(game_data.hashTable);
	free(game_data.asciiTable);
}
