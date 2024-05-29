/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:40:50 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/29 10:20:00 by nbardavi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

enum {
	dWord,
	dLetter,
	tWord,
	tLetter,
};

typedef struct word {
	char		*word;
	int			score;
	struct word	*next;
} word;

typedef struct grid {
	int			grid[15][15];
	int			modifier[15][15];
} grid;

typedef struct letter{
	void* a;
	void* b;
	void* c;
	void* d;
	void* e;
	void* f;
	void* g;
	void* h;
	void* i;
	void* j;
	void* k;
	void* l;
	void* m;
	void* n;
	void* o;
	void* p;
	void* q;
	void* r;
	void* s;
	void* t;
	void* u;
	void* v;
	void* w;
	void* x;
	void* y;
	void* z;
	void* last;
	int valid;
} letter;

#endif
