/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashTableAdd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:40:49 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/28 17:02:37 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashTableDefine.h"

void hashTableAdd(word **hashTable, word *word, size_t index) {
	if (hashTable[index] == NULL) {
		hashTable[index] = word;
	}
	else {
		struct word *current = hashTable[index];
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = word;
	}
}
