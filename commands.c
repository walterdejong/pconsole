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
	command.c	WJ101
*/

#include "config.h"
#include "commands.h"
#include "pconsole.h"
#include "cstring.h"
#include "terminal.h"
#include "Conn.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef HAVE_FCNTL_H
#include "fcntl.h"
#endif

Command commands[] = {
	{ "help",		cmd_help,		"Give help about the available commands",	},
	{ "?",			cmd_help,		"short-cut for 'help'",						},

	{ "version",	cmd_version,	"Display version information",				},
	{ "echo",		cmd_echo,		"Turn echo on or off",						},

	{ "attach",		cmd_attach,		"Attach to a tty device",					},
	{ "detach",		cmd_detach,		"Detach from a tty device",					},
	{ "list",		cmd_list,		"Show devices currently attached to",		},

	{ "connect",	cmd_connect,	"Leave command mode",						},
	{ "quit",		cmd_exit,		"Exit pconsole",							},
	{ "exit",		cmd_exit,		"Exit pconsole",							},

	{ NULL,			NULL,			NULL,										},
};

int pcommand(char *buf) {
char cmd[4096], **arr, *cmd_str;
int i, len;

	strcpy(cmd, buf);

	cstrip_line(cmd);
	if (!*cmd)
		return 0;

	if ((arr = cstrsplit(cmd, ' ')) == NULL) {
		fprintf(stderr, "pconsole: out of memory (?)\n");
		return 0;
	}
	len = strlen(arr[0]);

	for(i = 0; commands[i].cmd != NULL; i++) {
		cmd_str = commands[i].cmd;
		if (*cmd_str == '|')
			cmd_str++;

		if (!strncmp(cmd_str, arr[0], len)) {
			if (commands[i].func == NULL)
				printf("pconsole: not yet implemented\n");
			else
				commands[i].func(&arr[1]);
			break;
		}
	}
	if (commands[i].cmd == NULL)
		printf("pconsole: unknown command '%s'\n", arr[0]);

	free(arr);
	return 0;
}


int cmd_help(char **argv) {
int i;

	if (argv == NULL || argv[0] == NULL) {
		for(i = 0; commands[i].cmd != NULL; i++)
			if (commands[i].cmd[0] == '|')
				printf("\n %-14s %s\n", commands[i].cmd+1, commands[i].help);
			else
				printf(" %-14s %s\n", commands[i].cmd, commands[i].help);
	} else {
		int j, len;
		char *cmd;

		for(j = 0; argv[j] != NULL; j++) {
/*
	Special command 'help warranty' to satisfy the GNU people
	It is formatted at 60 chars because the pconsole.sh script opens the
	window by default at only 60 chars. The message is too long so it
	still scrolls off the screen, but oh well...
*/
			if (!strcmp(argv[j], "warranty")) {
				printf(
"Copyright (C) 2001  Walter de Jong <walter@heiho.net>\n"
"\n");
				printf(
"This program is free software; you can redistribute it\n"
"and/or modify it under the terms of the GNU General Public\n"
"License as published by the Free Software Foundation;\n"
"either version 2 of the License, or (at your option)\n"
"any later version.\n"
"\n");
				printf(
"This program is distributed in the hope that it will be\n"
"useful, but WITHOUT ANY WARRANTY; without even the\n"
"implied warranty of MERCHANTABILITY or FITNESS FOR A\n"
"PARTICULAR PURPOSE. See the GNU General Public License\n"
"for more details.\n"
"\n");
				printf(
"You should have received a copy of the GNU General Public\n"
"License along with this program; if not, write to the\n"
"Free Software Foundation, Inc., 59 Temple Place,\n"
"Suite 330, Boston, MA  02111-1307  USA\n");
				continue;
			}
/*
	give help on specified commands
*/
			len = strlen(argv[j]);
			for(i = 0; commands[i].cmd != NULL; i++) {
				cmd = commands[i].cmd;
				if (*cmd == '|')
					cmd++;

				if (!strncmp(argv[j], cmd, len)) {
					printf(" %-14s %s\n", cmd, commands[i].help);
					break;
				}
			}
			if (commands[i].cmd == NULL)
				printf("pconsole: unknown command '%s'\n", argv[j]);
		}
	}
	return 0;
}

int cmd_version(char **argv) {
	printf("pconsole " VERSION " WJ101\n"
		"Copyright (C) 2001  Walter de Jong <walter@heiho.net>\n"
		"\n"
		"This is free software with ABSOLUTELY NO WARRANTY.\n"
		"For details type 'help warranty'.\n"
		"\n"
		"The distribution page is at http://www.heiho.net/pconsole/\n"
	);
	return 0;
}

int cmd_echo(char **argv) {
	if (argv == NULL || argv[0] == NULL)
		flags ^= FLAGS_ECHO;
	else
		if (!strcmp(argv[0], "on"))
			flags |= FLAGS_ECHO;
		else
			if (!strcmp(argv[0], "off"))
				flags &= ~FLAGS_ECHO;
			else {
				printf("usage: echo [on|off]\n");
				return 1;
			}

	printf("pconsole: echo is now toggled %s\n", (flags & FLAGS_ECHO) ? "on" : "off");
	return 0;
}

