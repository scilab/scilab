SHELL = /bin/sh

# If necessary, modify the path of Maple V.4 or Maple V.5 below
MAPLE = maple

include ../Path.incl
include ../Makefile.incl

all:: maple2scilab.mpl

maple2scilab.mpl: maple2scilab.mpl.g
	@$(RM) maple2scilab.mpl
	@sed -e "s+SCILAB_DIRECTORY+$(SCIDIR)+" maple2scilab.mpl.g > \
        maple2scilab.mpl;
	@echo maple2scilab.mpl created

test:
	$(MAPLE) < examples.mpl > examples.out
	$(SCIDIR)/util/scidem $(SCIDIR) examples.tst examples.dia

clean::
	$(RM) foo* examples.out examples.dia

distclean::
	$(RM) maple2scilab.mpl foo* examples.out examples.dia
