#=============================================================================
#Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
#Copyright (C) 2015 - Scilab-Enterprises - Cedric Delamarre
#This file is distributed under the same license as the Scilab package.
#=============================================================================

# these two environment variables must be defined
# export SCI=~/your/install/path/share/scilab
# export LD_LIBRARY_PATH=~/your/install/path/lib/scilab/:~/your/install/path/lib/thirdparty/

CC=gcc
EXEC=	bug_7601 \
		bug_7602 \
		bug_8115 \
		bug_8830

SRC= $(EXEC:=.c)

CFLAGS=	-W -Wall -Werror=implicit-function-declaration \
		-I../../../../../../include/scilab/

LDFLAGS=	-L../../../../../../lib/scilab/ \
			-L../../../../../../lib/thirdparty/ \
			-lscicall_scilab \
			-lscilocalization \
			-lstdc++ \
			-lscilab-cli


all: $(EXEC)


bug_7601: bug_7601.o
	$(CC) -o $@ $^ $(LDFLAGS)

bug_7602: bug_7602.o
	$(CC) -o $@ $^ $(LDFLAGS)

bug_8115: bug_8115.o
	$(CC) -o $@ $^ $(LDFLAGS) -lsciast

bug_8830: bug_8830.o
	$(CC) -o $@ $^ $(LDFLAGS)


%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf $(EXEC)
