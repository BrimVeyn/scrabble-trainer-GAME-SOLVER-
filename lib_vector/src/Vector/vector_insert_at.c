/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_insert_at.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 17:20:35 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/24 17:21:33 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void vector_insert_at(Vector *vect, size_t index, void *value) {
	if (index > vect->size) {
		if (SILENT == 0)
			out_of_range();
		return ;
	}
    if (vect->size == vect->capacity) {
        vect->capacity *= 2;
        vect->data = realloc(vect->data, vect->capacity * sizeof(void *));
    }
	for (size_t i = vect->size; i > index; i--) {
		vect->data[i] = vect->data[i - 1];
	}
	vect->data[index] = vect->funcs.duplicate(value);
	vect->size++;
}
