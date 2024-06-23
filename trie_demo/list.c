/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 23:18:37 by pollivie          #+#    #+#             */
/*   Updated: 2024/06/23 15:44:21 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

List *listCreate(void)
{
	List *list;

	list = (List *) malloc(sizeof(List));
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

List *listDestroy(List *self)
{
	Node *node;

	if (!self)
		return (NULL);
	while (!listIsEmpty(self))
	{
		node = listRemoveFront(self);
		nodeDestroy(node);
	}
	free(self);
	return (NULL);
}

void listInsertFront(List *self, Node *new_head)
{
	if (listIsEmpty(self))
	{
		self->head = new_head;
		self->tail = new_head;
	}
	else if (self->size == 1)
	{
		self->head = new_head;
		new_head->next = self->tail;
	}
	else
	{
		new_head->next = self->head;
		self->head = new_head;
	}
	self->size += 1;
}

void listInsertBack(List *self, Node *new_tail)
{
	if (listIsEmpty(self))
	{
		self->head = new_tail;
		self->tail = new_tail;
	}
	else if (self->size == 1)
	{
		self->head->next = new_tail;
		self->tail = new_tail;
	}
	else
	{
		self->tail->next = new_tail;
		self->tail = new_tail;
	}
	self->size += 1;
}

void listInsertAt(List *self, Node *node, uint64_t index)
{
	Node *temp;

	if (index == 0 || listIsEmpty(self))
		listInsertFront(self, node);
	else if (index >= self->size)
		listInsertBack(self, node);
	else
	{
		temp = nodeGetNchild(self->head, index - 1);
		nodeInsertChild(temp, node);
		self->size += 1;
	}
}

bool listIsEmpty(List *self)
{
	return (self->size == 0 || (!self->head && !self->tail));
}

uintptr_t listPopFront(List *self)
{
	uintptr_t data;
	Node     *node;

	node = listRemoveFront(self);
	if (!node)
		return (0);
	data = node->data;
	nodeDestroy(node);
	return (data);
}

uintptr_t listPopBack(List *self)
{
	uintptr_t data;
	Node     *node;

	node = listRemoveBack(self);
	if (!node)
		return (0);
	data = node->data;
	nodeDestroy(node);
	return (data);
}

uintptr_t listPopAt(List *self, uint64_t index)
{
	uintptr_t data;
	Node     *node;

	node = listRemoveAt(self, index);
	if (!node)
		return (0);
	data = node->data;
	nodeDestroy(node);
	return (data);
}

void listPushFront(List *self, uintptr_t data)
{
	Node *node;

	node = nodeCreate(data);
	listInsertFront(self, node);
}

void listPushBack(List *self, uintptr_t data)
{
	Node *node;

	node = nodeCreate(data);
	listInsertBack(self, node);
}

void listPushAt(List *self, uintptr_t data, uint64_t index)
{
	Node *node;

	node = nodeCreate(data);
	listInsertAt(self, node, index);
}

Node *listRemoveFront(List *list)
{
	Node *node;

	node = NULL;
	if (listIsEmpty(list))
		return (NULL);
	else if (list->size == 1)
	{
		node = list->head;
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		node = list->head;
		list->head = node->next;
		node->next = NULL;
	}
	list->size -= 1;
	return (node);
}

Node *listRemoveBack(List *list)
{
	Node *new_tail;
	Node *node;

	node = NULL;
	if (listIsEmpty(list))
		return (NULL);
	else if (list->size == 1)
	{
		node = list->head;
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		new_tail = nodeGetNchild(list->head, listSize(list) - 1);
		node = nodeRemoveChild(new_tail);
		list->tail = new_tail;
	}
	list->size -= 1;
	return (node);
}

Node *listRemoveAt(List *list, uint64_t index)
{
	Node *parent;
	Node *child;

	if (listIsEmpty(list))
		return (NULL);
	if (index == 0)
		return (listRemoveFront(list));
	if (index >= listSize(list))
		return (listRemoveBack(list));
	else
	{
		parent = nodeGetNchild(list->head, index - 1);
		child = nodeRemoveChild(parent);
		list->size -= 1;
	}
	return (child);
}

uint64_t listSize(List *self)
{
	return (self->size);
}

Node *nodeCreate(uintptr_t data)
{
	Node *node;

	node = (Node *) malloc(sizeof(Node));
	if (!node)
		return (NULL);
	node->next = 0;
	node->data = data;
	return (node);
}

uint64_t nodeCountChild(Node *self)
{
	uint64_t count;
	Node    *child;

	if (!self || !self->next)
		return (0);
	count = 0;
	child = self;
	while (child)
	{
		child = child->next;
		++count;
	}
	return (count);
}

Node *nodeGetNchild(Node *self, uint64_t n)
{
	Node *temp;

	if (!self || n == 0)
		return (self);
	temp = self;
	while (n--)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (temp);
}

void nodeInsertChild(Node *self, Node *child)
{
	if (!self || !child)
		return;
	child->next = self->next;
	self->next = child->next;
}

Node *nodeRemoveChild(Node *self)
{
	Node *child;

	if (!self || !self->next)
		return (NULL);
	child = self->next;
	self->next = self->next;
	child->next = NULL;
	return (child);
}

Node *nodeDestroy(Node *self)
{
	if (self)
	{
		free(self);
	}
	return (NULL);
}
