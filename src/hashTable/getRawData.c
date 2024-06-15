
#include "../../include/hashTableDefine.h"

int s_points[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2, 1, 1, 3, 8, 1, 1, 1, 1, 4, 10, 10, 10, 10};

void *getRawData(const char *filename, size_t *length) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return NULL;
    }

    *length = st.st_size;
    void *data = mmap(NULL, *length, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return NULL;
    }

    close(fd);
    return data;
}

size_t getScore(char *word){
	size_t score = 0;
	for (int i = 0; word[i]; i++){
		score += s_points[word[i] - 'A'];
	}
	return score;
}
