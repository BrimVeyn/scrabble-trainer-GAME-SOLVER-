/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_plus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:30:07 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 17:06:21 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void vector_add(Vector *vect, size_t index, void *to_add) {
	if (index >= vect->size) {
		if (SILENT == 0)
			out_of_range();
		return ;
	}
	vect->funcs.add(&vect->data[index], to_add);
}
