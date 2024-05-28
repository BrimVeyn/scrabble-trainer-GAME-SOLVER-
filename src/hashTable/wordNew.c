/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wordNew.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 16:39:27 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/28 17:02:37 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashTableDefine.h"

word *wordNew(char *word, int score) {
	struct word *ptr = malloc(sizeof(struct word));

	ptr->word = strdup(word);
	ptr->score = score;
	ptr->next = NULL;
	return ptr;
}
