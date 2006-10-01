@rem Allan CORNEI
@rem INRIA 2006
@rem Create Libscilab.def with all .vcdef 
if NOT EXIST ..\LibScilab.def @call :NewLibScilabDef
:Concatfiles
setlocal
for %%f in (%*) do @call :AddDef %%f
endlocal
goto :EOF

:NewLibScilabDef
@copy /Y ..\headerLibScilab.vcdef ..\LibScilab.def >NUL
@copy /Y ..\LibScilab.def+..\ExplicitExportsLibScilab.vcdef ..\LibScilab.def >NUL
goto :Concatfiles


:AddDef
@echo ; --------------------------------------- >>..\LibScilab.def
@echo ; %1 >>..\LibScilab.def
@echo ; --------------------------------------- >>..\LibScilab.def
@copy /Y ..\LibScilab.def+%1 ..\LibScilab.def >NUL


:EOF