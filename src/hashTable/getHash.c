/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getHash.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:40:23 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/28 17:02:37 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashTableDefine.h"

int getHash(char* s) {
	int n = strlen(s);
    long long p = 31, m = 1e9 + 7;
    long long hash = 0;
    long long p_pow = 1;
    for(int i = 0; i < n; i++) {
        hash = (hash + (s[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
	if (hash < 0)
		hash = -hash;
	return hash % TABLE_SIZE;
}
