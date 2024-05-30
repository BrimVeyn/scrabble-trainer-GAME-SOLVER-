/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashTableDefine.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 14:57:34 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/30 14:57:35 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHMAP_DEFINE
# define HASHMAP_DEFINE

#define TABLE_SIZE 150000
#define BUFFER_SIZE 1024

#define screenWidth  1000
#define screenHeight 1000

#include "struct.h"
#include "../lib/raylib/include/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

char	**ft_split(char const *s, char c);
int hashTableFind(word **hashTable, char *to_find);
void hashTableClear(word **hashTable);
word *wordNew(char *word, int score);
int getHash(char* s);
void hashTableAdd(word **hashTable, word *word, size_t index);
void hashTableInit(word **hashTable);
void hashTablePrint(word **hashTable);

//tree
char ***create_buffers(char *raw_file);
void free_3x_char(char ***buffer);
list mask_word(char *** database, char mask[15] );

#endif
