SCIDIR=..
include ..\Makefile.incl.mak

all ::
	Makesubdirs.bat all 

clean::
	Makesubdirs.bat clean 

distclean::
	Makesubdirs.bat distclean 

tests::
	Makesubdirs.bat tests

