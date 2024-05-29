/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_buffers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbardavi <nbabardavid@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 10:20:20 by nbardavi          #+#    #+#             */
/*   Updated: 2024/05/29 11:24:07 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/struct.h"
#include "../../include/hashTableDefine.h"

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
