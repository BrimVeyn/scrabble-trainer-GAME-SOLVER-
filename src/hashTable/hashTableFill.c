
#include "../../include/hashTableDefine.h"


void hashTableFill(word **hashTable, char *raw_data) {
	int i = 0, j;
	char buffer[64];
	while (raw_data[i]) {
		j = 0;
		while (raw_data[i] && (raw_data[i] >= 'A' && raw_data[i] <= 'Z')) {
			buffer[j++] = raw_data[i++];
		}
		if (!raw_data[i]) return;
		buffer[j] = 0;
		struct word *ptr = wordNew(buffer, getScore(buffer));
		hashTableAdd(hashTable, ptr, getHash(ptr->word));
		i++;
	}
}
