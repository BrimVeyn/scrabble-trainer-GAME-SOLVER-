/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_push_back.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:54:20 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/06 12:57:13 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

static size_t vector_binary_search_insert(Vector *vect, void *value) {
	if (vect->size == 0) return 0;
	int L = 0;
	int R = vect->size;
	int m;

	while (L < R) {
		m = ((L + R) / 2);
		if (vect->funcs.cmp(value, vect->data[m]) > 0)
			L = m + 1;
		else
			R = m;
	}
	return L;
}

void vector_push_back(Vector *vect, void *value) {
    if (vect->size == vect->capacity) {
        vect->capacity *= 2;
        vect->data = realloc(vect->data, vect->capacity * sizeof(void *));
    }
    vect->data[vect->size] = vect->funcs.duplicate(value);
    vect->size++;
}

void vector_push_sorted(Vector *vect, void *value) {
	size_t index = vector_binary_search_insert(vect, value);
	vector_insert_at(vect, index, value);
}

void vector_push_vector(Vector *v1, Vector *v2) {
	for (size_t i = 0; i < v2->size; i++)
		vector_push_back(v1, v2->data[i]);
}
