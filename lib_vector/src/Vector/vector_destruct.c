/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_destruct.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:02:10 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 16:21:38 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Vector/vector_define.h"

void	vector_destruct(Vector *vect) {
	for (size_t i = 0; i < vect->size; i++) { free(vect->data[i]); }
	free(vect->data);
}
