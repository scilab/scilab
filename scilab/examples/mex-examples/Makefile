SHELL = /bin/sh

SCIDIR=../../
include ../../Makefile.incl


SUBDIRS =  cmex  fmex mexglx mexdll

info	:
	@echo "Type \"make tests\" in examples directory"
	@echo "  to test mex features " 

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

TEST_SUBDIRS =  cmex

tests::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(TEST_SUBDIRS) ;\
	do \
		(cd $$i ; echo "making tests in examples/$$i..."; \
		$(MAKE) $(MFLAGS)  tests); \
	done
