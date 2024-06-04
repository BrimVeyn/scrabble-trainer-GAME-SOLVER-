/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 12:32:16 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/06/04 12:32:34 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashTableDefine.h"

static int	calculate_size(int n)
{
	size_t	size;

	if (n <= 0)
		size = 1;
	else
		size = 0;
	while (n != 0)
	{
		size++;
		n /= 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char			*str;
	size_t			size;
	unsigned int	unb;

	size = calculate_size(n);
	str = malloc((size + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[size] = '\0';
	unb = (unsigned int)n;
	if (n < 0)
	{
		str[0] = '-';
		unb = -((unsigned int)n);
	}
	else if (n == 0)
		str[0] = '0';
	while (unb != 0)
	{
		str[--size] = (unb % 10) + '0';
		unb = unb / 10;
	}
	return (str);
}
