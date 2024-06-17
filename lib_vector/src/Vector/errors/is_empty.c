/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_empty.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 08:28:33 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/17 08:03:21 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"

void is_empty( void ) {
	int a;
	a = write(STDERR_FILENO, "Error: vector is empty\n", 23);
	if (a == -1)
		perror("Write Failed\n");
}
