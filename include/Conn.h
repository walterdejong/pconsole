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
	Conn.h	WJ101
*/

#ifndef CONN_H_WJ101
#define CONN_H_WJ101	1

#include "config.h"
#include "List.h"

#ifdef HAVE_ST_RDEV
#include <sys/types.h>
#endif

typedef struct Conn_tag Conn;

struct Conn_tag {
	Conn *prev, *next;

	int fd;				/* file descriptor */
#ifdef HAVE_ST_RDEV
	dev_t rdev;			/* used for checking for duplicate connections */
#endif
	char *dev;			/* ttyname */
	char *hostname;		/* hostname */
};

extern Conn *AllConns;

Conn *new_Conn(void);
void destroy_Conn(Conn *);
Conn *prepend_Conn(Conn *);
Conn *add_Conn(Conn *);
void remove_Conn(Conn *);

Conn *find_Conn_by_fd(int);
Conn *find_Conn_by_dev(char *);
Conn *find_Conn_by_hostname(char *);

#ifdef HAVE_ST_RDEV
Conn *find_Conn_by_rdev(dev_t);
#endif

#endif	/* CONN_H_WJ101 */

/* EOB */
