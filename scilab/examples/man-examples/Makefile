SHELL = /bin/sh

SCIDIR=../..

DIR1 = $(SCIDIR)/examples/man-examples/helpdir1
DIR2 = $(SCIDIR)/examples/man-examples/helpdir2
DIRS = $(DIR1) $(DIR2)

all::
tests::

clean::
	@for i in $(DIRS) ;\
	do \
		(cd $$i ; echo "making clean in $$i..."; \
		rm -f  *.cat whatis ) ; \
	done

distclean:: clean
