
SCIDIR1 = ..\..

tests:
	$(SCIDIR1)\bin\scilex -nw -f JavaTests.sce

clean ::
	@del *.dia

distclean::
	@del *.dia
