/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 23:11:11 by pollivie          #+#    #+#             */
/*   Updated: 2024/06/10 23:11:34 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct node_t
{
	struct node_t *next;
	uintptr_t      data;

} Node;

Node    *nodeCreate(uintptr_t data);
Node    *nodeGetNchild(Node *self, uint64_t n);
void     nodeInsertChild(Node *self, Node *child);
Node    *nodeRemoveChild(Node *self);
uint64_t nodeCountChild(Node *self);
Node    *nodeNext(Node *self);
Node    *nodeDestroy(Node *self);

typedef struct list_t
{
	Node    *head;
	Node    *tail;
	uint64_t size;

} List;

List     *listCreate(void);
void      listInsertFront(List *self, Node *new_head);
void      listInsertAt(List *self, Node *node, uint64_t index);
void      listInsertBack(List *self, Node *new_tail);
bool      listIsEmpty(List *self);
uint64_t  listSize(List *self);
Node     *listRemoveFront(List *self);
Node     *listRemoveBack(List *self);
Node     *listRemoveAt(List *self, uint64_t index);
void      listPushFront(List *self, uintptr_t data);
void      listPushBack(List *self, uintptr_t data);
void      listPushAt(List *self, uintptr_t data, uint64_t index);
uintptr_t listPopFront(List *self);
uintptr_t listPopBack(List *self);
uintptr_t listPopAt(List *self, uint64_t index);
void      listSort(Node **list, int (*f)(uintptr_t d1, uintptr_t d2));
List     *listDestroy(List *self);

#endif
