
SCIDIR1 = ..\..

tests:
	$(SCIDIR1)\bin\scilex -nb -f JavaTests.sce

clean ::
	@del *.dia

distclean::
	@del *.dia
