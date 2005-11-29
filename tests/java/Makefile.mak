
SCIDIR1 = ..\..

include ..\..\Makefile.incl.mak

tests:
	$(SCIDIR1)\bin\scilex -nw -f JavaTests.sce

clean ::
	@del *.dia

distclean::
	@del *.dia
