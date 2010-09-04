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
	cstring.c	WJ101
*/

#include "config.h"
#include "cstring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HAVE_MEMSET
void *memset(void *s, int c, size_t n) {
	while(n > 0) {
		*((char *)s) = (char)c;
		n--;
	}
	return s;
}
#endif

#ifndef HAVE_STRDUP
char *strdup(char *s) {
char *str;
int len;

	if (s == NULL)
		return NULL;

	if ((str = (char *)malloc(strlen(s)+1)) == NULL)
		return NULL;
	strcpy(str, s);
	return str;
}
#endif

#ifndef HAVE_STRERROR
char *strerror(int err) {
static char buf[64];

	sprintf(buf, "error #%d\n", err);
	return buf;
}
#endif


void cstrip_line(char *msg) {
char *p;
int i;

	if (msg == NULL || !*msg)
		return;

	while((p = strchr(msg, 27)) != NULL)		/* filter escape characters */
		memmove(p, p+1, strlen(p)+1);
	while((p = strchr(msg, '\t')) != NULL)		/* tab2space */
		*p = ' ';

	i = strlen(msg);
	while(i && *msg == ' ')
		memmove(msg, msg+1, i--);

	i--;
	while(i >= 0 && (msg[i] == ' ' || msg[i] == '\r' || msg[i] == '\n'))
		msg[i--] = 0;
	return;
}

/*
	Splits a line into an array
	Just like Perl's split() function
	The substrings are not copied into the array; the string that is
	passed as parameter to split() is chopped in pieces

	The return value is allocated, it must be free()d
*/
char **cstrsplit(char *line, char token) {
char **args, *p, *startp;
int num = 2;

	if (!line || !*line)
		return NULL;

	p = line;
	while((p = strchr(p, token)) != NULL) {
		num++;
		p++;
	}
	if ((args = (char **)malloc(num * sizeof(char *))) == NULL)
		return NULL;

	startp = p = line;
	num = 0;
	while((p = strchr(p, token)) != NULL) {
		args[num++] = startp;
		*p = 0;
		p++;
		startp = p;
	}
	args[num++] = startp;
	args[num] = NULL;
	return args;
}

/* EOB */
