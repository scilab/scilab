SHELL = /bin/sh
SCIDIR=../../

all:: 
	Makesubdirs.bat all

distclean::
	Makesubdirs.bat distclean 

clean::
	Makesubdirs.bat clean 

include ../../Makefile.incl.mak






