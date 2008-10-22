@rem Allan CORNET
@rem INRIA 2006
@rem Create Libscilab.def with all .vcdef 

@type headerLibScilab.vcdef > ..\LibScilab.def

:Concatfiles
setlocal
for %%f in (..\*.vcdef) do @call :AddDef %%f
endlocal
goto :EOF

:AddDef
@echo ; --------------------------------------- >>..\LibScilab.def
@echo ; %1 >>..\LibScilab.def
@echo ; --------------------------------------- >>..\LibScilab.def
@type %1 >> ..\LibScilab.def

:EOF
