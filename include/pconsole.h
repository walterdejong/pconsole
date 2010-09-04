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
	pconsole.h	WJ101
*/

#ifndef PCONSOLE_H_WJ101
#define PCONSOLE_H_WJ101	1

#define VERSION			"1.0"

#define FLAGS_ECHO		1
#define FLAGS_CMD_MODE	2

extern int flags;

void command_mode(void);
void pconsole(void);
int main(int, char **);

#endif	/* PCONSOLE_H_WJ101 */

/* EOB */
