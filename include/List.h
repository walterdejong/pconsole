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
	List.h	WJ99
*/

#ifndef _LIST_H_WJ99
#define _LIST_H_WJ99 1

#define List(x)		x *prev, *next

typedef struct List_tag ListType;

struct List_tag {
	List(ListType);
};

void *prepend_List(void *, void *);
void *add_List(void *, void *);
void listdestroy_List(void *, void *);
void *concat_List(void *, void *);
void remove_List(void *, void *);
int list_Count(void *);
void *rewind_List(void *);
void *unwind_List(void *);
void *sort_List(void *, int (*)(void *, void *));

#endif	/* _LIST_H_WJ99 */

/* EOB */
