all::
	cl /c ../f2c/libf2c/main.c 
	cl /c -DWIN32 javasci_Matrix.c 
	link ..\..\bin\libscilab.lib -nologo -debug -dll -out:..\..\bin\javasci.dll javasci_Matrix.obj main.obj
	cd classes
	@MakeJavaClass.bat
	cd ..

clean::
	-del main.obj
	-del javasci_Matrix.obj
	-del ..\..\bin\javasci.* /s
	-del ..\..\bin\javasci\*.class /s