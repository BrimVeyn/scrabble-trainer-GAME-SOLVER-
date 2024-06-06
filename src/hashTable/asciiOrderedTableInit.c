/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asciiOrderedTableInit.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 15:46:38 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/06 10:29:37 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/hashTableDefine.h"
#include "struct.h"

void asciiOrderedClear(struct AsciiOrderedTable **table) {
	for (int i = 0; i < ASCII_ORDERED_SIZE; i++) {
		if (table[i] != NULL) {
			struct AsciiOrderedTable * current = table[i];
			struct AsciiOrderedTable * tmp;
			while (current != NULL) {
				tmp = current->next;
				vector_destruct(&current->value_vect);
				free(current->key);
				free(current);
				current = tmp;
			}
		}
	}
}

void asciiOrderedAdd(AsciiOrderedTable **table, AsciiOrderedTable *new, size_t index) {
	if (table[index] == NULL) {
		table[index] = new;
	}
	else {
		struct AsciiOrderedTable *current = table[index];
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new;
	}
}

AsciiOrderedTable *asciiOrderedNew(char *key, Vector *vect) {
	struct AsciiOrderedTable *ptr = malloc(sizeof(struct AsciiOrderedTable));

	ptr->key = strdup(key);
	ptr->value_vect = *vect;
	ptr->next = NULL;
	return ptr;
}

int getHashAscii(char* s) {
	int n = strlen(s);
    long long p = 31, m = 1e9 + 7;
    long long hash = 0;
    long long p_pow = 1;
    for(int i = 0; i < n; i++) {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
	if (hash < 0)
		hash = -hash;
	return hash % ASCII_ORDERED_SIZE;
}

// int count = 0;

void asciiOrderedFill(AsciiOrderedTable **table, char *raw_data) {
    int i = 0;
    
    while (raw_data[i]) {
        Vector vect = vector_construct(STR_TYPE);
        char key[30] = {0};
        
        // Extract key
        int j = 0;
        while (raw_data[i] && raw_data[i] != '=') {
            i++;
        }
        
        // Check if we reached the end of raw_data
        if (!raw_data[i]) {
            vector_destruct(&vect);
            break;
        }
        
        i++;  // Skip the '=' character
        
        while (raw_data[i] && raw_data[i] != '\n') {
            key[j++] = raw_data[i++];
        }
        
        // Skip the newline character
        if (raw_data[i] == '\n') {
            i++;
        }
        
        // Extract values for the vector
        while (raw_data[i] && raw_data[i] != '-') {
            char buffer[30] = {0};
            int k = 0;
            
            while (raw_data[i] && raw_data[i] != '=') {
                if (raw_data[i] == '-') {
                    break;
                }
                i++;
            }
            
            i += 2;  // Skip the '=' and the next character
            
            while (raw_data[i] && raw_data[i] != '-' && raw_data[i] != '\n') {
                buffer[k++] = raw_data[i++];
            }
            
            if (strlen(buffer) > 0) {
				// count++;
                vector_push_back(&vect, buffer);
            }
            
            // Skip to the next item or exit if end of vector
            if (raw_data[i] == '-') {
                i++;
            }
        }
        
        struct AsciiOrderedTable *ptr = asciiOrderedNew(key, &vect);
        asciiOrderedAdd(table, ptr, getHashAscii(ptr->key));
    }
}

void asciiOrderedInit(AsciiOrderedTable **table) {
	for (int i = 0; i < ASCII_ORDERED_SIZE; i++) {
		table[i] = NULL;
	}
}

void asciiOrderedPrint(AsciiOrderedTable **table) {
	for (int i = 0; i < ASCII_ORDERED_SIZE; i++) {
		if (table[i] != NULL) {
			struct AsciiOrderedTable *current = table[i];
			printf("-- %d\t", i);
			while (current != NULL) {
				printf("w: %-10s\n", current->key);
				vector_print(&current->value_vect);
				current = current->next;
			}
			printf("\n");
		}
		else
			printf("-- %d\tNULL\n", i);
	}
}

void asciiOrderedTableInit(GameData *game_data) {
	game_data->asciiTable = calloc(ASCII_ORDERED_SIZE, sizeof(AsciiOrderedTable *));

	int sorted_fd = open("data/text.txt", O_RDONLY);
	if (sorted_fd == -1)
		exit(EXIT_FAILURE);

	char *raw_sorted = getRawData(sorted_fd);

	asciiOrderedFill(game_data->asciiTable, raw_sorted);
	free(raw_sorted);
	// asciiOrderedPrint(table);
	// asciiOrderedClear(table);

	// printf("count = %d\n", count);
	

}

// int main(void) {
// 	AsciiOrderedTable *table[ASCII_ORDERED_SIZE];
// 	asciiOrderedInit(table);
//
// 	// int sorted_fd = open("sortedByAscii.txt", O_RDONLY);
// 	int sorted_fd = open("src/hashTable/here.txt", O_RDONLY);
// 	if (sorted_fd == -1)
//     {
// 		printf("fails");
// 		exit(EXIT_FAILURE);
//     }
//
// 	char *raw_sorted = getRawData(sorted_fd);
//
// 	asciiOrderedFill(table, raw_sorted);
// 	free(raw_sorted);
// 	// asciiOrderedPrint(table);
// 	asciiOrderedClear(table);
//
// 	// printf("count = %d\n", count);
// 	
//
// }
