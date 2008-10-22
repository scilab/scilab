
SCIDIR1 = ..\..

tests:
	$(SCIDIR1)\bin\scilex -nb -f m2sci.sce

clean ::
	$(SCIDIR1)\bin\scilex -nb -f clean.sce

distclean::
	$(SCIDIR1)\bin\scilex -nb -f clean.sce
