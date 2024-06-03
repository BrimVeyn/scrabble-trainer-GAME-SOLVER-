/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_find.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:19:59 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 10:04:12 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"

Iterator vector_find(Vector *vect, void *value) {
	for (size_t i = 0; i < vect->size; i++) {
		if (vect->funcs.cmp(vect->data[i], value) == 0)
			return (Iterator) {vect, i};
	}
	return (Iterator) {vect, vect->size};
}

