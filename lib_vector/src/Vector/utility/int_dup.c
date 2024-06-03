/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_dup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 08:30:36 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 15:17:09 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"

void *int_dup(const void *value) {
    int *ptr = malloc(1 * sizeof(int));
    *ptr = *(const int *)value;
    return (void *)ptr;
}
