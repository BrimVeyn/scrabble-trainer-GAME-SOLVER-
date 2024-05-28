/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashTableFind.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:37:16 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/28 17:02:37 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashTableDefine.h"

int hashTableFind(word **hashTable, char *to_find) {
	int index = getHash(to_find);
	if (hashTable[index] != NULL) {
		struct word * current = hashTable[index];
		while (current != NULL) {
			if (strcmp(current->word, to_find) == 0)
				return index;
			current = current->next;
		}
	}
	return -1;
}
