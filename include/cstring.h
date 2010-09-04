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
	cstring.h	WJ101
*/

#ifndef CSTRING_H_WJ101
#define CSTRING_H_WJ101	1

#include "config.h"

#include <string.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

#ifndef HAVE_STRCHR
#define strchr(x,y)		index((x),(y))
#endif

#ifndef HAVE_MEMSET
#include <types.h>
void *memset(void *, int, size_t);
#endif

#ifndef HAVE_STRDUP
char *strdup(char *);
#endif

#ifndef HAVE_STRERROR
char *strerror(int);
#endif

void cstrip_line(char *);
char **cstrsplit(char *, char);

#endif	/* CSTRING_H_WJ101 */

/* EOB */
