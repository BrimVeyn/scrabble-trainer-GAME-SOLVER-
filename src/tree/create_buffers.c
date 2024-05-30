/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_buffers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:20:20 by nbardavi          #+#    #+#             */
/*   Updated: 2024/05/29 17:45:30 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/struct.h"
#include "../../include/hashTableDefine.h"
#include <string.h>

char ***create_buffers(char *raw_file){
	int i = 0;
	int count = 0;
	int save = 0; //save where first current_letter is

	char actual_letter = 'A';
	char *** sorted_file;
	sorted_file = malloc(sizeof(char**) * 27);
	sorted_file[26] = NULL;
	
	while(actual_letter <= 'Z'){
		save = i;
		count = 0;
		while(raw_file[i] && raw_file[i] == actual_letter){
			count ++;
			//go to next word
			while(raw_file[i] && raw_file[i] != '\n'){i++;}
			i++;
		}
		sorted_file[actual_letter - 'A'] = malloc(sizeof(char*) * (count + 1));
		sorted_file[actual_letter - 'A'][count] = NULL;
		
		int spos = save; //start positon of the current word
		int epos = save; // end postion of the current word
		count = 0; // number of the current word
		int j = save;
		while(j < i){
			spos = j;
			//go to the end of the word
			while(raw_file[j] && raw_file[j] != '\n'){j++;}
			
			epos = j;

			//Alloc + copy
			sorted_file[actual_letter - 'A'][count] = malloc(sizeof(char) * (epos - spos + 1));
			sorted_file[actual_letter - 'A'][count][epos - spos] = '\0';
			memcpy(sorted_file[actual_letter - 'A'][count], &raw_file[spos], epos - spos);

			count++;
			j++;
		}
		actual_letter++;
	}
	return sorted_file;
}

void free_3x_char(char ***buffer){
	for (int i = 0; buffer[i]; i++){
		for(int j = 0; buffer[i][j]; j++){
			free(buffer[i][j]);
			buffer[i][j] = NULL;
		}
		free(buffer[i]);
		buffer[i] = NULL;
	}
	free(buffer);
	buffer = NULL;
}

size_t mask_length(char mask[15]){
	int c;
	for (int i = 0; i < 15; i++)
		if (mask[i] != '*') c = i ;
	return c;
}

int check_word(char *word, char mask[15]){
	
	int len = strlen(word);
	char *cpy = malloc(len * sizeof(char) + 1);
	cpy[len] = '\0';
	int i = 0;

	while(i < len){
		if (mask[i] != '*')
			cpy[i] = word[i];
		else
			cpy[i] = '*';
		i++;
	}
	int result = strncmp(mask, cpy, len);
	free(cpy);
	return result;
}

list list_init(int size){
	list new;
	new.content = malloc(sizeof(char*) * (size + 1));
	new.size = 0;
	new.capacity = size + 1;
	return new;
}

void list_shrink_to_fit(list *list){
	if (list->size != list->capacity - 1){
		list->content = realloc(list->content, sizeof(char *) * list->size + 1);
		list->content[list->size] = NULL;
	}
}

void addToList(char *word, list *list){
	int len = strlen(word);

	if (list->size == list->capacity - 1){
		list->content = realloc(list->content, (list->capacity - 1) * 2 + 1);
		list->capacity = (list->capacity - 1) * 2 + 1;
	}
	list->content[list->size] = malloc(sizeof(char) * len);
	memcpy(list->content[list->size], word, len + 1);
	list->size++;
}

void is_first_letter(char **db, char mask[15], list *list){
	
	int i = 0;
	size_t min_length = strlen(mask);

	while(db[i]){
		if (strlen(db[i]) >= min_length && check_word(db[i], mask) == 0){
			addToList(db[i], list);
		}
		i++;
	}
}

/*
 * mask :
 * "*" for every char
 * "P" precise char
 * example: [L, *, *, P] can return LOOP
 */
list mask_word(char *** database, char mask[15] ){
	
	list list = list_init(8);
	if (mask[0] != '*'){ is_first_letter(database[mask[0] - 'A'], mask, &list);}
	list_shrink_to_fit(&list);
	// int min_length = mask_length(mask);
	return list;
}
