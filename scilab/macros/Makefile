SHELL = /bin/sh

include ../Path.incl
include ../Makefile.incl

CFLAGS = $(CC_OPTIONS) 

SUBDIRS = algebre arma auto calpol \
	elem metanet optim percent robust sci2for \
	signal tdcs util xdess scicos scicos_blocks sound \
	m2sci mtlb int

all:: Progs 
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making all in macros/$$i..."; \
		$(MAKE) $(MFLAGS) all); \
	done

release : Progs
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making all in macros/$$i..."; \
		$(MAKE) $(MFLAGS) release ); \
	done

distclean::
	$(RM) Lib Name Gengatsce Gensosce Gensomex Tomake Forwin
	$(RM) Lib.o Name.o Gengatsce.o Gensosce.o Gesomex.o Tomake.o Forwin.o

distclean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making distclean in macros/$$i..."; \
		$(MAKE) $(MFLAGS)  distclean); \
	done

clean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making clean in macros/$$i..."; \
		$(MAKE) $(MFLAGS) clean); \
	done

Progs : Lib Name Gengatsce Gensosce Gensomex Tomake Forwin

Lib:  Lib.c 
	$(RM) $@
	$(CC) $(CFLAGS) -o $@ $?  $(CC_LDFLAGS) 

Name:  Name.c 
	$(RM) $@
	$(CC) $(CFLAGS) -o $@ $?  $(CC_LDFLAGS) 

Gengatsce:  Gengatsce.c 
	$(RM) $@
	$(CC) $(CFLAGS) -o $@ $?  $(CC_LDFLAGS) 

Gensosce:  Gensosce.c 
	$(RM) $@
	$(CC) $(CFLAGS) -o $@ $?  $(CC_LDFLAGS) 

Gensomex:  Gensomex.c 
	$(RM) $@
	$(CC) $(CFLAGS) -o $@ $?  $(CC_LDFLAGS) 

Tomake:  Tomake.c 
	$(RM) $@
	$(CC) $(CFLAGS) -o $@ $?  $(CC_LDFLAGS) 

Forwin:  Forwin.c 
	$(RM) $@
	$(CC) $(CFLAGS) -o $@ $?  $(CC_LDFLAGS) 




