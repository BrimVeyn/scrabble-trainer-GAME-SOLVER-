/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matchVector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:31:09 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/10 16:42:19 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/hashTableDefine.h"

MatchVector matchVectorInit( void ) {
	MatchVector new = {
		.data = calloc(10, sizeof(Match)),
		.size = 0,
		.capacity = 10,
	};
	return new;
}

void matchVectorPushBack(MatchVector * vect, Match match) {
	if (vect->size == vect->capacity) {
		vect->capacity *= 2;
		vect->data = realloc(vect->data, vect->capacity * sizeof(Match));
	}
	vect->data[vect->size] = match;
	vect->size++;
}

void matchVectorPrint(MatchVector * vect) {
	for (size_t i = 0; i < vect->size; i++) {
		printf("word[%zu] = { .word = %s\n .start = %d,\n .end = %d,\n .saved_coord = %d,\n", i, vect->data[i].word, vect->data[i].start, vect->data[i].end, vect->data[i].save_coord);
		if (vect->data[i].dir == VERTICAL)
			printf(" .dir = VERTICAL\n");
		else
			printf(" .dir = HORIZONTAL\n");
		printf(" .score =" DARK_REDF " %d" RESET_COLOR "\n }\n", vect->data[i].score);
	}
}

void matchVectorDestruct (MatchVector * vect) {
	free(vect->data);
}

void matchVectorPushVector(MatchVector *v1, MatchVector *v2) {
	for (size_t i = 0; i < v2->size; i++)
		matchVectorPushBack(v1, v2->data[i]);
}

void matchVectorSwap(MatchVector *vect, size_t i1, size_t i2) {
    Match tmp = vect->data[i1];
    vect->data[i1] = vect->data[i2];
    vect->data[i2] = tmp;
}

static size_t partition(MatchVector *vect, int low, int high) {
    int pivot = vect->data[high].score;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (vect->data[j].score <= pivot) {
            i++;
            matchVectorSwap(vect, i, j);
        }
    }
    matchVectorSwap(vect, i + 1, high);
    return i + 1;
}

static void quick_sort(MatchVector *vect, int low, int high) {
    if (low < high) {
        int pi = partition(vect, low, high);

        quick_sort(vect, low, pi - 1);
        quick_sort(vect, pi + 1, high);
    }
}

void matchVectorQuickSort(MatchVector *vect) {
    quick_sort(vect, 0, vect->size - 1);
}
