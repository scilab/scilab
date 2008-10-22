SCIDIR=../../..
SCIDIR1=..\..\..



all:: tests

clean::
	@del *.dia 

distclean:: clean

tests	:
	@"$(SCIDIR1)\bin\scilex.exe" -nwni -nb -e scitest('odedc.sce',%t);quit; 
	@"$(SCIDIR1)\bin\scilex.exe" -nwni -nb -e scitest('odeoptions.sce',%t);quit; 







