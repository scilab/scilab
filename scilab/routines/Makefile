# Generated automatically from Makefile.in by configure.
SHELL = /bin/sh

include ../Makefile.incl

SCIDIRS = graphics calelm blas comm control default integ interf intersci \
	lapack \
	libcomm metanet optim poly signal sparse sun system system2 \
	menusX scicos sound dcd randlib \
	gd int xsci  tksci pvm

SUBDIRS = $(XAW_LOCAL_SUBDIR) $(DLD_SUBDIR) $(SCIDIRS) 

all::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making all in routines/$$i..."; \
		$(MAKE) $(MFLAGS) all); \
	done

distclean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in X11 dld $(SCIDIRS) ;\
	do \
		(cd $$i ; echo "making distclean in routines/$$i..."; \
		$(MAKE) $(MFLAGS)  distclean); \
	done

clean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making clean in routines/$$i..."; \
		$(MAKE) $(MFLAGS) clean); \
	done

f2cclean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making f2cclean in routines/$$i..."; \
		$(MAKE) $(MFLAGS) f2cclean); \
	done
