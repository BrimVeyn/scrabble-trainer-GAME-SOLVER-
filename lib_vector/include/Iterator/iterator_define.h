/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_define.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 15:09:49 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/23 16:25:09 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../struct.h"

void	it_add(Iterator *it, void *value);
void	it_pp(Iterator *it);
void	it_mm(Iterator *it);
void	it_print(Iterator *it);
void	it_erase(Iterator *it);

Iterator	it_prev(Iterator *it);
Iterator	it_next(Iterator *it);
Iterator	it_begin(Vector *vect);
Iterator	it_end(Vector *vect);
