/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trie.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 22:32:18 by pollivie          #+#    #+#             */
/*   Updated: 2024/06/23 15:46:15 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#define ALPHABET_SIZE 26
#include "list.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE + 1];

} TrieNode;

typedef struct Trie
{
	TrieNode *root;

} Trie;

TrieNode *TrieNodeCreate(void);
TrieNode *TrieNodeFindPrefixNode(TrieNode *const self, const char *prefix);
void      TrieNodeDestroy(TrieNode *self);
bool TrieNodeRemoveChild(TrieNode *self, const char *const key, const uint64_t depth);
bool TrieNodeIsEmpty(TrieNode *self);

Trie *TrieCreate(void);
void  TrieInsert(Trie *const self, const char *const key);
bool  TrieSearch(Trie *const self, const char *const key);
bool  TrieRemove(Trie *const self, const char *const key);
List *TrieSuggest(Trie *const self, const char *prefix);
void TrieCollectSuggestions(TrieNode *const node, const char *prefix, List *suggestions);
Trie *TrieDestroy(Trie *const self);

// EXPERIMENTAL
//
List *TrieSearchPattern(Trie *const self, char *pattern);

#endif
