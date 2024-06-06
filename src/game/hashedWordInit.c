
#include "../../include/hashTableDefine.h"

void hashedWordInit(GameData *game_data) {
	game_data->hashTable = calloc(TABLE_SIZE, sizeof(word *));

	int fd = open(ORDERED_WORD_LIST_PATH, O_RDONLY);
	if (fd == -1)
		exit(EXIT_FAILURE);

	char *raw_file = getRawData(fd);

	hashTableFill(game_data->hashTable, raw_file);
	free(raw_file);
	close(fd);
}
