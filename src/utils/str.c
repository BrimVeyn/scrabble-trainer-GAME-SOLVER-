#include "../../include/hashTableDefine.h"


char* sort_ascii(char* buf) {
	size_t len = strlen(buf);
	char *ptr = strdup(buf);

    // Simple bubble sort
    for (size_t i = 0; i < len - 1; i++) {
        for (size_t j = 0; j < len - i - 1; j++) {
            if (ptr[j] > ptr[j + 1]) {
                // Swap characters
                char temp = ptr[j];
                ptr[j] = ptr[j + 1];
                ptr[j + 1] = temp;
            }
        }
    }
    return ptr;
}

char *str_append(char *s, char c) {
	s[strlen(s)] = c;
	return s;
}

char *str_remove(char *s) {
	s[strlen(s) - 1] = 0;
	return s;
}
