SHELL = /bin/sh

SUBDIRS = 

all::

demos::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making" demos "in demos/$$i..."; \
			$(MAKE) $(MFLAGS) all); \
	done

distclean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making distclean in demos/$$i..."; \
		$(MAKE) $(MFLAGS)  distclean); \
	done

clean::
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(SUBDIRS) ;\
	do \
		(cd $$i ; echo "making clean in demos/$$i..."; \
		$(MAKE) $(MFLAGS) clean); \
	done
