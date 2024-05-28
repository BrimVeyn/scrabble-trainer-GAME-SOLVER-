/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashTablePrint.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:41:36 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/28 17:02:37 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashTableDefine.h"

void hashTablePrint(word **hashTable) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		if (hashTable[i] != NULL) {
			struct word *current = hashTable[i];
			printf("-- %d\t", i);
			while (current != NULL) {
				printf("w: %-10s\ts: %d\t", current->word, current->score);
				current = current->next;
			}
			printf("\n");
		}
		else
			printf("-- %d\tNULL\n", i);
	}
}
