
SHELL = /bin/sh

SCIDIR=../
include ../Makefile.incl

all	:: info 

info	:
	@echo "Type \"make tests\" in examples directory"
	@echo "  to test link and addinter features"

SUBDIRS = interface-tour interface-tour-so \
	interface-tutorial interface-tutorial-so \
	interface-lapack intersci-examples intersci-examples-so \
	link-examples link-examples-so \
	misc-examples man-examples macros-examples mex-examples 

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

TEST_SUBDIRS = interface-tour interface-tour-so \
	interface-tutorial interface-tutorial-so \
	interface-lapack intersci-examples intersci-examples-so \
	link-examples link-examples-so \
	misc-examples man-examples macros-examples mex-examples 

tests::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(TEST_SUBDIRS) ;\
	do \
		(cd $$i ; echo "making tests in examples/$$i..."; \
		$(MAKE) $(MFLAGS)  tests); \
	done
