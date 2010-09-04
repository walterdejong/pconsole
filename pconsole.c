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
	pconsole.c	WJ101
*/

#include "config.h"
#include "pconsole.h"
#include "commands.h"
#include "cstring.h"
#include "terminal.h"
#include "Conn.h"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#define KEY_CTRL(x)		((x) - 'A' + 1)

int flags = FLAGS_ECHO;

void command_mode(void) {
char buf[256], *p;

	terminal_mode(TERMINAL_COOKED);
	printf("\n"
		"pconsole command mode\n"
		">>> ");
	fflush(stdout);

	flags |= FLAGS_CMD_MODE;

	while((p = fgets(buf, 256, stdin)) != NULL) {
		cstrip_line(buf);
		if (*buf)
			pcommand(buf);

		if (flags & FLAGS_CMD_MODE) {
			if (*buf)
				printf("\n");
			printf(">>> ");
			fflush(stdout);
		} else
			break;
	}
	if (p == NULL) {
		if (AllConns == NULL) {
			printf("\n\n");
			terminal_mode(TERMINAL_COOKED);
			exit(0);
		} else
			printf("<Ctrl-D>\n");
	}
	flags &= ~FLAGS_CMD_MODE;

	printf("\n"
		"Press <Ctlr-A> for command mode\n"
		"> ");
	fflush(stdout);
	terminal_mode(TERMINAL_RAW);
}

void pconsole(void) {
int key, typed = 0;
char kar;
Conn *c, *c_next;

	if (AllConns == NULL)
		command_mode();										/* start in command mode */
	else {
		printf("\n"
			"Press <Ctlr-A> for command mode\n"
			"> ");
		fflush(stdout);
		terminal_mode(TERMINAL_RAW);
	}
	while(read(fileno(stdin), &kar, 1) > 0) {
		key = kar & 0xff;
		if (key == 1) {										/* Ctrl-A => command mode */
			printf("<Ctrl-A>\n");
			command_mode();
			continue;
		}
		if (key == KEY_CTRL('S')) {							/* Ctrl-S => toggle echo */
			printf("<Ctrl-S> ");
			cmd_echo(NULL);

			printf("> ");
			fflush(stdout);
			typed = 0;
			continue;
		}
		if (key == '\r' || key == '\n') {					/* return */
			printf("\n> ");
			fflush(stdout);
			typed = 0;
		} else {
			if (flags & FLAGS_ECHO) {
				if (key == 0x7f || key == '\b') {			/* backspace */
					if (typed) {
						printf("\b \b");
						fflush(stdout);
						typed--;
					}
				} else {
					if (key >= ' ' && key <= '~') {
						fputc(key, stdout);
						typed++;
					} else {
						switch(key) {
							case 0x1b:
								printf("<Esc>");
								break;

							case '\t':
								printf("<Tab>");
								break;

							default:
								printf("<Ctrl-%c>", key - 1 + 'A');
						}
						printf("\n> ");
						typed = 0;
					}
					fflush(stdout);
				}
			}
		}
/*
	put character in everyones input buffer
*/
		for(c = AllConns; c != NULL; c = c_next) {
			c_next = c->next;

			if (c->fd > 0) {
				seteuid(0);											/* regain root privs */
				if (ioctl(c->fd, TIOCSTI, &kar) == -1) {			/* simulate terminal input */
					seteuid(getuid());								/* drop root privs again */
					printf("\nioctl() : %s\n", strerror(errno));
					if (c->hostname != NULL)
						printf("detaching from %s#%s\n", c->hostname, c->dev);
					else
						printf("detaching from %s\n", c->dev);
					remove_Conn(c);
					destroy_Conn(c);
				} else
					seteuid(getuid());								/* drop the root privs */
			}
		}
		if (AllConns == NULL)
			command_mode();
	}
}

RETSIGTYPE sig_exit(int sig) {
	cmd_exit(NULL);
	exit(127 + sig);				/* never reached... but hey */
}

int main(int argc, char **argv) {
	if (geteuid()) {
		fprintf(stderr, "You must be root to run this program or this program should be setuid root.\n");
		return -1;
	}
	if (seteuid(getuid())) {				/* drop root privs */
		fprintf(stderr, "failed to drop root privileges\n");
		exit(-1);
	}
	if (!isatty(fileno(stdin))) {
		fprintf(stderr, "pconsole: not a tty\n");
		return -1;
	}
	printf("pconsole WJ101\n");

	signal(SIGTERM, sig_exit);
	signal(SIGINT, sig_exit);

	if (argc > 1)
		cmd_attach(&argv[1]);

	pconsole();
	cmd_exit(NULL);
	return 0;
}

/* EOB */
