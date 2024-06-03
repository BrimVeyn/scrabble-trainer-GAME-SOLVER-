/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   it_next.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:27:32 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 16:00:46 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Iterator/iterator_define.h"

Iterator it_next(Iterator *it) {
	return (Iterator) {it->vector, it->index + 1};
}
