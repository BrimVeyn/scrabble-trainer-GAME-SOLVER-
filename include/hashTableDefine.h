/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashTableDefine.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:34 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/11 10:55:49 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_DEFINE
# define HASHMAP_DEFINE

#define BUFFER_SIZE 1024

#define screenWidth  1200
#define screenHeight 1200
#define	CELL_SIZE		 55
#define DRAW_OFFSET_X     5
#define DRAW_OFFSET_Y     5

#include "struct.h"
#include "../lib/raylib/include/raylib.h"
#include "../lib_vector/include/struct.h"
#include "../lib_vector/include/Vector/vector_define.h"
#include "../lib_vector/include/Iterator/iterator_define.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

char	**ft_split(char const *s, char c);
int hashTableFind(word **hashTable, char *to_find);
void hashTableClear(word **hashTable);
word *wordNew(char *word, int score);
void hashTableAdd(word **hashTable, word *word, size_t index);
void hashTableInit(word **hashTable);
void hashTablePrint(word **hashTable);
void hashTableFill(word **hashTable, char *raw_data);

int getHash(char* s);
char* getRawData(int fd);
size_t getScore(char *word);

//tree
char ***create_buffers(char *raw_file);
void free_3x_char(char ***buffer);
list mask_word(char *** database, char mask[15] );
Grid gridInit(void);
struct Ruler rulerInit(Purse *purse);
struct Purse purseInit( void );
void hashedWordInit(GameData *game_data);
GameData gameDataInit( void );


char	*ft_itoa(int n);


void asciiOrderedTableInit(GameData * game_data);
void asciiOrderedClear(AsciiOrderedTable **table);
Vector *asciiOrderedFind(AsciiOrderedTable **hashTable, char *to_find);

void findMatchPrint(FindMatch match);
void printGrid(int grid[15][15]);
void  printPoints(Point *points);
void drawRuler(Ruler *ruler);
TextVar getTextVar(char *text, int font_size);
TextVar getText(int c, int font_size);
Color getColor(int c);

int countRulerOnGrid(Ruler ruler);
int checkAlignment(GameData *game_data);
void updateTourHighest(TourManager * tour, int x, int y);
int calcWordScore(Match word, int g_modifier[15][15], int g_tour[15][15], TourManager *tour);
int calcWordListScore(GameData * game_data);
void drawTourWordOutline(GameData * game_data);
void drawValidate(GameData *game_data);
void drawPurse(GameData * game_data);
void drawTourScore(GameData * game_data);
void drawTotalScore(GameData * game_data);
void mainMenu(GameData *game_data);
void resetRuler(GameData *game_data, Vector2 mouseP);
void refillRuler(GameData *game_data);
void matrixAdd(GameData *game_data);
void scoreAdd(GameData * game_data);
void validateTour(GameData *game_data, Vector2 mouseP);
void checkTourWord(GameData *game_data);
void processWord(GameData *game_data, int *j, Match m);
Rectangle getMatchRect(Match m, int dir, Ruler ruler, int i);
void dragRuler(GameData *game_data, Vector2 mouseP);
void dropRuler(GameData *game_data, Vector2 mouseP);
Match getWordMatch(Point ruler_cell, GameData *game_data, int direction);
Point findClosestEmpty(Point cell, Grid grid);
Neighbor contactPoint(Point c, GameData *game_data);
void drawGrid(Grid *grid);
bool findWordList(Match word_list[30], Match m);
bool areWordsValid(TourManager tour);
char* sort_ascii(char* buf);
char *str_append(char *s, char c);
char *str_remove(char *s);
int findWordScore(Match word, int g_modifier[15][15], int g_grid[15][15]);
int calcMatchWordScore(Match word, int g_modifier[15][15], int g_tour[15][15], int scrabble);

MatchVector matchVectorInit( void );
void matchVectorPushBack(MatchVector * vect, Match match);
void matchVectorPrint(MatchVector * vect);
void matchVectorDestruct (MatchVector * vect);
void matchVectorPushVector(MatchVector *v1, MatchVector *v2);
void matchVectorQuickSort(MatchVector *vect);


MatchVector computeAdjacent(GameData * game_data, Vector * word_list, Point cell);
bool isLetter(char c);
bool isVert(GameData * game_data, Point cell);
bool isHori(GameData * game_data, Point cell);
bool isVertHori(GameData * game_data, Point cell);
#endif
