/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   it_erase.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:27:52 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 17:21:42 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Iterator/iterator_define.h"
#include "../../include/Vector/vector_define.h"

void it_erase(Iterator *it) {
	if (IT_EQ(*it, it_end(it->vector))) return ;
	vector_erase_index(it->vector ,it->index);
	*it = it_prev(it);
}
