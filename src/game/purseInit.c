
#include "../../include/hashTableDefine.h"

struct Purse purseInit( void ) {
	struct Purse new = {
		.purse = { 9, 2, 2, 3, 15, 2, 2, 2, 8, 1, 1, 5, 3, 6, 6, 2, 1, 6, 6, 6, 6, 2, 1, 1, 1, 1 },
	};
	Vector vect = vector_construct(INT_TYPE);
	for (int i = 0; i < 26; i++) {
		while (new.purse[i] != 0) {
			vector_push_back(&vect, INT_L('A' + i));
			new.purse[i]--;
		}
	}
	new.purse_vect = vect;
	// vector_print(&vect);
	return new;
}
