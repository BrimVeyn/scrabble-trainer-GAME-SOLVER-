/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matchVector.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 14:31:09 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/15 00:19:40 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/hashTableDefine.h"

#define DARK_GREENF "\x1b[38;2;0;128;0m"

const char* getModifiedColor(float score) {
    static char modifiedColor[24]; // Sufficient size to hold the modified color string
    int r = 214; // Original red component
    int g = 150; // Original green component
    int b = 60; // Original blue component

    // Ensure score is not zero to avoid division by zero
    if (score == 0) score = 1;

    // Divide the color components by the score
    r *= score;
    // g *= score;
    b *= score;

    // Create the modified color string
    snprintf(modifiedColor, sizeof(modifiedColor), "\x1b[38;2;%d;%d;%dm", r, g, b);
    
    return modifiedColor;
}

MatchVector *matchVectorInit( void ) {
	MatchVector *new = malloc(sizeof(struct MatchVector));

	new->data = calloc(10, sizeof(struct Match));
	new->size = 0;
	new->capacity = 10;

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

void matchVectorPrint(MatchVector * vect, int debug) {
	if (debug) {
		for (size_t i = 0; i < vect->size; i++) {
			printf("word[%zu] = { .word = %s\n .start = %d,\n .end = %d,\n .saved_coord = %d,\n", i, vect->data[i].word, vect->data[i].start, vect->data[i].end, vect->data[i].save_coord);
			if (vect->data[i].dir == VERTICAL)
				printf(" .dir = VERTICAL\n");
			else
				printf(" .dir = HORIZONTAL\n");
			printf(" .score =" DARK_REDF " %d" RESET_COLOR "\n }\n", vect->data[i].score);
		}
	} else {
		int max_score = vect->data[vect->size - 1].score;
		for (size_t i = 0; i < vect->size; i++) {
			float score_on_max = (float)(vect->data[i].score) / max_score;
			printf("word[%4zu] = "DARK_REDF"%-15s"RESET_COLOR"| %s%-4d"RESET_COLOR" | %s"RESET_COLOR"\n", i, vect->data[i].word, getModifiedColor(score_on_max), vect->data[i].score, (vect->data[i].dir == VERTICAL) ? YELLOW_F"V" : YBLUE_F"H");
		}
	}
}

void matchVectorDestruct (MatchVector * vect) {
	free(vect->data);
  free(vect);
}

void matchVectorPushVector(MatchVector *v1, MatchVector *v2) {
  if (v2->size == 0)
    return ;
	for (size_t i = 0; i < v2->size; i++) {
		matchVectorPushBack(v1, v2->data[i]);
  }
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
