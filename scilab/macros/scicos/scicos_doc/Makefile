# $Header$

SUBDIRS            =
FILES_TO_DISTCLEAN =
FILES_TO_CLEAN     = *~

all::

clean::
	@$(RM) -r $(FILES_TO_CLEAN)

distclean::
	@$(RM) -r $(FILES_TO_CLEAN)
	@$(RM) -r $(FILES_TO_DISTCLEAN)

srctargz: distclean
	(cd ..; tar -czf scicos_doc_`date +%d_%m_%y`_src.tar.gz scicos_doc/)

