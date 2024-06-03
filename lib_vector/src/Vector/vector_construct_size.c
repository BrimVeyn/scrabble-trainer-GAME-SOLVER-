/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_construct_size.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:16:18 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 15:18:08 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/struct.h"

Vector vector_construct_size(size_t size, struct s_util_fun func) {
	Vector vect;

	vect.data = (void **) calloc(size + VECTOR_NULL, sizeof(void *));
	vect.size = 0;
	vect.capacity = size;
	vect.funcs = func;

	return vect;
}
