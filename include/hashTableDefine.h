/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashTableDefine.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:34 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/04 17:09:40 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_DEFINE
# define HASHMAP_DEFINE

#define BUFFER_SIZE 1024

#define screenWidth  900
#define screenHeight 900
#define	CELL_SIZE		 45
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

#endif
