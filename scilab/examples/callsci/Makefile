SHELL = /bin/sh

SCIDIR=../../
include ../../Makefile.incl

all	:: info 

info	:
	@echo "How to call Scilab from Fortran C C++ Java "
	@echo "walk through the subdirs to learn more "

SUBDIRS = callsciC callsciC++ callsciFortran callsciJava
 
distclean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making distclean in examples/$$i..."; \
		$(MAKE) $(MFLAGS)  distclean); \
	done

clean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making clean in examples/$$i..."; \
		$(MAKE) $(MFLAGS) clean); \
	done

tests::
	@echo "How to call Scilab from Fortran C C++ Java "
	@echo "Automatic tests are not performed" 
