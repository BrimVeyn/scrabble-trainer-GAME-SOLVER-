/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 12:06:28 by pollivie          #+#    #+#             */
/*   Updated: 2024/06/12 12:06:29 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"
#include "trie.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *to_lowercase(char *str)
{
	if (!str)
		return (NULL);
	for (int i = 0; str[i] != 0; ++i)
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = (str[i] - 'A') + 'a';
	return (str);
}
char **split(const char *str, const char delimiter, int *count)
{
	char **tokens;
	char  *buffer;
	int    length, numTokens, token_index, i, j;

	numTokens = 1;
	length = strlen(str);
	for (i = 0; i < length; i++)
	{
		if (str[i] == delimiter)
			numTokens++;
	}

	tokens = (char **) malloc((numTokens + 1) * sizeof(char *));
	if (tokens == NULL)
		return NULL;

	buffer = (char *) malloc((length + 1) * sizeof(char));
	if (buffer == NULL)
	{
		free(tokens);
		return NULL;
	}

	j = 0;
	token_index = 0;
	for (i = 0; i < length; i++)
	{
		if (str[i] == delimiter)
		{
			buffer[j] = '\0';
			tokens[token_index] = strdup(buffer);
			if (tokens[token_index] == NULL)
			{
				for (int k = 0; k < token_index; k++)
					free(tokens[k]);
				free(tokens);
				free(buffer);
				return NULL;
			}
			token_index++;
			j = 0;
		}
		else
		{
			buffer[j] = str[i];
			j++;
		}
	}
	buffer[j] = '\0';
	tokens[token_index] = strdup(buffer);
	if (tokens[token_index] == NULL)
	{
		for (int k = 0; k < token_index; k++)
			free(tokens[k]);
		free(tokens);
		free(buffer);
		return NULL;
	}
	free(buffer);
	tokens[++token_index] = NULL; // Null-terminate the list of tokens
	*count = numTokens;
	return tokens;
}

char *openReadToEndAlloc(const char *filename)
{
	FILE *handle = fopen(filename, "r");
	if (!handle)
		return NULL;

	if (fseek(handle, 0, SEEK_END) != 0)
	{
		fclose(handle);
		return NULL;
	}
	long size = ftell(handle);
	if (size < 0)
	{
		fclose(handle);
		return NULL;
	}
	rewind(handle);

	char *buffer = (char *) malloc((size + 1) * sizeof(char));
	if (!buffer)
	{
		fclose(handle);
		return NULL;
	}

	size_t read_bytes = fread(buffer, 1, size, handle);
	if (read_bytes != size)
	{
		fclose(handle);
		free(buffer);
		return NULL;
	}
	buffer[size] = '\0';
	fclose(handle);
	return buffer;
}

void printSuggestionList(List *list)
{
	Node *temp;
	if (!list)
		return;
	temp = list->head;
	while (temp)
	{
		printf("%s\n", (char *) temp->data);
		temp = temp->next;
	}
}

void printResult(char *prefix_buffer, List *list)
{
	if (!prefix_buffer || !list)
		return;
	printf("For the prefix: %s there are %d suggestions in the trie\n", prefix_buffer,
	       (int32_t) list->size);
	printSuggestionList(list);
}

int main(int argc, char **argv)
{
	Trie  *word_db = NULL;
	char  *file_buffer = NULL;
	char **file_words = NULL;
	int    wcount = 0;

	if (argc > 1)
	{
		word_db = TrieCreate();
		if (!word_db)
		{
			fprintf(stderr, "Failed to create Trie\n");
			return EXIT_FAILURE;
		}

		file_buffer = openReadToEndAlloc(argv[1]);
		if (!file_buffer)
		{
			fprintf(stderr, "Failed to read file\n");
			TrieDestroy(word_db);
			return EXIT_FAILURE;
		}

		file_words = split(file_buffer, '\n', &wcount);
		if (!file_words)
		{
			fprintf(stderr, "Failed to split file contents\n");
			free(file_buffer);
			TrieDestroy(word_db);
			return EXIT_FAILURE;
		}

		for (int i = 0; i < wcount; i++)
		{
			to_lowercase(file_words[i]);
			TrieInsert(word_db, file_words[i]);
		}

		char prefix_buffer[100];
		printf("Enter a prefix to see suggestions (or 'exit' to quit):\n");
		while (1)
		{
			memset(prefix_buffer, 0, sizeof(prefix_buffer));
			printf("Prefix: ");
			if (scanf("%99s", prefix_buffer) != 1)
			{
				printf("Invalid input, please try again.\n");
				continue;
			}
			if (strcmp(prefix_buffer, "exit") == 0)
				break;

			List *suggestions = TrieSuggest(word_db, prefix_buffer);
			if (suggestions)
			{
				to_lowercase(prefix_buffer);
				printResult(prefix_buffer, suggestions);
				listDestroy(suggestions);
			}
			else
				printf("No suggestions found.\n");
		}

		free(file_buffer);
		for (int i = 0; i < wcount; i++)
			free(file_words[i]);
		free(file_words);
		TrieDestroy(word_db);
	}
	return EXIT_SUCCESS;
}
