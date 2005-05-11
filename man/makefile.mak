all:: check eng fr

build:: eng fr

check:
  @Makesubdirs.bat
  
fr	:
	@cd fr
	@echo Creation de l'aide (Francais)
	@..\..\bin\scilex -nb -nouserstartup -nwni -l fr -e xmltohtml([],[],'html-rev.xsl');exit;
	@cd..

eng	:	
	@cd eng
	@echo Creation of the help (English)
	@..\..\bin\scilex -nb -nouserstartup -nwni -l eng -e xmltohtml([],[],'html-rev.xsl');exit;
	@cd..

distclean::	
clean::
	-del eng\arma\*.htm /s
	-del eng\control\*.htm /s
	-del eng\dcd\*.htm /s
	-del eng\elementary\*.htm /s
	-del eng\fileio\*.htm /s
	-del eng\functions\*.htm /s
	-del eng\graphics\*.htm /s
	-del eng\gui\*.htm /s
	-del eng\identification\*.htm /s
	-del eng\linear\*.htm /s
	-del eng\metanet\*.htm /s
	-del eng\mtlb\*.htm /s
	-del eng\nonlinear\*.htm /s
	-del eng\polynomials\*.htm /s
	-del eng\programming\*.htm /s
	-del eng\pvm\*.htm /s
	-del eng\robust\*.htm /s
	-del eng\signal\*.htm /s
	-del eng\sound\*.htm /s
	-del eng\statistics\*.htm /s
	-del eng\strings\*.htm /s
	-del eng\tdcs\*.htm /s
	-del eng\tksci\*.htm /s
	-del eng\translation\*.htm /s
	-del eng\utilities\*.htm /s
	-del eng\sparse\*.htm /s
# -del eng\scicos\*.htm /s

	-del fr\arma\*.htm /s
	-del fr\control\*.htm /s
	-del fr\dcd\*.htm /s
	-del fr\elementary\*.htm /s
	-del fr\fileio\*.htm /s
	-del fr\functions\*.htm /s
	-del fr\graphics\*.htm /s
	-del fr\gui\*.htm /s
	-del fr\identification\*.htm /s
	-del fr\linear\*.htm /s
	-del fr\metanet\*.htm /s
	-del fr\mtlb\*.htm /s
	-del fr\nonlinear\*.htm /s
	-del fr\polynomials\*.htm /s
	-del fr\programming\*.htm /s
	-del fr\pvm\*.htm /s
	-del fr\robust\*.htm /s
	-del fr\signal\*.htm /s
	-del fr\sound\*.htm /s
	-del fr\statistics\*.htm /s
	-del fr\strings\*.htm /s
	-del fr\tdcs\*.htm /s
	-del fr\tksci\*.htm /s
	-del fr\translation\*.htm /s
	-del fr\utilities\*.htm /s
	-del fr\sparse\*.htm /s
# -del fr\scicos\*.htm /s
# -del *.htm /s
# -del *.xml2* /s

	