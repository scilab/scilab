@ECHO OFF
SETLOCAL ENABLEEXTENSIONS
SET argC=0
SET bSimu=0
SET "argList=["
SET "cmd1=WScilex -e"
FOR %%x IN (%*) DO Set /A argC+=1


IF %argC% EQU 0 (
    START %cmd1% xcos
    EXIT /B
)

FOR %%x IN (%*) DO (
    IF %%x == -simulate (
        SET bSimu=1
    ) ELSE (
        call :strcat %%x
    )
)

set "argList=%argList% ]"

IF %argC% EQU 1 (
    IF %bSimu% EQU 1 (
        goto :Syntax
    )
)

IF %argC% GTR 2 (
    IF %bSimu% == 1 (
        echo Wrong Syntax: When -simulate is used, only one file must be provided
        EXIT /B 43
    )
)

IF %bSimu% EQU 1 (
    START %cmd1% "importXcosDiagram(%argList%);xcos_simulate(scs_m, 4);"
) ELSE (
    START %cmd1% "xcos(%argList%)"
)
EXIT /B


:Syntax
ECHO Start Xcos, the hybrid simulator
ECHO Usage:
ECHO xcos [ file1.xcos, [ file2.xcos ]]
ECHO.
ECHO     Possible arguments are:
ECHO       -simulate : Start the simulation. Only one file will be accepted.
EXIT /B

:strcat
set "argList=%argList% '%1', "
goto :eof
