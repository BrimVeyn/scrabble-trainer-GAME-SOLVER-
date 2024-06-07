/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_at.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 08:36:29 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 17:08:11 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void *vector_at(Vector *vect, size_t index) {
	if ((index) >= vect->size)  {
		if (SILENT == 0)
			out_of_range();
		return NULL;
	}
	return vect->data[index];
}
