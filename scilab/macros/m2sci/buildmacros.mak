all:: kernel percent sci_files

kernel	:
	@cd kernel
	@makemacros.bat
	@cd ..

percent	:
	@cd percent
	@makemacros.bat
	@cd ..

sci_files:
	@cd sci_files
	@makemacros.bat
	@cd ..



clean::
	-del *.bin /s
	-del names.* /s
	-del genlib /s
