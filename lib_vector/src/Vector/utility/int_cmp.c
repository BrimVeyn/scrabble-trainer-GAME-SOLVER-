/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_cmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:55:14 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/24 12:02:04 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"

int int_cmp(void *n1, void *n2) {
	if (*(int *) n1 == *(int *)n2)
		return 0;
	else if (*(int *) n1 < *(int *)n2)
		return -1;
	else
		return 1;
}
