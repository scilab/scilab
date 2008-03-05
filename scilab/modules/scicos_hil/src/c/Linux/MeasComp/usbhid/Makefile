#############################################################################
#	
#	Makefile for building:
#
#		libmccusb.so:      Library for MCC USB series
#		test-usb1208LS:    Program to test -1208LS module
#		test-usb1024LS:    Program to test USB-1024LS module
#		test-usb1208FS:    Program to test USB-1208FS module
#		test-usb1608FS:    Program to test USB-1208FS module
#		test-usb1096HFS:   Program to test USB-1096HFS module
#		test-usb1016FS:    Program to test USB-1616FS module
#		test-usb-tc:       Program to test USB-TC module
#		test-usb-temp:     Program to test USB-TEMP module
#		test-minilab1008:  Program to test minilab 1008
#
#               Copyright (C) February 5, 2005
#               Written by:  Warren J. Jasper
#                            North Carolina State Univerisity
#
#
# This program, libmccusb.so, is free software; you can redistribute it
# and/or modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version, provided that this
# copyright notice is preserved on all copies.
#
# ANY RIGHTS GRANTED HEREUNDER ARE GRANTED WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, AND FURTHER,
# THERE SHALL BE NO WARRANTY AS TO CONFORMITY WITH ANY USER MANUALS OR
# OTHER LITERATURE PROVIDED WITH SOFTWARE OR THAM MY BE ISSUED FROM TIME
# TO TIME. IT IS PROVIDED SOLELY "AS IS".
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
###########################################################################

#  Current Version of the driver
VERSION=1.12

SRCS    = pmd.c usb-1208LS.c usb-1024LS.c usb-1208FS.c usb-1608FS.c usb-tc.c usb-temp.c usb-1096HFS.c usb-1616FS.c minilab-1008.c
HEADERS = pmd.h usb-1208LS.h usb-1024LS.h usb-1208FS.h usb-1608FS.h usb-tc.h usb-temp.h usb-1096HFS.h usb-1616FS.h minilab-1008.h
OBJS = $(SRCS:.c=.o)   # same list as SRCS with extension changed
CC=gcc
CFLAGS= -g -Wall
TARGETS=libmccusb.so libmccusb.a test-usb1208LS test-usb1024LS test-usb1208FS test-usb1608FS test-usb-tc test-usb-temp test-usb1096HFS test-usb1616FS test-minilab1008
ID=MCCUSBHID
DIST_NAME=$(ID).$(VERSION).tgz
DIST_FILES={README,License,Makefile,usb-1208LS.h,usb-1208LS.c,usb-1024LS.h,usb-1024LS.c,usb-1208FS.h,usb-1208FS.c,usb-1608FS.h,usb-1608FS.c,usb-tc.h,usb-tc.c,usb-temp.h,usb-temp.c,usb-1096HFS.h,usb-1096HFS.c,usb-1616FS.h,usb-1616FS.c,pmd.h,pmd.c,minilab-1008.h,minilab-1008.c,test-usb1208LS.c,test-usb1024LS.c,test-usb1208FS.c,test-usb1608FS.c,test-usb-tc.c,test-usb-temp.c,test-usb1096HFS.c,test-usb1616FS.c,test-minilab1008.c}

###### RULES
all: $(TARGETS)

%.d: %.c
	set -e; $(CC) -I. -M $(CPPFLAGS) $< \
	| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@
ifneq ($(MAKECMDGOALS),clean)
include $(SRCS:.c=.d)
endif

libmccusb.so: $(OBJS)
	$(CC) -shared -Wall $(OBJS) -o $@

libmccusb.a: $(OBJS)
	ar -r libmccusb.a $(OBJS)
	ranlib libmccusb.a

test-usb1208LS:	test-usb1208LS.c libmccusb.a
	$(CC) -g -Wall -I. -o $@  $@.c -lmccusb -L. -lm

test-usb1024LS:	test-usb1024LS.c libmccusb.a
	$(CC) -g -Wall -I. -o $@  $@.c -lmccusb -L. -lm

test-usb1208FS:	test-usb1208FS.c libmccusb.a
	$(CC) -g -Wall -I. -o $@  $@.c -lmccusb -L. -lm

test-usb1608FS:	test-usb1608FS.c libmccusb.a
	$(CC) -g -Wall -I. -o $@  $@.c -lmccusb -L. -lm

test-usb-tc:	test-usb-tc.c libmccusb.a
	$(CC) -g -Wall -I. -o $@  $@.c -lmccusb -L. -lm

test-usb-temp:	test-usb-temp.c libmccusb.a
	$(CC) -g -Wall -I. -o $@  $@.c -lmccusb -L. -lm

test-usb1096HFS: test-usb1096HFS.c libmccusb.a
	$(CC) -g -Wall -I. -o $@  $@.c -lmccusb -L. -lm

test-usb1616FS: test-usb1616FS.c libmccusb.a
	$(CC) -g -Wall -I. -o $@  $@.c -lmccusb -L. -lm

test-minilab1008: test-minilab1008.c libmccusb.a
	$(CC) -g -Wall -I. -o $@  $@.c -lmccusb -L. -lm

clean:
	rm -rf *.d *.o *~ *.a *.so $(TARGETS)

dist:	
	make clean
	cd ..; tar -zcvf $(DIST_NAME) usbhid/$(DIST_FILES);

install:
	-install -d /usr/local/lib
	-install -c ./libmccusb.a libmccusb.so /usr/local/lib

uninstall:
	-rm -f /usr/local/lib/libmccusb*
