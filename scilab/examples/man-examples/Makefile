SHELL = /bin/sh
# just to know where is scilab 
include ../../Path.incl

# to get FILES_TO_CLEAN

include $(SCIDIR)/Makefile.incl

# provide the list of sub-directories and format request 
# in format.sci 

all:: 
	$(SCIDIR)/bin/scilab -nw -f "format.sci"
tests   :  Makefile 
	@echo "No tests to do in man-examples"

distclean::
	$(RM)  $(FILES_TO_CLEAN) */*.cat

clean::
	$(RM)  $(FILES_TO_CLEAN) 
