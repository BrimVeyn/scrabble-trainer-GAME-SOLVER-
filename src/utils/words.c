#include "../../include/hashTableDefine.h"


bool findWordList(Match word_list[30], Match m) {
	//return true if a Match m is a duplicate of any in the word list
	for (int i = 0; word_list[i].dir != 0; i++) {
		if (!strcmp(word_list[i].word, m.word) && word_list[i].start == m.start && word_list[i].end == m.end && word_list[i].dir == m.dir)
			return true;
	}
	return false;
}

bool areWordsValid(TourManager tour) {
	//return true if all words in the list are found in the hash_table
	if (tour.word_list[0].dir == 0)
		return false;
	for (int i = 0; tour.word_list[i].dir != 0; i++) {
		if (tour.word_list[i].validated == false)
			return false;
	}
	return true;
}
