all:: libmex libmx libmat

libmex	:
	@cd libmex
	@echo Construction of libmex.dll
	..\..\..\bin\scilex -e exec('libmex.sce');exit;
	@copy libmex.dll ..\..\..\bin\libmex.dll
	@cd..

libmx	:	
	@cd libmx
	@echo Construction of libmx.dll
	..\..\..\bin\scilex -e exec('libmx.sce');exit;
	@copy libmx.dll ..\..\..\bin\libmx.dll
	@cd..


libmat	:
	@cd libmat
	@echo Construction of libmat.dll
	..\..\..\bin\scilex -e exec('libmat.sce');exit;
	@copy libmat.dll ..\..\..\bin\libmat.dll
	@cd..


clean::
	@cd libmex
	-del loader.sce	
	-del makelib.mak
	-del *.exp	
	-del *.ilib	
	-del *.def	
	-del *.obj	
	-del *.dll
	@cd..
	@cd libmx
	-del loader.sce	
	-del makelib.mak
	-del *.exp
	-del *.ilib	
	-del *.def	
	-del *.obj	
	-del *.dll
	@cd..
	@cd libmat
	-del loader.sce	
	-del makelib.mak
	-del *.exp
	-del *.ilib	
	-del *.def	
	-del *.obj	
	-del *.dll
	@cd..
