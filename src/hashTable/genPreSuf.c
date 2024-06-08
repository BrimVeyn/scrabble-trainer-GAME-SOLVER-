// #include "../include/hashTableDefine.h"
// #include <string.h>
//
// bool is_suffix(void *vs1, void *vs2) {
//     char *s1 = (char *) vs1;
//     char *s2 = (char *) vs2;
//
//     if (strlen(s1) >= strlen(s2))
//         return false;
//     if (!strncmp(s1, s2, strlen(s1)))
//         return true;
//     return false;
// }
//
// int vector_print_fd(Vector *vect, int fd) {
// 	int count = 0;
// 	if (!vect->size) {
// 		if (SILENT == 0)
// 			is_empty();
// 		return count;
// 	}
// 	for (size_t i = 0; i < vect->size; i++) {
//         write(fd, (char *)vect->data[i], strlen(vect->data[i]));
//         write(fd, "\n", 1);
// 		count++;
// 	}
// 	return count;
// }
//
// int main(void) {
// 	
// 	int fd_in = open("data/Data.txt", O_RDONLY);
//     int fd_out = open("suffix_text.tmp", O_CREAT | O_TRUNC | O_WRONLY, 0644);
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
// 			if (is_suffix(vect.data[it.index], vect.data[it2.index])) {
// 				vector_push_back(&res, vect.data[it2.index]);
// 				// it_erase(&it2);
//             }
// 		}
// 		if (res.size)
//         {
// 			write(fd_out, "----------\nKEY=", 15);
//             write(fd_out, (char *)vect.data[it.index], strlen(vect.data[it.index]));
//             write(fd_out, "\n", 1);
// 			// printf("KEY=%s\n", (char *)vect.data[it.index]);
// 			vector_print_fd(&res, fd_out);
//         }
// 	}
//
//     //merge actual dictionnarie with
//     //ED-WORD
//     //stock WORD-END
// 	
// 	free(raw_data);
// 	vector_destruct(&vect);
//     close(fd_in);
//     close(fd_out);
//
// }
