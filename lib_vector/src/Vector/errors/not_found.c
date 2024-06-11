/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   not_found.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 09:56:32 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 09:57:23 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"
#include <unistd.h>

void not_found( void ) {
	int a;
	a = write(STDERR_FILENO, "Error: not found\n", 17);
	(void)a;
}
