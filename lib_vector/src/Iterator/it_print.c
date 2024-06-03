/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   it_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:25:56 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 17:19:23 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Iterator/iterator_define.h"
#include "../../include/Vector/vector_define.h"

void	it_print(Iterator *it) {
	if (IT_EQ(*it, it_end(it->vector))) {
		if (SILENT == 0)
			return not_found();
		else
			return ;
    }
	it->vector->funcs.print(it->index, it->vector->data[it->index]);
}
