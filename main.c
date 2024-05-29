/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 13:08:05 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/29 17:02:39 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/hashTableDefine.h"
#include "include/struct.h"
#include <string.h>

int s_points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2, 1, 1, 3, 8, 1, 1, 1, 1, 4, 10, 10, 10, 10};

char* getRawData(int fd) {
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

void fillHashTable(word **hashTable, char *raw_data) {
	int i = 0, j;
	char buffer[64];
	while (raw_data[i]) {
		j = 0;
		while (raw_data[i] && (raw_data[i] >= 'A' && raw_data[i] <= 'Z')) {
			buffer[j++] = raw_data[i++];
		}
		if (!raw_data[i]) return;
		buffer[j] = 0;
		struct word *ptr = wordNew(buffer, 10);
		hashTableAdd(hashTable, ptr, getHash(ptr->word));
		i++;
	}
}

grid gridInit(void) {
	struct grid my_grid = {
		.grid =		{ {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, 
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 'P', 32, 'P', 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 'I', 32, 'O', 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 'A', 'L', 'O', 'R', 'S', 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 'E', 32, 'C', 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},
					  {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, },
		.modifier = { {TWORD, 0, 0, DLETTER, 0, 0, 0, TWORD, 0, 0, 0, DLETTER, 0, 0, TWORD},
			       	  {0, DWORD, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, DWORD, 0},
					  {0, 0, DWORD, 0, 0, 0, DLETTER, 0, DLETTER, 0, 0, 0, DWORD, 0, 0},
					  {DLETTER, 0, 0, DWORD, 0, 0, 0, DLETTER, 0, 0, 0, DWORD, 0, 0, DLETTER},
					  {0, 0, 0, 0, DWORD, 0, 0, 0, 0, 0, DWORD, 0, 0, 0, 0},
					  {0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0},
					  {0, 0, DLETTER, 0, 0, 0, DLETTER, 0, DLETTER, 0, 0, 0, DLETTER, 0, 0},
					  {TWORD, 0, 0, DLETTER, 0, 0, 0, DWORD, 0, 0, 0, DLETTER, 0, 0, TWORD},
					  {0, 0, DLETTER, 0, 0, 0, DLETTER, 0, DLETTER, 0, 0, 0, DLETTER, 0, 0},
					  {0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0},
					  {0, 0, 0, 0, DWORD, 0, 0, 0, 0, 0, DWORD, 0, 0, 0, 0},
					  {DLETTER, 0, 0, DWORD, 0, 0, 0, DLETTER, 0, 0, 0, DWORD, 0, 0, DLETTER},
					  {0, 0, DWORD, 0, 0, 0, DLETTER, 0, DLETTER, 0, 0, 0, DWORD, 0, 0},
			       	  {0, DWORD, 0, 0, 0, TLETTER, 0, 0, 0, TLETTER, 0, 0, 0, DWORD, 0},
					  {TWORD, 0, 0, DLETTER, 0, 0, 0, TWORD, 0, 0, 0, DLETTER, 0, 0, TWORD} },
	};
	return my_grid;
}

void gridDisplay(grid grid) {
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (grid.modifier[i][j] == TWORD)
				printf(DARK_RED "[%c]" RESET_COLOR, grid.grid[i][j]);
			else if (grid.modifier[i][j] == DWORD)
				printf(LIGHT_RED "[%c]" RESET_COLOR, grid.grid[i][j]);
			else if (grid.modifier[i][j] == TLETTER)
				printf(DARK_BLUE "[%c]" RESET_COLOR, grid.grid[i][j]);
			else if (grid.modifier[i][j] == DLETTER)
				printf(LIGHT_BLUE "[%c]" RESET_COLOR, grid.grid[i][j]);
			else
				printf(WHITE "[%c]" RESET_COLOR, grid.grid[i][j]);
		}
		printf("\n");
	}
}

int main(void) {


	struct grid s_grid = gridInit();

	gridDisplay(s_grid);

	// word * hashTable[TABLE_SIZE];
	// hashTableInit(hashTable);
	//
	// int fd = open("data/Data.txt", O_RDONLY);
	// if (fd == -1)
	// 	return 1;
	//
	// char *raw_file = getRawData(fd);
	//
	// fillHashTable(hashTable, raw_file);
	//
	// free(raw_file);
	// close(fd);
	//
	// printf("%d\n", hashTableFind(hashTable, "MANGERENT"));
	//
	// // hashTablePrint(hashTable);
	// hashTableClear(hashTable);
}
