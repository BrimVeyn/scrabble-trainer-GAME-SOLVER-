/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   it_begin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:23:21 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 15:23:34 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Iterator/iterator_define.h"

Iterator	it_begin(Vector *vect) {
	return (Iterator) {vect, 0};
}
