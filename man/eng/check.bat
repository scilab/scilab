@echo off
@..\..\..\Win-util\xmlint\xmlint.exe *.xml > xmlint.tmp
IF NOT ERRORLEVEL 1 GOTO OK
echo          Problem(s) detected (See xmlint.tmp)
GOTO END
:OK
del xmlint.tmp
:END
@echo on