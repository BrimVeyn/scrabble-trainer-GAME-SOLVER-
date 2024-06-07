#include "../include/hashTableDefine.h"
#include <string.h>

// int main(void) {
// 	
// 	int fd_in = open("data/Data.txt", O_RDONLY);
// 	char *raw_data = getRawData(fd_in);
//
// 	Vector vect = vector_construct(STR_TYPE);
//
// 	char buffer[30];
//
// 	for (int i = 0; raw_data[i]; i++)
//     {
// 		if (raw_data[i] == '\n')
// 			continue;
// 		int j = i;
// 		for (; raw_data[j] != '\n'; j++) {
// 			buffer[j - i] = raw_data[j];
// 		}
// 		buffer[j - i] = 0;
// 		vector_push_back(&vect, buffer);
// 		i = j;
//     }
//
// 	Vector res = vector_construct(STR_TYPE);
//
// 	Iterator it = it_begin(&vect);
// 	for (; IT_NEQ(it, it_end(&vect)); it_pp(&it)) {
// 		vector_clear(&res);
// 		Iterator it2 = it_begin(&vect);
// 		for (; IT_NEQ(it2, it_end(&vect)); it_pp(&it2)) {
// 			// printf("index = %zu, size = %zu\n", it2.index, it2.vector->size - 1);
// 			if (it2.index == it2.vector->size - 1)
// 				break;
// 			if (!strcmp(vect.data[it2.index], vect.data[it.index]))
// 				continue;
// 			if (strstr(vect.data[it2.index], vect.data[it.index])) {
// 				vector_push_back(&res, vect.data[it2.index]);
// 				it_erase(&it2);
//             }
// 		}
// 		if (res.size)
//         {
// 			printf("----------\n");
// 			printf("KEY=%s\n", (char *)vect.data[it.index]);
// 			vector_print(&res);
//         }
// 	}
//
// 	
// 	free(raw_data);
// 	vector_destruct(&vect);
//
// }
