
SCIDIR1 = ..\..

tests:
	$(SCIDIR1)\bin\scilex -nb -f benchmark.sce

clean ::
	@del $(SCIDIR1)\benchmark.ini

distclean::
	@del $(SCIDIR1)\benchmark.ini
