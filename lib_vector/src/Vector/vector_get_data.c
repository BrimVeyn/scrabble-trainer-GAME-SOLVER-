/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_get_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:27:27 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 14:42:43 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

int **vector_get_data_to_int(Vector *vect) {
	if (vect->funcs.type != 1) return wrong_type(), (int **) {NULL};
		return ((int **)vect->data);
}

char **vector_get_data_to_str(Vector *vect) {
	if (vect->funcs.type != 2) return wrong_type(), (char **) {NULL};
	return ((char **)vect->data);
}

void **vector_get_data(Vector *vect) {
	return (vect->data);
}
