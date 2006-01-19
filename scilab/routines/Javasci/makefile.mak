all::
	cl /c ../f2c/libf2c/main.c 
	cl /c -DWIN32 javasci_globals.c
	cl /c -DWIN32 javasci_Scilab.c
	cl /c -DWIN32 javasci_SciReal.c
	cl /c -DWIN32 javasci_SciString.c
	cl /c -DWIN32 DllmainJavasci.c
	link ..\..\bin\libscilab.lib -nologo -debug -dll -out:..\..\bin\javasci.dll javasci_globals.obj javasci_Scilab.obj javasci_SciReal.obj javasci_SciString.obj main.obj DllmainJavasci.obj
	cd classes
	@MakeJavaClass.bat
	cd ..

clean::
	-del *.obj
	-del ..\..\bin\javasci.* /s
	-del ..\..\bin\javasci\*.class /s