/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:21 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/12 17:21:51 by bvan-pae         ###   ########.fr       */
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

Vector findPossibleWords(char *letters, GameData * game_data, char *mandatory) {

	// int count = 0;
	char buffer[15] = {0};

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

bool isOutOfBound(Match try) {
	if (try.start > 14 || try.start < 0)
		return true;
	if (try.end > 14 || try.start < 0)
		return true;
	return false;
}

Match findBestWordMatch(GameData * game_data, char * word, Point p, Vector indexs, int scrabble) {

	Match best_match = {0};
	int dir = 0;

	if (isCellVertHori(game_data, p))
		return best_match;
	else if (isCellVert(game_data, p)){		
		dir = VERTICAL;
	}
	else if (isCellHori(game_data, p)) {
		dir = HORIZONTAL;
	}

	for (size_t i = 0; i < indexs.size; i++) {
		//try placement
		Match try = {
			.word = {0},
			.dir = dir,
			.start = (dir == VERTICAL) ? p.y - *((int *) indexs.data[i]) : p.x - *((int *) indexs.data[i]),
			.end = (dir == VERTICAL) ? p.y - *((int *) indexs.data[i]) + strlen(word) - 1 : p.x - *((int *) indexs.data[i]) + strlen(word) - 1,
			.save_coord = (dir == VERTICAL) ? p.x : p.y,
		};
		if (isOutOfBound(try))
			continue;
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

void markFurther(GameData * game_data) {

	int y, x;

	for (y = 0; y < 15; y++) {
		for (x = 0; x < 15; x++) {
			if (x < 15 &&  y < 15 && !isHori(game_data, (Point) {y, x}) && game_data->grid.copy[y][x] == '$') {
				printf("Point | = .y %d .x %d\n", y, x);
				int tmp = y;
				while (tmp < 15 && (game_data->grid.copy[tmp][x] == '$' || game_data->grid.copy[tmp][x] == '|'))
					tmp++;
				if (tmp < 15 && game_data->grid.copy[tmp][x] == '-')
					game_data->grid.copy[tmp++][x] = ':';
				while (tmp < 15 && game_data->grid.copy[tmp][x] == 0)
					game_data->grid.copy[tmp++][x] = '|';
			}
			if (x < 15 &&  y < 15 && !isVert(game_data, (Point) {y, x}) && (game_data->grid.copy[y][x] == '$' || game_data->grid.copy[y][x] == '|')) {
				printf("Point - = .y %d .x %d\n", y, x);
				x++;
				if (game_data->grid.copy[y][x] == '|')
					game_data->grid.copy[y][x++] = ':';
				while (x < 15 && (game_data->grid.copy[y][x] == 0 || game_data->grid.copy[y][x] == ':'))
					game_data->grid.copy[y][x++] = '-';
			}
			if (isVertHori(game_data, (Point){y, x}) && !isLetter(game_data->grid.copy[y][x]))
				game_data->grid.copy[y][x] = ':';
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
				// MatchVector tmp = evaluateACell(game_data, (Point) { .y = i, .x = j }, chevalet);
				// matchVectorPushVector(&vect, &tmp);
				// matchVectorDestruct(&tmp);
            }
            if (game_data->grid.copy[i][j] != 0 && game_data->grid.copy[i][j] != '$') {
                markAdjacentCells((Point) { .y = i, .x = j }, &game_data->grid);
            }
        }
    }

	MatchVector tmp = evaluateAdjacents(game_data, chevalet);
	matchVectorPushVector(&vect, &tmp);
	matchVectorDestruct(&tmp);

	matchVectorQuickSort(&vect);
	matchVectorPrint(&vect);
	matchVectorDestruct(&vect);

	// markFurther(game_data);
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

		score += findWordScore(word, game_data->grid.modifier, game_data->grid.grid);
		printf("word = %s, coords = %d, coorde = %d\n", word.word, word.start, word.end);
		printf("score = %d\n", score);
	}
	
	printGrid(copy);
	restoreGrid(copy, save, match, vars);
	if (passed == strlen(match.word))
		return -1;
	return score;
}

Match tryWord(GameData * game_data, char * word, Point cell, int dir, int offset) {
	Match match = {.score = 0};

	match.dir = (dir == HORIZONTAL) ? HORIZONTAL : VERTICAL,
	match.start = ((dir == HORIZONTAL) ? cell.y - strlen(word) + 1 : cell.x -strlen(word) + 1) + offset,
	match.end = ((dir == HORIZONTAL) ? cell.y : cell.x) + offset, 
	match.save_coord = (dir == HORIZONTAL) ? cell.x : cell.y,
	strcat(match.word, word);

	if (!strcmp(match.word, "RETAISE"))
	{
		printf("ICI");
		printf("match.start = %d, match.end = %d, match.save_coord %d\n", match.start, match.end, match.save_coord);
	}

	if (isOutOfBound(match)) {
		match.dir = -1;
		return match;
	}

	MatchVar vars = {
		.dx = (match.dir == VERTICAL) ? 0 : 1,
		.dy = (match.dir == VERTICAL) ? 1 : 0,
		.x = (match.dir == VERTICAL) ? &match.save_coord : &match.start,
		.y = (match.dir == VERTICAL) ?  &match.start : &match.save_coord,
	};

	if ((match.score = getScoreFind(game_data->grid.copy, match, vars, game_data)) == -1) {
		match.dir = -1;
		return match;
	}

	match.score += findWordScore(match, game_data->grid.modifier, game_data->grid.grid);
	if (strlen(match.word) == 7)
		match.score += SCRABBLE;
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
	return (isVert(game_data, cell) && isHori(game_data, cell));
}

void addTry(GameData * game_data, char *word, Point cell, int dir, MatchVector * vect) {
	for (int offset = 0; word[offset]; offset++) {
		Match try = tryWord(game_data, word, cell, dir, offset);
		if (try.score == -1 || try.dir == -1)
			continue;
		matchVectorPushBack(vect, try);
	}
}

MatchVector computeAdjacent(GameData * game_data, Vector * word_list, Point cell) {
	//try placing all words --> add to a vector
	MatchVector vect = matchVectorInit();
	for (Iterator it = it_begin(word_list); IT_NEQ(it, it_end(word_list)); it_pp(&it)) {
		if (isVertHori(game_data, cell)) {
			// addTry(game_data, it.vector->data[it.index], cell, HORIZONTAL, &vect);
			// addTry(game_data, it.vector->data[it.index], cell, HORIZONTAL, &vect);
		}
		if (isHori(game_data, cell)) {
			addTry(game_data, it.vector->data[it.index], cell, HORIZONTAL, &vect);
		}
		else if (isVert(game_data, cell)){
			addTry(game_data, it.vector->data[it.index], cell, VERTICAL, &vect);
		}
	}
	// matchVectorPrint(&vect);
	return vect;
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
	memset(c, 0x00, 15);

	int pos[15];
	memset(pos, 0x00, 15);

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

int computeScore(Point p, GameData * game_data, char current_letter) {
	int score = 0;
	char buffer[15];
	memset(buffer, 0x0, 15);

	int start = (game_data->grid.copy[p.y - 1][p.x] == 0) ? p.y : p.y + 1;
	int it;
	int word_multiplier = 1;
	// printf("p {.y = %d, .x = %d}\n", p.y, p.x);

	for (; start > 0 && game_data->grid.copy[start][p.x] != 0; start--) { }

	for (it = start; it < GRID_SIZE && (game_data->grid.copy[it][p.x] != 0 || it == p.y); it++) {
		if (it == p.y) {
			buffer[it] = current_letter;
			score += j_points[current_letter - 'A'] * getLMultiplier(game_data->grid.modifier[it][p.x]);
			word_multiplier = getWMultiplier(game_data->grid.modifier[it][p.x]);
			continue;
        }
		buffer[it] = game_data->grid.copy[it][p.x];
		score += j_points[game_data->grid.copy[it][p.x] - 'A'];
	}

	// printf("word = %s\n", buffer);

	if (strlen(buffer) < 2)
		score = 0;
	else if (hashTableFind(game_data->hashTable, buffer) == -1)
		score = -1;

	return score * word_multiplier;
}


void tryMatch(Match *match, GameData * game_data) {
	// printf("------- %s---------\n", match->word);
	int tmp_score;
	for (int i = 0; match->start + i < match->end; i++) {
		if (game_data->grid.copy[match->save_coord][match->start + i] == 0) {
			tmp_score = computeScore((Point) {.y = match->save_coord, .x = match->start + i}, game_data, match->word[i]);
			if (tmp_score == -1) {
				match->score = -1;
				return;
			}
			
			match->score += tmp_score;
		}
	}

	printf("word = %s match score = %d\n", match->word, match->score);
	match->score += findWordScore(*match, game_data->grid.modifier, game_data->grid.grid);
}

MatchVector computeMatchs(GameData * game_data, Vector * vect, Point p, char *mandatory_letters) {

	MatchVector try = matchVectorInit();
	
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

		printf("word = %s match score = %d\n", current.word, current.score);
		matchVectorPushBack(&try, current);
	}
	return try;
}


MatchVector computeCellWords(GameData * game_data, Point p, Constraints cell_c, char * chevalet) {
	
	MatchVector match_result = matchVectorInit();

	for (size_t i = 0; i < cell_c.size; i++) {
		Vector try = vector_construct(STR_TYPE);

		char mandatory_letters[15];
		memset(mandatory_letters, 0x0, 15);

		for (size_t j = 0; cell_c.pos[i].c[j] ; j++)
			mandatory_letters[j] = cell_c.pos[i].c[j];

		//Find all possible permutations given mandatory letters
		findPermutations2(chevalet, &try, 0, mandatory_letters, cell_c.range[i].s, cell_c.range[i].e);

		// vector_print(&try);

		//Eliminate impossible permutations and returns a new vector that has all possible words given possible permutations;
		try = purgeWrongPermutations(game_data, &try);

		//Elimate impossible words given pos constraints
		applyPosConstraints(&try, cell_c, i);

		// vector_print(&try);
		
		MatchVector match_try = computeMatchs(game_data, &try, p, mandatory_letters);

		matchVectorPushVector(&match_result, &match_try);

	}

	// matchVectorQuickSort(&match_result);
	// matchVectorPrint(&match_result);

	return match_result;
}

void evaluateGrid(GameData * game_data, char *chevalet) {

	//Max len is equal to the number of letter in the chevalet
	size_t max_len = strlen(chevalet);
	//Init matchVector
	MatchVector result = matchVectorInit();

	//This function will be called twice with a matrix rotation 90 in between
	for (int Y = 0; Y < GRID_SIZE; Y++) {
		for (int X = 0; X < GRID_SIZE; X++) {
			//Create a point to the current cell;
			Point p = { .y = Y, .x = X };
			// printf("@@@ P = .y %d .x %d @@@\n", Y, X);


			//Compute all constraints on this grid
			Constraints cell_c = getConstraints(game_data, p, max_len);
			if (cell_c.size == 0) {
				freeConstraints(cell_c);
				continue;
			}
			
			//Compute all possible words given cell_constraints
			MatchVector cell_result = computeCellWords(game_data, p, cell_c, chevalet); 
			printConstraint(cell_c);

			//Save words to the result vector
			matchVectorPushVector(&result, &cell_result);

			//Destroy temporary cell_vector
			matchVectorDestruct(&cell_result);

			//Free constraints
			freeConstraints(cell_c);
		}
	}
	matchVectorQuickSort(&result);
	matchVectorPrint(&result);
	matchVectorDestruct(&result);
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

	// InitWindow(screenWidth, screenHeight, "[Scrabble Trainer]");
	//
	// SetTargetFPS(60);
	//
	// Ra0000yLoop(&game_data);
	
    //Algo start//

    copyGrid(&game_data);

	// char *chevalet = rulerToStr(&game_data);
	// rotateMatrix(&game_data);
	// printGrid(game_data.grid.copy);
	char chevalet[] = "SIZHYQS";
	evaluateGrid(&game_data, chevalet);

	printf("------GRID GRID-----\n");
	printGrid(game_data.grid.grid);
	printf("----------------------\n");
	// fillAdjacentCells(&game_data, chevalet);


	printf("LETTERS = %s\n", chevalet);
    // printGrid(game_data.grid.copy);

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
