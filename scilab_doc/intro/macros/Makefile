SHELL = /bin/sh

SCIDIR=../../..
include ../../../Makefile.incl

.SUFFIXES: .sci .vtex $(SUFFIXES)

VTEX = make_macro.vtex

all:: $(VTEX)

distclean::
	$(RM) $(VTEX)

.sci.vtex:
#	@echo '\\begin{verbatim}' > begv
	@cat begv $*.sci > prov
#	@echo '\end{verbatim}' > endv
	@cat prov endv > $*.vtex
	@$(RM) prov
#	@$(RM) begv endv
