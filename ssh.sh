#! /bin/sh
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
#	pconsole ssh.sh	WJ101
#
#	This is a wrapper script for ssh
#	Without it, pconsole can't work correctly with ssh
#

# list all possible paths where ssh could be
PATH=/bin:/usr/bin:/sbin:/usr/sbin:/usr/local/bin:/usr/local/ssh:/usr/local/ssh/bin:/usr/ssh:/usr/ssh/bin:/etc:/usr/bsd:/etc/ssh:/etc/ssh/bin:/usr/share/bin:/usr/share/ssh:/opt/bin:/opt/ssh:/opt/ssh/bin:/usr/etc

SSH=`type ssh 2>/dev/null`
if [ -z "${SSH}" ]
then
	telnet $*
else
	ssh $*
fi

# EOB
