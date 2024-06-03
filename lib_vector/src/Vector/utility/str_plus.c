/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_plus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:27:56 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 15:17:08 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"

void str_plus(void **ptr1, void *ptr2) {
    char *s1 = *(char **)ptr1, *s2 = (char *)ptr2;
	size_t l1 = strlen(s1), l2 = strlen(s2);
    char *new = malloc((l1 + l2 + 1) * sizeof(char));

	memcpy(new, s1, l1 );
	memcpy(new + l1, s2, l2 + 1);

	free(*ptr1);

    *ptr1 = new;
}
