SHELL = /bin/sh

include ../../Path.incl
include ../../Makefile.incl

CFLAGS = $(CC_OPTIONS) 

SUBDIRS = Branching Linear Sinks Misc Sources Events NonLinear Threshold Electrical Hydraulics


all:: 
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making all in macros/scicos_blocks/$$i..."; \
		$(MAKE) $(MFLAGS) all); \
	done

release : 
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making all in macros/scicos_blocks/$$i..."; \
		$(MAKE) $(MFLAGS) release ); \
	done


distclean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making distclean in macros/scicos_blocks/$$i..."; \
		$(MAKE) $(MFLAGS)  distclean); \
	done

clean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making clean in macros/scicos_blocks/$$i..."; \
		$(MAKE) $(MFLAGS) clean); \
	done




