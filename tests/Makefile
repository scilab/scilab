
SHELL = /bin/sh
SCIDIR = ../

SUBDIRS = basic_tests \
			java \
			m2sci_tests \
			Benchmarks

distclean::
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making distclean in $$i..."; \
		$(MAKE) distclean); \
	done

clean::
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making clean in $$i..."; \
		$(MAKE) clean); \
	done

tests::
	@for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; \
		echo "================================================="; \
		echo ""; \
		echo ""; \
		echo "making tests in $$i..."; \
		echo ""; \
		echo ""; \
		echo "================================================="; \
		echo ""; \
		$(MAKE) tests); \
	done
