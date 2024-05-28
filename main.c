/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:08:05 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/28 15:52:26 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/hashmap_define.h"
#include "include/struct.h"

#define TABLE_SIZE 150000
#define BUFFER_SIZE 1024

int s_points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2, 1, 1, 3, 8, 1, 1, 1, 1, 4, 10, 10, 10, 10};

int get_hash(char* s) {
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
	return hash % TABLE_SIZE;
}

void hashtable_add(word **hashTable, word *word, size_t index) {
	if (hashTable[index] == NULL) {
		hashTable[index] = word;
	}
	else {
		struct word *current = hashTable[index];
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = word;
	}
}

void hashtable_init(word **hashTable) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		hashTable[i] = NULL;
	}
}

void hashtable_print(word **hashTable) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		if (hashTable[i] != NULL) {
			struct word *current = hashTable[i];
			printf("-- %d\t", i);
			while (current != NULL) {
				printf("w: %-10s\ts: %d\t", current->word, current->score);
				current = current->next;
			}
			printf("\n");
		}
		else
			printf("-- %d\tNULL\n", i);
	}
}

char* get_raw_data(int fd) {
    char *buffer = NULL;
    size_t buffer_size = 0;
    ssize_t bytes_read;
    char temp_buffer[BUFFER_SIZE];

    while ((bytes_read = read(fd, temp_buffer, BUFFER_SIZE)) > 0) {
        char *new_buffer = realloc(buffer, buffer_size + bytes_read + 1);
        if (new_buffer == NULL) {
            free(buffer);
            return NULL;
        }
        buffer = new_buffer;
        for (int i = 0; i < bytes_read; i++) {
            buffer[buffer_size + i] = temp_buffer[i];
        }
        buffer_size += bytes_read;
    }

    if (buffer != NULL) {
        buffer[buffer_size] = '\0';
    }

    return buffer;
}

word *word_new(char *word, int score) {
	struct word *ptr = malloc(sizeof(struct word));

	ptr->word = strdup(word);
	ptr->score = score;
	ptr->next = NULL;
	return ptr;
}

void fill_hashtable_from_data(word **hashTable, char *raw_data) {
	int i = 0, j;
	char buffer[64];
	while (raw_data[i]) {
		j = 0;
		while (raw_data[i] && (raw_data[i] >= 'A' && raw_data[i] <= 'Z')) {
			buffer[j++] = raw_data[i++];
		}
		if (!raw_data[i]) return;
		buffer[j] = 0;
		struct word *ptr = word_new(buffer, 10);
		hashtable_add(hashTable, ptr, get_hash(ptr->word));
		i++;
	}
}

void hashtable_clear(word **hashTable) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		if (hashTable[i] != NULL) {
			struct word * current = hashTable[i];
			struct word * tmp;
			while (current != NULL) {
				tmp = current->next;
				free(current->word);
				free(current);
				current = tmp;
			}
		}
	}
}

int main(void) {

	word * hashTable[TABLE_SIZE];
	hashtable_init(hashTable);

	int fd = open("data.txt", O_RDONLY);
	if (fd == -1)
		return 1;

	char *raw_file = get_raw_data(fd);

	fill_hashtable_from_data(hashTable, raw_file);

	free(raw_file);
	close(fd);


	// hashtable_print(hashTable);
	hashtable_clear(hashTable);
}
