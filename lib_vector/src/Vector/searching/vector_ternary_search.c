/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_ternary_search.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 09:10:20 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 09:25:36 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"

static size_t vector_ternary_search_algo(int l, int r, Vector *vect, void *value) {

	if (r >= l) {
		int mid1 = l + (r - l) / 3;
		int mid2 = r - (r - l) / 3;

		if (vect->funcs.cmp(vect->data[mid1], value) == 0)
			return mid1;
		if (vect->funcs.cmp(vect->data[mid2], value) == 0)
			return mid2;
		if (vect->funcs.cmp(value, vect->data[mid1]) < 0)
			return vector_ternary_search_algo(l, mid1 - 1, vect, value);
		else if (vect->funcs.cmp(value, vect->data[mid1]) > 0)
			return vector_ternary_search_algo(mid2 + 1, r, vect, value);
		else
			return vector_ternary_search_algo(mid1 + 1, mid2 - 1, vect, value);
	}
    return vect->size;
}


Iterator vector_ternary_search(Vector *vect, void *value) {
	size_t index = vector_ternary_search_algo(0, vect->size - 1, vect, value);
	return (Iterator) {vect, index};
}
