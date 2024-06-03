/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_quick_sort.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 17:25:59 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 09:04:07 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"

static size_t partition(Vector *vect, int low, int high) {
	void *pivot = vect->data[high];

	int i = low - 1;

	for (int j = low; j < high; j++) {
		if (vect->funcs.cmp(vect->data[j], pivot) <= 0) {
			i += 1;
			vector_swap(vect, i, j);
		}
	}
	vector_swap(vect, i + 1, high);

	return i + 1;
}

static void quick_sort(Vector *vect, int low, int high) {
	if (low < high) {
		int pi = partition(vect, low, high);

		quick_sort(vect, low, pi - 1);
		quick_sort(vect, pi + 1, high);
	}
}

void vector_quick_sort(Vector *vect) {
	quick_sort(vect, 0, vect->size - 1);
}
