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

Trie *TrieDestroy(Trie *const self)
{
	if (self)
	{
		TrieNodeDestroy(self->root);
		free(self);
	}
	return (NULL);
}
