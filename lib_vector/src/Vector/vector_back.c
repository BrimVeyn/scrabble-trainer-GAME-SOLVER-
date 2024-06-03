/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_back.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 08:37:44 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 17:10:11 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void *vector_back(Vector *vect) {
	if (vect->size) return vect->data[vect->size - 1];
	if (SILENT == 0)
		is_empty();
	return NULL;
}
