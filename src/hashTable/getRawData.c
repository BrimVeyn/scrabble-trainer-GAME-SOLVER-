
#include "../../include/hashTableDefine.h"

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

size_t getScore(char *word){
	size_t score = 0;
	for (int i = 0; word[i]; i++){
		score += s_points[word[i] - 'A'];
	}
	return score;
}
