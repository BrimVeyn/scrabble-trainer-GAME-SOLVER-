/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   it_prev.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:27:10 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 15:27:19 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Iterator/iterator_define.h"

Iterator it_prev(Iterator *it) {
	return (Iterator) {it->vector, it->index - 1};
}
