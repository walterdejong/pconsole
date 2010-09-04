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
	terminal.c	WJ101
*/

#include "config.h"
#include "terminal.h"
#include "cstring.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <sys/types.h>

void terminal_mode(int mode) {
static struct termios ori_term;
static int saved = 0;
struct termios term;

	if (!isatty(fileno(stdin))) {
		fprintf(stderr, "pconsole: not on a tty\n");
		return;
	}
	tcgetattr(fileno(stdin), &term);
	if (mode == TERMINAL_RAW) {
		if (!saved) {
			memcpy(&ori_term, &term, sizeof(struct termios));
			saved = 1;
		}
		term.c_iflag |= IGNBRK;
		term.c_iflag &= ~(INLCR | ICRNL | IXON | IXOFF);
		term.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);
		term.c_cc[VMIN] = 1;
		term.c_cc[VTIME] = 0;
		tcsetattr(fileno(stdin), TCSANOW, &term);
	} else {
		if (saved) {
			memcpy(&term, &ori_term, sizeof(struct termios));
			tcsetattr(fileno(stdin), TCSANOW, &term);
		}
	}
}

/* EOB */
