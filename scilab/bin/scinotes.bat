@ECHO OFF
SETLOCAL ENABLEEXTENSIONS
SET argC=0
SET bIndent=0
SET "argList=["
SET "cmd1=WScilex -e"
FOR %%x IN (%*) DO Set /A argC+=1


IF %argC% EQU 0 (
    START %cmd1% scinotes
    EXIT /B
)

FOR %%x IN (%*) DO (
    IF %%x == -indent (
        SET bIndent=1
    ) ELSE (
        call :strcat %%x
    )
)

set "argList=%argList% ]"

IF %argC% EQU 1 (
    IF %bIndent% == 1 (
        goto :Syntax
    )
)

IF %bIndent% EQU 1 (
    START %cmd1% "scinotes(%argList%, ['indent','trailing','quote']);exit();"
) else (
    START %cmd1% "scinotes(%argList%);"
)


EXIT /B


:Syntax
ECHO Start Scinotes, the Scilab text editor
ECHO Usage:
ECHO scinotes [ file1.sci, [ file2.sce ]]
ECHO.
ECHO     Possible arguments are:
ECHO       -indent : format (indent, trailing spaces, quote mismatch) the files
EXIT /B

:strcat
set "argList=%argList% '%1', "
goto :eof
