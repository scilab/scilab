
build:: fr eng 
 
fr	:
	@echo Build Scilab Manual CHM (French)
	@""%ProgramFiles%\HTML Help Workshop\hhc.exe" ../sciman.hhp"
	@del whatis_chm.htm /s
	

eng	:
	@echo Build Scilab Manual CHM (English)	
	@""%ProgramFiles%\HTML Help Workshop\hhc.exe" ../sciman.hhp"
	@del whatis_chm.htm /s

distclean::	
clean::
	-del *.chm
	-del *.chm
	