/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_construct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:03:04 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 15:17:28 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

Vector vector_construct(struct s_util_fun func) {
	Vector vect;
	
	vect.data = (void **) calloc(VECTOR_DEFAULT_CAPACITY + VECTOR_NULL, sizeof(void *));
	vect.size = VECTOR_DEFAULT_SIZE;
	vect.capacity = VECTOR_DEFAULT_CAPACITY;
	vect.funcs = func;

	return vect;
}
