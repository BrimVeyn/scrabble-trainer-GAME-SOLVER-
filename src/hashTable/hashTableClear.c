/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashTableClear.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:38:45 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/28 17:02:37 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashTableDefine.h"

void hashTableClear(word **hashTable) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		if (hashTable[i] != NULL) {
			struct word * current = hashTable[i];
			struct word * tmp;
			while (current != NULL) {
				tmp = current->next;
				free(current->word);
				free(current);
				current = tmp;
			}
		}
	}
}
