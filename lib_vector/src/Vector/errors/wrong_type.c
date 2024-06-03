/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrong_type.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 14:35:11 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 14:36:47 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Vector/vector_define.h"
#include <unistd.h>

void wrong_type( void ) {
	write(STDERR_FILENO, "Error: trying to get a wrong type\n", 34);
}
