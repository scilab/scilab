SHELL = /bin/sh

include ../../Path.incl
include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) 

SUBDIRS = kernel percent sci_files


all:: 
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "Making all in macros/m2sci/$$i..."; \
		$(MAKE) $(MFLAGS) all); \
	done

release : 
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "Making all in macros/m2sci/$$i..."; \
		$(MAKE) $(MFLAGS) release ); \
	done


distclean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "Making distclean in macros/m2sci/$$i..."; \
		$(MAKE) $(MFLAGS)  distclean); \
	done

clean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "Making clean in macros/m2sci/$$i..."; \
		$(MAKE) $(MFLAGS) clean); \
	done




