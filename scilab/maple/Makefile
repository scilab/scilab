SHELL = /bin/sh

include ../Makefile.incl

# If necessary, change the path of Maple below
MAPLE = maple
# If necessary, change the path of Scilab below
SCIDIR= /usr/local/lib/scilab

tests:
	$(MAPLE) < examples.mpl > examples.out
	$(SCIDIR)/util/scidem $(SCIDIR) examples.tst examples.dia

clean::
	$(RM) foo* examples.out examples.dia

distclean::
	$(RM) foo* examples.out examples.dia
