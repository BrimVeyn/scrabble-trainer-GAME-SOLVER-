/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_jump_search.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 10:07:22 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/27 10:18:03 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/Vector/vector_define.h"
#include <math.h>

static int min(int a, int b){
	if(b>a)
		return a;
	else
		return b;
}

static size_t vector_jump_search_algo(Vector *vect, void *value) {
    int step = sqrt(vect->size);
 
    // Finding the block where element is
    // present (if it is present)
    int prev = 0;
    while (vect->funcs.cmp(vect->data[min(step, vect->size) - 1], value) < 0)
    {
        prev = step;
        step += sqrt(vect->size);
        if (prev >= (int)vect->size)
            return vect->size;
    }
 
    // Doing a linear search for x in block
    // beginning with prev.
    while (vect->funcs.cmp(vect->data[prev], value) < 0)
    {
        prev++;
 
        // If we reached next block or end of
        // array, element is not present.
        if (prev == min(step, vect->size))
            return vect->size;
    }
    // If element is found
    if (vect->funcs.cmp(vect->data[prev], value) == 0)
        return prev;
 
    return vect->size;
}

Iterator vector_jump_search(Vector *vect, void *value) {
	size_t index = vector_jump_search_algo(vect, value);
	return (Iterator) {vect, index};
}
