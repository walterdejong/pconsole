# Generated automatically from Makefile.in by configure.
#
#   pconsole WJ101
#   Copyright (C) 2001  Walter de Jong <walter@heiho.net>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#	pconsole Makefile.in	WJ101
#

prefix = /home/walter/.local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin

CC = gcc
CFLAGS = -O2 -Wall -Wstrict-prototypes -fomit-frame-pointer
LFLAGS =
LIBS =
INCLUDE = include

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = /usr/bin/install -c -m 0750
INSTALL_SCRIPT = /usr/bin/install -c -m 0750
INSTALL_DATA = /usr/bin/install -c -m 0640

.c.o:
	$(CC) -I$(INCLUDE) -I. $(CFLAGS) -c $<

OBJS = List.o Conn.o cstring.o terminal.o commands.o pconsole.o

CFILES = $(OBJS:.o=.c)

TARGETS = pconsole

all: config.status config.log $(TARGETS)

include .depend

#
#   Targets
#

config.status config.log:
	@( \
		echo "Not yet configured. Type './configure' to configure." ; \
		echo \
	) ; \
	exit 1

pconsole: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o pconsole $(LIBS)


neat:
	rm -f core *~

clean:
	rm -f core *.o $(TARGETS) *~

mrproper: clean
	rm -f .depend config.status config.cache config.log config.h pconsole.sh
	touch .depend

depend dep .depend:
	$(CC) -I$(INCLUDE) -I. -M $(CFILES) > .depend


install: all
	@( \
		echo "installing pconsole ..."; \
		mkdir -p $(bindir) 2>/dev/null ; \
		${INSTALL} pconsole $(bindir) ;		\
		${INSTALL_PROGRAM} pconsole.sh $(bindir) ;	\
		${INSTALL_PROGRAM} ssh.sh $(bindir) ;			\
		echo ;	\
		echo "Note: You may want to make pconsole setuid root with the following commands:";	\
		echo ;	\
		echo "   sudo chown root $(bindir)/pconsole && sudo chmod 4110 $(bindir)/pconsole" ; \
		echo \
	)

uninstall:
	rm -f $(bindir)/pconsole $(bindir)/pconsole.sh $(bindir)/ssh.sh ; \
	rmdir $(bindir) 2>/dev/null

# EOB
