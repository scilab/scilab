SHELL = /bin/sh
# just to know where is scilab 
include ../../Path.incl

# to get FILES_TO_CLEAN
include $(SCIDIR)/Makefile.incl

# provide the list of sub-directories and format request 
# in format.sci 

all:: 
	@echo running builder 
	@$(SCIDIR)/util/scidem $(SCIDIR) builder.sce builder.dia

tests   :  Makefile 
	@echo "No tests to do in man-examples"

distclean::
	$(RM)  $(FILES_TO_CLEAN)
	$(RM)  */*.xml2 */*.htm *.htm sciman.hh*

clean::
	$(RM)  $(FILES_TO_CLEAN) 

