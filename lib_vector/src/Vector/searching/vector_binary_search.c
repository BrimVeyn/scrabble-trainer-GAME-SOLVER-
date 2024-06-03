/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_binary_search.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 09:05:44 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 09:52:24 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"

static size_t vector_binary_search_algo(Vector *vect, void *value) {
	if (vect->size == 0) return 0;
	int L = 0;
	int R = vect->size - 1;
	int m;

	while (L <= R) {
		m = ((L + R) / 2);
		if (vect->funcs.cmp(vect->data[m], value) < 0)
			L = m + 1;
		else if (vect->funcs.cmp(vect->data[m], value) > 0)
			R = m - 1;
		else
			return m;
	}
	return vect->size;
}

Iterator vector_binary_search(Vector *vect, void *value) {
	size_t index = vector_binary_search_algo(vect, value);
	return (Iterator) {vect, index};
}
