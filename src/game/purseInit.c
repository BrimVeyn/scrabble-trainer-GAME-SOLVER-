
#include "../../include/hashTableDefine.h"

struct Purse purseInit( void ) {
	struct Purse new = {
		.purse = { 9, 2, 2, 3, 15, 2, 2, 2, 8, 1, 1, 5, 3, 6, 6, 2, 1, 6, 6, 6, 6, 2, 1, 1, 1, 1 },
	};

	return new;
}
