/*
    pconsole WJ101
    Copyright (C) 2001  Walter de Jong <walter@heiho.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
/*
	List.c	WJ98
*/

#include "config.h"
#include "List.h"

#include <stdlib.h>

#ifndef NULL
#define NULL	0
#endif

void *prepend_List(void *v1, void *v2) {
ListType **root, *l;

	if (v1 == NULL)
		return NULL;

	root = (ListType **)v1;
	l = (ListType *)v2;

	if (l == NULL)
		return (void *)*root;

	l->prev = NULL;
	l->next = *root;
	if (l->next != NULL)
		l->next->prev = l;
	*root = l;
	return l;
}

void *add_List(void *v1, void *v2) {
ListType **root, *l;

	if (v1 == NULL)
		return NULL;

	root = (ListType **)v1;
	l = (ListType *)v2;

	if (l == NULL)
		return (void *)*root;

	l->prev = l->next = NULL;
	if (*root == NULL)
		*root = l;
	else {
		ListType *lp;

/* Link in at the end of the list */

		for(lp = *root; lp->next != NULL; lp = lp->next);
		lp->next = l;
		l->prev = lp;
	}
	return (void *)l;
}

/*
	Note: listdestroy_List() now auto-rewinds the list (!)
*/
void listdestroy_List(void *v1, void *v2) {
ListType *l, *l2;
void (*destroy_func)(ListType *);

	if (v1 == NULL || v2 == NULL)
		return;

	l = (ListType *)v1;
	destroy_func = (void (*)(ListType *))v2;

	while(l->prev != NULL)			/* auto-rewind */
		l = l->prev;

	while(l != NULL) {
		l2 = l->next;
		destroy_func(l);
		l = l2;
	}
}

void *concat_List(void *v1, void *v2) {
ListType **root, *l;

	if (v1 == NULL)
		return NULL;

	root = (ListType **)v1;
	l = (ListType *)v2;

	if (l == NULL)
		return (void *)*root;

	if (*root == NULL)
		*root = l;
	else {
		ListType *p;

		for(p = *root; p->next != NULL; p = p->next);
		p->next = l;
		l->prev = p;
	}
	return (void *)l;
}

void remove_List(void *v1, void *v2) {
ListType **root, *l;

	if (v1 == NULL || v2 == NULL)
		return;

	root = (ListType **)v1;
	l = (ListType *)v2;

	if (*root == NULL)
		return;

	if (l->prev == NULL)				/* must be in root node */
		*root = l->next;
	else
		l->prev->next = l->next;

	if (l->next != NULL)
		l->next->prev = l->prev;
	l->next = l->prev = NULL;
}

int list_Count(void *v) {
ListType *l;
int c = 0;

	for(l = (ListType *)v; l != NULL; l = l->next)
		c++;
	return c;
}

void *rewind_List(void *v) {
ListType *root;

	if (v == NULL)
		return NULL;

	root = (ListType *)v;

	while(root->prev != NULL)
		root = root->prev;
	return root;		
}

void *unwind_List(void *v) {
ListType *root;

	if (v == NULL)
		return NULL;

	root = (ListType *)v;

	while(root->next != NULL)
		root = root->next;
	return root;		
}

/*
	sort a list using qsort()
*/
void *sort_List(void *v, int (*sort_func)(void *, void *)) {
ListType *root, *p, **arr;
int count, i;

	if (v == NULL)
		return NULL;

	root = (ListType *)v;
	if (sort_func == NULL)
		return root;

	count = 0;
	for(p = root; p != NULL; p = p->next)		/* count # of elements */
		count++;

	if (count <= 1)								/* nothing to be sorted */
		return root;

	if ((arr = (ListType **)malloc(count * sizeof(ListType *))) == NULL)
		return root;							/* malloc() failed, return root */

/* fill the array of pointers */
	p = root;
	for(i = 0; i < count; i++) {
		arr[i] = p;
		p = p->next;
	}
/* do the sorting (we include a nasty typecast here for convenience) */
	qsort(arr, count, sizeof(ListType *), (int (*)(const void *, const void *))sort_func);

/* rearrange the prev and next pointers */
	arr[0]->prev = NULL;
	arr[0]->next = arr[1];

	for(i = 1; i < count-1; i++) {
		arr[i]->prev = arr[i-1];
		arr[i]->next = arr[i+1];
	}
	arr[i]->prev = arr[i-1];
	arr[i]->next = NULL;

/* free the array and return new root */
	root = arr[0];
	free(arr);
	return root;
}

/* EOB */
