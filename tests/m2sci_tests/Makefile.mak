
SCIDIR1 = ..\..

include ..\..\Makefile.incl.mak

tests:
	$(SCIDIR1)\bin\scilex -nw -f m2sci.sce

clean ::
	$(SCIDIR1)\bin\scilex -nw -f clean.sce

distclean::
	$(SCIDIR1)\bin\scilex -nw -f clean.sce
