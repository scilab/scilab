all:: eng fr


fr	:
	@cd fr
	@echo Creation de l'aide (Francais)
	..\..\bin\scilex -e xmltohtml([],[],'html-rev.xsl');exit;
	@cd..

eng	:	
	@cd eng
	@echo Creation of the help (English)
	..\..\bin\scilex -e xmltohtml([],[],'html-rev.xsl');exit;
	@cd..
	
clean::
	-del *.htm /s
	-del *.xml2* /s