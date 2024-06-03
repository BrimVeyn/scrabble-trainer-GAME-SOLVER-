/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   it_add.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:06:46 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 17:18:25 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Iterator/iterator_define.h"

void	it_add(Iterator *it, void *value) {
	if (IT_EQ(*it, it_end(it->vector))) return ;
	it->vector->funcs.add(&it->vector->data[it->index], value);
}
