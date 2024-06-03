/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   it_end.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:24:31 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 15:28:48 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Iterator/iterator_define.h"

Iterator	it_end(Vector *vect) {
	return (Iterator) {vect, vect->size};
}
