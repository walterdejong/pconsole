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
	command.h	WJ101
*/

#ifndef COMMAND_H_WJ101
#define COMMAND_H_WJ101	1

typedef struct {
	char *cmd;
	int (*func)(char **);
	char *help;
} Command;

int pcommand(char *);

int cmd_help(char **);
int cmd_version(char **);
int cmd_echo(char **);
int cmd_connect(char **);
int cmd_exit(char **);
int cmd_attach(char **);
int cmd_detach(char **);
int cmd_list(char **);

#endif	/* COMMAND_H_WJ101 */

/* EOB */
