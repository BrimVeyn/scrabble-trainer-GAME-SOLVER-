/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 08:33:06 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 15:17:08 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"

void str_print(size_t index, const void *value) {
	printf("vect[%zu] = %s\n", index, (char *)value);
}
