/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_swap.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 17:23:57 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/24 17:24:40 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void vector_swap(Vector *vect, size_t i1, size_t i2) {
	void *tmp = vect->data[i1];
	vect->data[i1] = vect->data[i2];
	vect->data[i2] = tmp;
}
