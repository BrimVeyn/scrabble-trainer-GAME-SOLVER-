/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_clear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 16:21:49 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 17:05:43 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void vector_clear(Vector *vect) {
	for (size_t i = vect->size - 1; i >= 0 && i < vect->size; i--) {
		vector_erase_index(vect, i);
	}
	vect->size = 0;
}
