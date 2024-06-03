/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_erase_index.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 08:38:39 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 17:09:16 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void vector_erase_index(Vector *vect, size_t index) {
	if (!vect->size) {
		if (SILENT == 0)
			is_empty();
		return ;
	}
	void *to_erase = vect->data[index];

	for (size_t i = index; i < vect->size - 1; i++) {
		vect->data[i] = vect->data[i + 1];
	}
	vect->data[vect->size - 1] = NULL;
	vect->size--;
	free(to_erase);
}
