
build:: fr eng 
 
fr	:
	@echo Build Scilab Manual CHM (French)
	@""%ProgramFiles%\HTML Help Workshop\hhc.exe" ..\fr\sciman.hhp"
	@del ..\fr\whatis_chm.htm /s
	

eng	:
	@echo Build Scilab Manual CHM (English)	
	@""%ProgramFiles%\HTML Help Workshop\hhc.exe" ..\eng\sciman.hhp"
	@del ..\eng\whatis_chm.htm /s

distclean::	
clean::
	-del ..\eng\*.chm
	-del ..\fr\*.chm
	