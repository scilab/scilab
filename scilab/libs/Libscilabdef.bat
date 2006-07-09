@rem Allan CORNEI
@rem INRIA 2006
@rem Create Libscilab.def with all .def 

if EXIST libg2c.def del libg2c.def >NUL
if EXIST libscilab.def del libscilab.def >NUL
if EXIST defforscilab del defforscilab >NUL

dir *.def /B /ON > defforscilab
TYPE NUL>Libscilab.def
setlocal
for /F  %%i in (defforscilab) do @call :AddDef %%i
endlocal

del defforscilab >NUL
copy LibScilab.def ..\bin\LibScilab.def >NUL
goto :EOF


:AddDef
@echo ; --------------------------------------- >>Libscilab.def
@echo ; %1 >>Libscilab.def
@echo ; --------------------------------------- >>Libscilab.def
@copy /Y Libscilab.def+%1 LibScilab.def >NUL


:EOF
