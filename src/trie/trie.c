/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trie.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 22:31:53 by pollivie          #+#    #+#             */
/*   Updated: 2024/06/10 22:31:55 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/trie.h"

TrieNode *TrieNodeCreate(void)
{
	TrieNode *self;

	self = (TrieNode *) malloc(sizeof(TrieNode));
	if (!self)
		return (NULL);
	self->is_end_of_word = false;
	memset(self->children, 0x00, ALPHABET_SIZE * sizeof(void *));
	return (self);
}

TrieNode *TrieNodeFindPrefixNode(TrieNode *const self, const char *prefix)
{
	TrieNode *temp;

	temp = self;
	while (*prefix && temp)
	{
		const uint64_t index = *prefix - 'a';
		temp = temp->children[index];
		++prefix;
	}
	return (self);
}

bool TrieNodeRemoveChild(TrieNode *self, const char *const key, const uint64_t depth)
{
	if (!self)
		return (false);
	if (*key == '\0')
	{
		if (self->is_end_of_word)
		{
			self->is_end_of_word = false;
			if (TrieNodeIsEmpty(self))
			{
				free(self);
				return (true);
			}
			return (false);
		}
	}
	else
	{
		const uint64_t index = *key - 'a';
		if (TrieNodeRemoveChild(self->children[index], key + 1, depth + 1))
		{
			self->children[index] = NULL;
			return (!self->is_end_of_word && TrieNodeIsEmpty(self));
		}
	}
	return (false);
}

bool TrieNodeIsEmpty(TrieNode *self)
{
	for (uint64_t i = 0; i < ALPHABET_SIZE; ++i)
	{
		if (self->children[i])
			return (false);
	}
	return (true);
}

void TrieNodeDestroy(TrieNode *self)
{
	if (self)
	{
		for (uint64_t i = 0; i < ALPHABET_SIZE; ++i)
		{
			if (self->children[i])
				TrieNodeDestroy(self->children[i]);
		}
		free(self);
	}
}

Trie *TrieCreate(void)
{
	Trie *self;

	self = (Trie *) malloc(sizeof(Trie));
	if (!self)
		return (NULL);
	self->root = TrieNodeCreate();
	if (!self->root)
	{
		free(self);
		return (NULL);
	}
	return (self);
}

void TrieInsert(Trie *const self, const char *const key)
{
	TrieNode   *node;
	const char *ptr;

	ptr = key;
	node = self->root;
	while (*ptr)
	{
		const uint64_t index = *ptr - 'a';
		if (!node->children[index])
			node->children[index] = TrieNodeCreate();
		node = node->children[index];
		++ptr;
	}
	node->is_end_of_word = true;
}

bool TrieSearch(Trie *const self, const char *const key)
{
	TrieNode   *node;
	const char *ptr;

	node = self->root;
	ptr = key;
	while (*ptr)
	{
		const uint64_t index = *key - 'a';
		if (!node->children[index])
			return (false);
		node = node->children[index];
		++ptr;
	}
	return (node != NULL && node->is_end_of_word);
}

bool TrieRemove(Trie *const self, const char *const key)
{
	return (TrieNodeRemoveChild(self->root, key, 0));
}

List *TrieSuggest(Trie *const self, const char *prefix)
{
	List     *suggestions;
	TrieNode *prefix_node;

	suggestions = listCreate();
	prefix_node = TrieNodeFindPrefixNode(self->root, prefix);
	if (prefix_node)
		TrieCollectSuggestions(prefix_node, strdup(prefix), suggestions);
	return (suggestions);
}

void TrieCollectSuggestions(TrieNode *const node, const char *prefix, List *suggestions)
{
	if (!node)
		return;
	if (node->is_end_of_word)
		listPushBack(suggestions, (uintptr_t) strdup(prefix));
	for (uint64_t i = 0; i < ALPHABET_SIZE; ++i)
	{
		if (node->children[i])
		{
			uint64_t prefix_len = strlen(prefix);
			uint64_t new_len = prefix_len + 2;
			char    *prefix_buffer = (char *) malloc(new_len);
			if (prefix_buffer)
			{
				snprintf(prefix_buffer, new_len, "%s%c", prefix, (uint8_t) ('a' + i));
				TrieCollectSuggestions(node->children[i], prefix_buffer, suggestions);
				free(prefix_buffer);
			}
		}
	}
}

Trie *TrieDestroy(Trie *const self)
{
	if (self)
	{
		TrieNodeDestroy(self->root);
		free(self);
	}
	return (NULL);
}
