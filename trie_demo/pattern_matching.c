/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:35:32 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/23 15:37:05 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void TrieCollectWords(TrieNode *node, char *prefix, List *suggestions) {
    if (!node)
        return;
    if (isEndOfWord(node))
        listPushBack(suggestions, (uintptr_t)strdup(prefix));

    char new_prefix[strlen(prefix) + 2];
    for (uint64_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            snprintf(new_prefix, sizeof(new_prefix), "%s%c", prefix, 'a' + i);
            TrieCollectWords(node->children[i], new_prefix, suggestions);
        }
    }
}

void TrieBFSForWildcards(TrieNode *node, uint64_t depth, List *nodes) {
    if (depth == 6) {
        listPushBack(nodes, (uintptr_t)node);
        return;
    }

    for (uint64_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (node->children[i]) {
            TrieBFSForWildcards(node->children[i], depth + 1, nodes);
        }
    }
}

List *TrieSearchPattern(Trie *const self, const char *pattern) {
    List *nodes, *suggestions;
    TrieNode *node;
    char prefix[7] = {0};

    // Handle the case where the pattern starts with six wildcards
    nodes = listCreate();
    TrieBFSForWildcards(self->root, 0, nodes);

    suggestions = listCreate();
    while (!listIsEmpty(nodes)) {
        node = (TrieNode *)listPopFront(nodes);
        if (node->children['C' - 'A']) {
            snprintf(prefix, sizeof(prefix), "!!!!!!C");
            TrieCollectWords(node->children['C' - 'A'], prefix, suggestions);
        }
    }

    listDestroy(nodes);
    return suggestions;
}
