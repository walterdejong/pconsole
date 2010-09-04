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
	Conn.c	WJ101
*/

#include "config.h"
#include "Conn.h"
#include "cstring.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Conn *AllConns = NULL;

Conn *new_Conn(void) {
Conn *c;

	if ((c = (Conn *)malloc(sizeof(Conn))) == NULL)
		return NULL;
	memset(c, 0, sizeof(Conn));
	return c;
}

void destroy_Conn(Conn *c) {
	if (c == NULL)
		return;

	if (c->fd > 0)
		close(c->fd);
	if (c->dev != NULL)
		free(c->dev);
	if (c->hostname != NULL)
		free(c->hostname);
	free(c);
}

Conn *prepend_Conn(Conn *c) { return (Conn *)prepend_List(&AllConns, c); }
Conn *add_Conn(Conn *c) { return (Conn *)add_List(&AllConns, c); }
void remove_Conn(Conn *c) { remove_List(&AllConns, c); }

Conn *find_Conn_by_fd(int fd) {
Conn *c;

	for(c = AllConns; c != NULL; c = c->next)
		if (c->fd == fd)
			return c;
	return NULL;
}

Conn *find_Conn_by_dev(char *name) {
Conn *c;

	if (name == NULL)
		return NULL;

	for(c = AllConns; c != NULL; c = c->next)
		if (c->dev != NULL && !strcmp(c->dev, name))
			return c;
	return NULL;
}

Conn *find_Conn_by_hostname(char *name) {
Conn *c;

	if (name == NULL)
		return NULL;

	for(c = AllConns; c != NULL; c = c->next)
		if (c->hostname != NULL && !strcmp(c->hostname, name))
			return c;
	return NULL;
}

#ifdef HAVE_ST_RDEV
Conn *find_Conn_by_rdev(dev_t rdev) {
Conn *c;

	for(c = AllConns; c != NULL; c = c->next)
		if (c->rdev == rdev)
			return c;
	return NULL;
}
#endif

/* EOB */
