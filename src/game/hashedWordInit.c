
#include "../../include/hashTableDefine.h"

void hashedWordInit(GameData *game_data) {
	game_data->hashTable = calloc(TABLE_SIZE, sizeof(word *));

    size_t length;
    char *data = getRawData(ORDERED_WORD_LIST_PATH, &length);

    if (data != MAP_FAILED) {
        // Use the data
        printf("Read %zu bytes\n", length);
		hashTableFill(game_data->hashTable, data);

        // When done, unmap the memory
        if (munmap(data, length) == -1) {
            perror("munmap");
        }
    } else {
        fprintf(stderr, "Failed to map file\n");
    }
}
