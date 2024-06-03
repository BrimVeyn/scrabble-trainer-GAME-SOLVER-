/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_front.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 08:37:10 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 17:10:22 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void *vector_front(Vector *vect) {
	if (vect->size) return vect->data[0];
	if (SILENT == 0)
		is_empty();
	return NULL;
}
