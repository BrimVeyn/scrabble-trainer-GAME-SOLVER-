/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 08:42:12 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 17:09:42 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void vector_print(Vector *vect) {
	if (!vect->size) {
		if (SILENT == 0)
			is_empty();
		return ;
	}
	for (size_t i = 0; i < vect->size; i++) {
		vect->funcs.print(i, vect->data[i]);
	}
}
