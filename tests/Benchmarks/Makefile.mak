
SCIDIR1 = ..\..

include ..\..\Makefile.incl.mak

tests:
	$(SCIDIR1)\bin\scilex -nw -f benchmark.sce

clean ::
	@del $(SCIDIR1)/benchmark.ini

distclean::
	@del $(SCIDIR1)/benchmark.ini
