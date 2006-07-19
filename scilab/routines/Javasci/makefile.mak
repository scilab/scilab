all::
	cl /c ../f2c/libf2c/main.c 
	cl /c -DWIN32 javasci_globals.c
	cl /c -DWIN32 javasci_Scilab.c
	cl /c -DWIN32 javasci_Scilab2.c
	cl /c -DWIN32 javasci_SciDoubleArray.c
	cl /c -DWIN32 javasci_SciStringArray.c
	cl /c -DWIN32 javasci_SciComplexArray.c
	cl /c -DWIN32 DllmainJavasci.c
	link ..\..\bin\libscilab.lib -nologo -debug -dll -out:..\..\bin\javasci.dll javasci_globals.obj javasci_Scilab.obj javasci_Scilab2.obj javasci_SciDoubleArray.obj javasci_SciStringArray.obj javasci_SciComplexArray.obj main.obj DllmainJavasci.obj
	cd classes
	@MakeJavaClass.bat
	cd ..

clean::
	-del *.obj
	-del ..\..\bin\javasci.* /s
	-del ..\..\bin\javasci\*.class /s