int cmd_exit(char **argv) {
Conn *c, *c_next;
char *arr[2];

/*
	exit: detach from all attached terminals
	abuse the cmd_detach() command to do this
*/
	arr[1] = NULL;
	for(c = AllConns; c != NULL; c = c_next) {
		c_next = c->next;
		arr[0] = c->dev;
		cmd_detach(arr);
	}
	terminal_mode(TERMINAL_COOKED);
	exit(0);
	return -1;
}

int cmd_connect(char **argv) {
	if (AllConns == NULL) {
		printf("pconsole: currently not attached to any terminal device\n");
		return 0;
	}
	flags &= ~FLAGS_CMD_MODE;			/* drop out of command mode */
	return 0;
}

int cmd_attach(char **argv) {
int i, len;
char *tty, *devname, buf[256];
struct stat ttystat, ttystat2;
Conn *c;

	if (argv == NULL || argv[0] == NULL) {
		printf("usage: attach [hostname#]<tty device name> [...]\n");
		return 1;
	}
	if ((tty = ttyname(fileno(stdin))) == NULL) {
		printf("pconsole: not on a tty\n");
		cmd_exit(NULL);
		return -1;
	}
	if (stat(tty, &ttystat)) {
		printf("failed to stat my tty %s: %s\n", tty, strerror(errno));
		cmd_exit(NULL);
		return -1;
	}
	sprintf(buf, "\r\n[pconsole attached from tty %s]\r\n", tty);
	len = strlen(buf);

	for(i = 0; argv[i] != NULL; i++) {
		printf("attaching %s : ", argv[i]);

		if ((devname = strchr(argv[i], '#')) != NULL) {
			*devname = 0;
			devname++;
			if (!*devname) {
				printf("invalid terminal device name '%s:'\n", argv[i]);
				continue;
			}
		} else
			devname = argv[i];
/*
	do a couple of safety checks
	the given name should be a character device, and it should not be the same
	as the controlling tty
*/
		if (stat(devname, &ttystat2)) {
			printf("%s\n", strerror(errno));
			continue;
		}
		if (((ttystat2.st_mode & S_IFMT) & S_IFCHR) != S_IFCHR) {
			printf("not a character device\n");
			continue;
		}
#ifdef HAVE_ST_RDEV
		if (ttystat2.st_rdev == ttystat.st_rdev) {
			printf("cannot create a pconsole loop\n");
			continue;
		}
		if (find_Conn_by_rdev(ttystat2.st_rdev) != NULL) {
			printf("already attached\n");
			continue;
		}
#else
		if (find_Conn_by_name(devname) != NULL) {
			printf("already attached\n");
			continue;
		}
#endif
/*
	allocate connection object
*/
		if ((c = new_Conn()) == NULL) {
			printf("out of memory (?)\n");
			return -1;
		}
		c->dev = strdup(devname);
		if (devname != argv[i])
			c->hostname = strdup(argv[i]);
#ifdef HAVE_ST_RDEV
		c->rdev = ttystat2.st_rdev;
#endif
/*
	open the device
*/
		if ((c->fd = open(devname, O_RDWR)) == -1) {
			printf("%s\n", strerror(errno));
			destroy_Conn(c);
			continue;
		}
		add_Conn(c);
		printf("Ok\n");
		write(c->fd, buf, len);		/* tell target we're attached */
	}
	return 0;
}

int cmd_detach(char **argv) {
int i, len;
char *tty, buf[256];
Conn *c;

	if (argv == NULL || argv[0] == NULL) {
		printf("usage: detach [hostname#]<tty device name> [...]\n");
		return 1;
	}
	tty = ttyname(fileno(stdin));
	sprintf(buf, "\r\n[pconsole detached from tty %s]\r\n", (tty == NULL) ? "(unknown)" : tty);
	len = strlen(buf);

	for(i = 0; argv[i] != NULL; i++) {
		if ((c = find_Conn_by_dev(argv[i])) == NULL
			&& (c = find_Conn_by_hostname(argv[i])) == NULL) {
			printf("not attached to %s\n", argv[i]);
			continue;
		}
		do {
			if (c->hostname != NULL)
				printf("detaching from %s#%s : ", c->hostname, c->dev);
			else
				printf("detaching from %s : ", c->dev);

			if (c->fd > 0)
				write(c->fd, buf, len);

			remove_Conn(c);
			destroy_Conn(c);
			printf("Ok\n");
		} while((c = find_Conn_by_hostname(argv[i])) != NULL
			|| (c = find_Conn_by_dev(argv[i])) != NULL);
	}
	return 0;
}

int cmd_list(char **argv) {
Conn *c;

	if (AllConns == NULL) {
		printf("pconsole: currently not attached to any terminal device\n");
		return 0;
	}
	printf("Currently attached to:\n");
	for(c = AllConns; c != NULL; c = c->next) {
#ifdef HAVE_ST_RDEV
		if (c->hostname != NULL)
			printf(" %-16s %-20s (device no %d, %d)\n", c->dev, c->hostname, ((int)c->rdev >> 8) & 0xff, (int)c->rdev & 0xff);
		else
			printf(" %-16s %-20s (device no %d, %d)\n", c->dev, " ", ((int)c->rdev >> 8) & 0xff, (int)c->rdev & 0xff);
#else
		if (c->hostname != NULL)
			printf(" %-16s %s\n", c->dev, c->hostname);
		else
			printf(" %-16s\n", c->dev);
#endif
	}
	return 0;
}

/* EOB */
